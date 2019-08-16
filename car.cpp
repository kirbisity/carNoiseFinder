#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
/* in current directory */
#include "Car.h"

using namespace std;

#define MAXDIFF 2
#define DBCONVERSION false /* determine if the noise fitting will use real energy value instead of dB value */

Car::Car(void) {
	target_noise = 0;
}
Car::Car(vector<string> n, int y, vector<double> se, vector<double> ns) {
	name = n;
	year = y;
	speeds = se;
	noises = ns;
	target_noise = 0;
}

void Car::guess_displacement(void) {
	int found = 0;
	for (vector<string>::const_iterator i = name.begin(); i != name.end(); ++i) {
		size_t dotpos = i->find(".");
		if ((dotpos >= 1) && (dotpos < i->size())) {
			int dp = (int) dotpos;
			string ds = i->substr(dp-1, dp+2);
			displacement = atof(ds.c_str()); 
			found = 1;
		}
	}
	if (found == 0) {
		displacement = 0;
	}
}

void Car::noises_curve_fitting(void) {
	vector<double> energies;
	for (vector<double>::const_iterator i = noises.begin(); i != noises.end(); ++i) {
		energies.push_back(db_to_energy(*i)/1000);
	}
	equation.fit(speeds, energies); // speed is the x axis, energy is the y axis
}

void Car::guess_noise(double speed) {
	target_noise = energy_to_db(1000*equation.get_value(speed));
}

string Car::print(void) const {
	string text;
	for (vector<string>::const_iterator i = name.begin(); i != name.end(); ++i) {
	    text += *i + " ";
	}
	text += to_string(year) + " ";
	if (target_noise != 0) {
		int tn = (int) target_noise;
		text += " predicted[ " + to_string(tn) + "dB ] ";
	}
	else {
		for (vector<double>::const_iterator i = noises.begin(); i != noises.end(); ++i) {
		    text += to_string(*i) + " ";
		}
	}
	text += "\n";
	return text;
}

string Car::print_name(void) const {
	string text;
	for (vector<string>::const_iterator i = name.begin(); i != name.end(); ++i) {
	    text += *i + " ";
	}
	return text;
}

void Car::print_displacement_noise(void) const {
	if (displacement != 0) {
		cout << displacement << " ";
		for (vector<double>::const_iterator i = noises.begin(); i != noises.end(); ++i) {
		    cout << *i << " ";
		}
		cout << endl;
	}
}

void Car::print_year_noise(void) const {
	cout << year << " ";
	for (vector<double>::const_iterator i = noises.begin(); i != noises.end(); ++i) {
	    cout << *i << " ";
	}
	cout << endl;
}

vector<string> Car::get_speed_graph(int endspeed, int step) const {
	vector<string> dbs;
	for (int s = 0; s < endspeed + step; s += step) {
		double db = energy_to_db(1000*equation.get_value((double) s));
		dbs.push_back(to_string(s) + " " + to_string(db) + "\n");
	}
	return dbs;
}

bool compare_engine_up(Car c0, Car c1) { 
    return (c0.displacement < c1.displacement); 
}
bool compare_engine_down(Car c0, Car c1) { 
    return (c0.displacement > c1.displacement); 
} 
bool compare_noise_up(Car c0, Car c1) { 
    return (c0.target_noise < c1.target_noise); 
}
bool compare_noise_down(Car c0, Car c1) { 
    return (c0.target_noise > c1.target_noise); 
}

int edit_distance(string word1, string word2) {
    int n = word1.length();
    int m = word2.length();
    if ((n== 0) || (m==0)) {
        return n + m;
    }
    int d[n + 1][m + 1];
    for (int i = 0; i < n + 1; i++) {
        d[i][0] = i;
    }
    for (int j = 0; j < m + 1; j++) {
        d[0][j] = j;
    }
    for (int i = 1; i < n + 1; i++) {
        for (int j = 1; j < m + 1; j++) {
            int left = d[i - 1][j] + 1;
            int down = d[i][j - 1] + 1;
            int left_down = d[i - 1][j - 1];
            if (word1[i - 1] != word2[j - 1]) {
                left_down += 1;
            }
            d[i][j] = min(left, min(down, left_down));
        }
    }
    return d[n][m];
}

bool similar_name(string word1, string word2) {
	transform(word1.begin(), word1.end(), word1.begin(), ::tolower);
	transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
	if (edit_distance(word1, word2) < MAXDIFF) {
		return 1;
	}
	return 0;
}

double energy_to_db(double e) {
	if (DBCONVERSION)
		return 10*log10(e);
	else
		return e;
}
double db_to_energy(double d) {
	if (DBCONVERSION) 
		return pow(10, (d/10));
	else
		return d;
}

vector<Car> search_car(vector<Car>& cars, string input) {
	stringstream inputs(input);
	istream_iterator<string> ips(inputs);
	istream_iterator<string> ipe;
	vector<string> carnames(ips, ipe);
	vector<Car> legitcars;
	int match = 0;
	for (vector<Car>::iterator i = cars.begin(); i != cars.end(); ++i) {
		int legit = 1;
		for (vector<string>::iterator j = carnames.begin(); j != carnames.end(); ++j) {
			int found = 0;
			for (vector<string>::iterator k = i->name.begin(); k != i->name.end(); ++k) {
				if (similar_name(*j, *k)) { // use edit distance on every pair of words to find matches
					found++;
				}
			}
			if (found == 0) {
				legit = 0;
			}
		}
		if (legit) {
			legitcars.push_back(*i);
			match++;
		}
	}
	return legitcars;
}