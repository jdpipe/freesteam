#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <string>

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

struct Dimension{
	int m, l, t, k, i;
};

class Measurement{

	public:
		// the value, in base units
		double value;

		// the dimension of the measurement
		Dimension dim;

		Measurement(const double &value, const std::string &units);
};

/*
#define DEFINE_OUTPUT_METHOD(MM,LL,TT,KK,II,UNITS) \
		inline \
		std::ostream& operator <<(std::ostream &os,const Units<MM,LL,TT,KK,II> &u){ \
			double d = *reinterpret_cast<const double*>(&u); \
			os << d; \
			os.flags() & std::ios_base::showbase && os << " " << UNITS; \
			return os; \
		}
*/

#endif
