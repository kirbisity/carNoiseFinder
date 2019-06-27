#include "Car.h"

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
using namespace std;

#define NOISEENTRY 6

/* g++ parser.cpp Car.cpp -o parser.o */

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
			car.noises = noises;
			car.guess_displacement();
			cars.push_back(car);
		}
	}
	
	while (1) {
		string input = "";
		cout << "Enter the \"xxx\"km/h to print car noise data sorted by noise at the desired speed" << endl;
		cout << "Enter \"ed\" to print car noise data sorted by engine displacement" << endl;
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
		else if (input == "140") {
			cout << "Enter \"0\" to print sorted by noise at "<< input <<"km/h increasing order" << endl;
			cout << "Enter \"1\" to print sorted by noise at "<< input <<" decreasing order" << endl;
			getline(cin, input);
			for (vector<Car>::iterator i = cars.begin(); i != cars.end(); ++i) {
			    //i->print_displacement_noise();
			    i->guess_noise();
			}
			if (input == "0") {
				sort(cars.begin(), cars.end(), compare_noise_up);
			}
			else {
				sort(cars.begin(), cars.end(), compare_noise_down);
			}
			for (vector<Car>::iterator i = cars.begin(); i != cars.end(); ++i) {
			    //i->print_displacement_noise();
			    i->print();
			}
		}
		else if (input == "q") {
			break;
		}
		else {
			cout << "Unknown command: " << input << endl;
		}
		for (int i=0; i<2; ++i) {
			cout<<endl;
		}
	}
	data.close();
	return 0;
}