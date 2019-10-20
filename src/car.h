#ifndef CAR_H
#define CAR_H

/* in current directory */
#include "polynomial.h"

/*
target_noise: predicted noise level at specific speed
noises: accurate data points for noise at certain speeds
*/

class Car {
public:
	std::vector<std::string> name;
	int year;
	double displacement;
	double target_noise;
	std::vector<double> speeds;
	std::vector<double> noises;
	Polynomial equation;

	Car(void);
	Car(std::vector<std::string> n, int y, std::vector<double> se, std::vector<double> ns);
	void guess_displacement(void);
	void noises_curve_fitting(void);
	void guess_noise(double speed);
	std::string print(void) const;
	std::string print_name(void) const;
	void print_displacement_noise(void) const;
	void print_year_noise(void) const;
	std::vector<std::string> get_speed_graph(int endspeed, int step) const;
};

bool compare_engine_up(Car c0, Car c1);
bool compare_engine_down(Car c0, Car c1);
bool compare_noise_up(Car c0, Car c1);
bool compare_noise_down(Car c0, Car c1);
int edit_distance(std::string s0, std::string s1);
bool similar_name(std::string s0, std::string s1);
double energy_to_db(double e);
double db_to_energy(double d);
std::vector<Car> search_car(std::vector<Car>& cars, std::string input);

#endif