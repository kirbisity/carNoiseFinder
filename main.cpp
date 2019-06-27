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
#include "parser.h"

using namespace std;

#define NOISEENTRY 6

/* g++ main.cpp parser.cpp Car.cpp Polynomial.cpp -o main.o */

int main() {
	vector<Car> cars;
	vector<double> speedentries;
	ifstream data;
	data.open("data.txt");
	ofstream output;
	output.open ("output.txt");
	if(!data) {
	    cout << "Cannot open input file" << endl;
	    return 1;
	}
	if(!output) {
	    cout << "Cannot open output file" << endl;
	    return 1;
	}
	string line;
	if (data) {
		getline(data, line);
		speedentries = parse_tabs(line);
	}
	
	while (data) {
		getline(data, line);
		vector<string> items = split(line);
		vector<string>::const_iterator i = items.end()-1;
		if ((items.size() > NOISEENTRY) && (atof(i->c_str()) != 0)) {
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
			Car car(name, year, speedentries, noises);
			car.guess_displacement();
			car.noises_curve_fitting();
			cars.push_back(car);
		}
	}
	
	while (1) {
		string input = "";
		cout << "Enter \"s\" to search for your car" << endl;
		cout << "Enter \"xxx\"km/h to list noise data sorted by noise at the desired speed" << endl;
		cout << "Enter \"ed\" to list noise data sorted by engine displacement" << endl;
		cout << "Enter \"q\" to quit" << endl;
		getline(cin, input);
		if (input == "ed") {
			cout << "Enter \"0\" to list noises by engine displacement increasing order" << endl;
			cout << "Enter \"1\" to list noises by engine displacement decreasing order" << endl;
			getline(cin, input);
			if (input == "0") {
				sort(cars.begin(), cars.end(), compare_engine_up);
			}
			else {
				sort(cars.begin(), cars.end(), compare_engine_down);
			}
			for (vector<Car>::iterator i = cars.begin(); i != cars.end(); ++i) {
			    if (i->displacement != 0) {
			    	cout << i->print();
			    	output << i->print();
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
			cout << "Enter \"0\" to list noises at "<< input <<"km/h increasing order" << endl;
			cout << "Enter \"1\" to list noises at "<< input <<"km/h decreasing order" << endl;
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
			    cout << i->print();
			    output << i->print();
			    i->target_noise=0;
			}
			cout<<"predicted noises are at "<<speed<<" km/h"<<endl;
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
					cout << i->print();
				    i->target_noise=0;
				}
				cout<<"predicted noises are at "<<speed<<" km/h"<<endl;
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
	output.close();
	return 0;
}