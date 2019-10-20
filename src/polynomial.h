#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

class Polynomial {
public:
	int degree;
	std::vector<double> coeffs; // from c, x, x2, x3 ...

	Polynomial(void);
	void fit(std::vector<double> x, std::vector<double> y);
	double get_value(double x) const;
	void print(void) const;
};

#endif