#ifndef CAR_H
#define CAR_H
#endif

#include <vector>
#include <string>
/* in current directory */
#include "Polynomial.h"
using namespace std;

/*
target_noise: predicted noise level at specific speed
noises: accurate data points for noise at certain speeds
*/

class Car {
public:
	vector<string> name;
	int year;
	double displacement;
	double target_noise;
	vector<double> speeds;
	vector<double> noises;
	Polynomial equation;

	Car(void);
	void guess_displacement(void);
	void noises_curve_fitting(void);
	void guess_noise(double speed);
	void print(void) const;
	void print_displacement_noise(void) const;
	void print_year_noise(void) const;
};

bool compare_engine_up(Car c0, Car c1);
bool compare_engine_down(Car c0, Car c1);
bool compare_noise_up(Car c0, Car c1);
bool compare_noise_down(Car c0, Car c1);
int edit_distance(string s0, string s1);
bool similar_name(string s0, string s1);
double energy_to_db(double e);
double db_to_energy(double d);