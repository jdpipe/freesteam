#ifndef CONVERGENCETEST_H
#define CONVERGENCETEST_H

#include "units.h"

template<class Property,int PropertyAlternative=0>
class ConvergenceTest{

	private:
		static const double errorP = 1e-6;
		static const double errorH = 1e-6;
		static const double errorS = 0.0001;
		static const double errorV = 1e-10;
		static const double errorRHO = 0.0001;
		static const double errorU = 1e-12;
		static const double errorCP = 0.0001;
		static const double errorCV = 0.0001;
		static const double errorX = 0.00001;
		static const double errorT = 1e-10;

	public:
		static bool test(const Property &X, const Pressure &p, const Temperature &T);
};

#endif
