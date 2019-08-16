#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#endif
using namespace std;

class Polynomial {
public:
	int degree;
	std::vector<double> coeffs; // from c, x, x2, x3 ...

	Polynomial(void);
	void fit(vector<double> x, vector<double> y);
	double get_value(double x) const;
	void print(void) const;
};