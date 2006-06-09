#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <iostream>

/**
	An dynamically-typed units class, for ease of
	integration with scripting languages and the like.

	Although the units.h class has no runtime overhead in C++, it
	imposes too much of a burden in Python as far as the 'width' of
	the SWIG interface, and makes declaring values etc painful.

	Also, type-checking with python becomes impossible since
	checks only work in units.h at compile-time.

	This class implements runtime declaration of values and the
	units.h class will be expanded to allow automatic casting
	from suitable Measurement values.
*/

class Dimension{
public:
	int m, l, t, k, i;
	~Dimension();
};

int dimension_cmp(const Dimension &d1, const Dimension &d2);
Dimension dimension_add(const Dimension &d1, const Dimension &d2);
Dimension dimension_sub(const Dimension &d1, const Dimension &d2);
Dimension dimension_mul(const Dimension &d, const int &n);

class Measurement{

	public:
		// the value, in base units
		double value;

		// the dimension of the measurement
		Dimension dim;

		Measurement(const double &value, const char *units);
		Measurement();
		Measurement(const double &value, const Dimension &dim, const double &scale=1);

		const std::string toString() const;
		bool isNonDim() const;
};


inline
std::ostream& operator <<(std::ostream &os,const Measurement &m){
	os << m.value << " <" << m.dim.m << "," << m.dim.l << "," << m.dim.t << "," << m.dim.k << "," << m.dim.i << ">";
	return os;
}


#endif
