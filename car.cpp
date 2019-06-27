#include "Car.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;



Car::Car(void) {
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

/* We used third degree curve fitting to predict noises at different speeds */
void noises_curve_fitting(void) {

}

void Car::guess_noise(void) {
	target_noise = noises[5];
}

void Car::print(void) const {
	for (vector<string>::const_iterator i = name.begin(); i != name.end(); ++i) {
	    cout << *i << " ";
	}
	cout << displacement << " ";
	cout << year << " ";
	for (vector<double>::const_iterator i = noises.begin(); i != noises.end(); ++i) {
	    cout << *i << " ";
	}
	cout << target_noise << " ";
	cout << endl;
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

bool compare_engine_up(Car c0, Car c1) 
{ 
    return (c0.displacement < c1.displacement); 
}
bool compare_engine_down(Car c0, Car c1) 
{ 
    return (c0.displacement > c1.displacement); 
} 
bool compare_noise_up(Car c0, Car c1) 
{ 
    return (c0.target_noise < c1.target_noise); 
}
bool compare_noise_down(Car c0, Car c1) 
{ 
    return (c0.target_noise > c1.target_noise); 
} 