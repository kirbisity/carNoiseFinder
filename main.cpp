#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
/* in current directory */
#include "Car.h"

using namespace std;

#define NOISEENTRY 6

/* g++ main.cpp Car.cpp Polynomial.cpp -o main.o */

int main() {
	vector<Car> cars;
	ifstream data("data.txt");
	if(!data) {
	    cout << "Cannot open input file" << endl;
	    return 1;
	}
	while (data) {
		string line;
		getline(data, line);
		stringstream ss(line);
		istream_iterator<string> begin(ss);
		istream_iterator<string> end;
		vector<string> items(begin, end);
		//print out
		//copy(vstrings.begin(), vstrings.end(), ostream_iterator<string>(std::cout, "\n"));
		vector<string>::const_iterator i = items.end()-1;
		if ((items.size() > 7) && (atof(i->c_str()) != 0)) { // valid entry
			vector<string> name;
			int year;
			vector<double> noises;
			int count = 0;
			for (i = items.end()-1; i != items.begin()-1; --i) {
				if (count < NOISEENTRY) {
					double dbvalue = atof(i->c_str());
					noises.push_back(dbvalue);
				}
				else if (count == NOISEENTRY) {
					year = atoi(i->c_str());
				}
				else {
					name.push_back(*i);
				}
				count++;
	    	}
	    	reverse(name.begin(), name.end());
	    	reverse(noises.begin(), noises.end());
			Car car;
			car.name = name;
			car.year = year;
			car.speeds.push_back(0);
			car.speeds.push_back(50);
			car.speeds.push_back(80);
			car.speeds.push_back(100);
			car.speeds.push_back(120);
			car.speeds.push_back(140);
			car.noises = noises;
			car.guess_displacement();
			car.noises_curve_fitting();
			cars.push_back(car);
		}
	}
	
	while (1) {
		string input = "";
		cout << "Enter the \"xxx\"km/h to print car noise data sorted by noise at the desired speed" << endl;
		cout << "Enter \"ed\" to print car noise data sorted by engine displacement" << endl;
		cout << "Enter \"s\" to search for your car" << endl;
		cout << "Enter \"q\" to quit" << endl;
		getline(cin, input);
		if (input == "ed") {
			cout << "Enter \"0\" to print sorted by engine displacement increasing order" << endl;
			cout << "Enter \"1\" to print sorted by engine displacement decreasing order" << endl;
			getline(cin, input);
			if (input == "0") {
				sort(cars.begin(), cars.end(), compare_engine_up);
			}
			else {
				sort(cars.begin(), cars.end(), compare_engine_down);
			}
			for (vector<Car>::iterator i = cars.begin(); i != cars.end(); ++i) {
			    if (i->displacement != 0) {
			    	i->print();
			    }
			}
		}
		else if ((atof(input.c_str())!=0) || (input=="0") || (input=="0.0")) {
			double speed = atof(input.c_str());
			if (speed < 0) {
				cout << "speed should not be negative" << endl;
				continue;
			}
			if (speed > 400) {
				cout << "speed should not be above 400km/h" << endl;
				continue;
			}
			for (vector<Car>::iterator i = cars.begin(); i != cars.end(); ++i) {
			    i->guess_noise(speed);
			}
			cout << "Enter \"0\" to print sorted by noise at "<< input <<"km/h increasing order" << endl;
			cout << "Enter \"1\" to print sorted by noise at "<< input <<" decreasing order" << endl;
			getline(cin, input);
			if (input == "0") {
				sort(cars.begin(), cars.end(), compare_noise_up);
			}
			else if (input == "1") {
				sort(cars.begin(), cars.end(), compare_noise_down);
			}
			else {
				continue;
			}
			for (vector<Car>::iterator i = cars.begin(); i != cars.end(); ++i) {
			    i->print();
			    i->target_noise=0;
			}
			cout<<"predicted speeds are at "<<speed<<" km/h"<<endl;
		}
		else if (input == "s") {
			cout << "Enter the name of the car" << endl;
			getline(cin, input);
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
			if (match == 0) {
				cout<<"no car with the name was found"<<endl;
			}
			else {
				cout << "Enter the \"xxx\"km/h to print the noise data of the car" << endl;
				getline(cin, input);
				double speed = atof(input.c_str());
				if (speed < 0) {
					cout << "speed should not be negative" << endl;
					continue;
				}
				if (speed > 400) {
					cout << "speed should not be above 400km/h" << endl;
					continue;
				}
				for (vector<Car>::iterator i = legitcars.begin(); i != legitcars.end(); ++i) {
					i->guess_noise(speed);
					i->print();
				    i->target_noise=0;
				}
				cout<<"predicted speeds are at "<<speed<<" km/h"<<endl;
			}
		}
		else if (input == "q") {
			break;
		}
		else {
			cout << "Unknown command: " << input << endl;
		}
		cout<<endl;
		cout << "Enter to continue" << endl;
		getline(cin, input);
		if (input == "q") {
			break;
		}
	}

	data.close();
	return 0;
}