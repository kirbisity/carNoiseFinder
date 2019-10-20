#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
/* in current directory */
#include "polynomial.h"

using namespace std;

Polynomial::Polynomial(void) {
	degree = 1;
}

/* n: number of data points*/
void Polynomial::fit(vector<double> x, vector<double> y) {
	coeffs.clear();
	int N = 6;
	double X[2*degree+1];
	for (int i=0; i<2*degree+1; i++) {
        X[i]=0;
        for (int j=0;j<N;j++) {
            X[i]=X[i]+pow(x[j],i);
        }
    }
    double B[degree+1][degree+2];
    double a[degree+1];
    for (int i=0; i<=degree; i++) {
        for (int j=0; j<=degree; j++) {
            B[i][j]=X[i+j];
        }
    }
    double Y[degree+1];
    for (int i=0; i<degree+1; i++) {    
        Y[i]=0;
        for (int j=0;j<N;j++) {
        	Y[i]=Y[i]+pow(x[j],i)*y[j];
        }
    }
    for (int i=0; i<=degree; i++) {
        B[i][degree+1]=Y[i];
    }
    degree=degree+1;
    for (int i=0;i<degree;i++) {
    	for (int k=i+1;k<degree;k++) {
            if (B[i][i]<B[k][i]) {
                for (int j=0;j<=degree;j++) {
                    double temp=B[i][j];
                    B[i][j]=B[k][j];
                    B[k][j]=temp;
                }
            }
    	}
    }
    for (int i=0;i<degree-1;i++) {
        for (int k=i+1;k<degree;k++) {
            double t=B[k][i]/B[i][i];
            for (int j=0;j<=degree;j++) {
                B[k][j]=B[k][j]-t*B[i][j];
            }
        }
    }
    for (int i=degree-1;i>=0;i--) {
        a[i]=B[i][degree];
        for (int j=0;j<degree;j++) {
            if (j!=i) {
                a[i]=a[i]-B[i][j]*a[j];
            }
        }
        a[i]=a[i]/B[i][i];
    }
    for (int i=0;i<degree;i++) {
        coeffs.push_back(a[i]);   
    }
}

double Polynomial::get_value(double x) const{
	double val = 0;
	double base = 1;
	for (vector<double>::const_iterator i = coeffs.begin(); i != coeffs.end(); ++i) {
		val += *i * base;
		base *= x;
	}
	return val;
}

void Polynomial::print(void) const {
	int p = 1;
	if (coeffs.size()>0) {
		cout<<coeffs[0];
	}
	for (vector<double>::const_iterator i = coeffs.begin()+1; i != coeffs.end(); ++i) {
		cout<<" + "<<*i<<" x^"<<p;
		p++;
	}
	cout<<endl;
}