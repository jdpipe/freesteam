/// UNIT CHECKING / TRACKING CLASS
/**
	Adapted from code given by Rettig at
	http://www.embedded.com/shared/printableArticle.jhtml?articleID=9900094
	http://www.embedded.com/code/2001code.htm
	
	Original Author:  Christopher Rettig ( rettigcd@bigfoot.com )
	
	Current version by John Pye <john dot pye at student-unsw-edu-au>
*/

#ifndef UNITS_H
#define UNITS_H

#include "exception.h"

#include <iostream>
#include <sstream>
#include <cmath>

#define CHECK_UNITS

#ifdef CHECK_UNITS

// All of the reinterpret casts are work-arounds to let us make
// val & Units(double) private without using friends

/**
	Purpose:
	 Template class to create a 'Units' data type.
	  It checks that units are correct at *COMPILE* time.
	  It hides conversion constants.
	  It enforces self documenting code.

	Dev platform:
		. Win2k -- gcc version 3.3.1 (cygming special)
	
	Tested on:
		. NT4.0 -- VC++ 6.0
		. Win2k -- gcc version 3.2.3 (mingw special 20030504-1)
		. Linux -- gcc version 3.2.2 20030222 (Red Hat Linux 3.2.2-5)
*/
template < int M, int L = 0, int T = 0, int K = 0, int I = 0 > class Units {

	public:

		Units() {
			val = 0;
		}

		/// Initialiser
		Units(double v) {
			setValue(v);
		}

		/// Copy constructor
		Units(const Units & u):val(u.getValue()) {}

		///     Assignment operator
		const Units & operator=(const Units & u) {
			val = u.getValue();
			return *this;
		}

		// MULTIPLICATION AND DIVISION BY SCALARS

		const Units & operator*=(double d) {
			val *= d;
			return *this;
		}
		const Units & operator/=(double d) {
			val /= d;
			return *this;
		}

		// ACCUMULATION AND DIMINUTION

		Units & operator+=(const Units & u) {
			val += u.getValue();
			return *this;
		}
		Units & operator-=(const Units & u) {
			val -= u.getValue();
			return *this;
		}

		/// Cast to scalar
		/**
			Not defined here because only Units<0,0,0,0,0> can cast to double
		*/
		operator double () const;
		
		double getValue() const {
			return val;
		}

		// Value checking
		bool isValid() {
			return !isnan(val) && !isinf(val);
		}

		bool isnan() {
			return isnan(val);
		}

		bool isinf() {
			return isinf(val);
		}
		
		operator bool(){
			return (bool)getValue();
		}

	private:

		double val;
		// used by */+- to make returning values easy
		void setValue(double v) {
			val = v;
		}

};

#ifndef NDEBUG
/// Casting to double
/**
	Debug version - throw an error if it gets through
*/
template < int m, int l, int t, int k, int i >
inline Units<m,l,t,k,i>::operator double() const{
	std::stringstream s;
	throw new Exception(s.str());
}
#endif

/// Casting to double
/**
	Only defined for unitless types
*/
inline Units < 0, 0, 0, 0, 0 >::operator double ()const {
	return val;
}


// MULTIPLICATION

template < int M, int L, int T, int K, int I, int m, int l,
int t, int k, int i > inline Units < M + m, L + l, T + t,
K + k, I + i > operator *(const Units < M, L, T, K, I > u,
                          const Units < m, l, t, k, i > v) {
	return Units < M + m, L + l, T + t, K + k,
	       I + i > (u.getValue() * v.getValue());
}

template < int m, int l, int t, int k, int i >
inline
Units < m, l, t, k, i >
operator *(double u, const Units < m, l, t, k, i > v) {
	return Units < m, l, t, k, i > (u * v.getValue());
}

template < int M, int L, int T, int K, int I >
inline
Units < M, L, T, K, I >
operator *(const Units < M, L, T, K, I > u, double v) {
	return Units < M, L, T, K, I > (u.getValue() * v);
}

template < int m, int l, int t, int k, int i >
inline
Units < m, l, t, k, i >
operator *(int u, const Units < m, l, t, k, i > v) {
	return Units < m, l, t, k, i > (u * v.getValue());
}

template < int M, int L, int T, int K, int I >
inline
Units < M, L, T, K, I >
operator *(const Units < M, L, T, K, I > u, int v) {
	return Units < M, L, T, K, I > (u.getValue() * v);
}

// DIVISION

template < int M, int L, int T, int K, int I, int m, int l,int t, int k, int i >
inline 
Units < M - m, L - l, T - t,K - k, I - i >
operator/(const Units < M, L, T, K, I > u, const Units < m, l, t, k, i > v) {
	
	if(v.getValue()==0){
		stringstream s;
	
		s << endl << __FILE__ << ":" << __LINE__ << ": Invalid division by zero-value having Units<" << m << "," << l << "," << t << "," << k << "," << i <<">!";
		
		throw new Exception(s.str());
	}
	
	return Units < M - m, L - l, T - t, K - k,
	       I - i > (u.getValue() / v.getValue());
}

template < int M, int L, int T, int K, int I >
inline
Units < M, L, T, K, I >
operator/(const Units < M, L, T, K, I > u, double v) {
	return Units < M, L, T, K, I > (u.getValue() / v);
}

template < int m, int l, int t, int k, int i >
inline
Units < -m, -l, -t, -k, -i >
operator/(double u, const Units < m, l, t, k, i > v) {
	return Units < -m, -l, -t, -k, -i > (u / v.getValue());
}

template < int m, int l, int t, int k, int i >
inline
Units < -m, -l, -t, -k, -i >
operator/(int u, const Units < m, l, t, k, i > v) {
	return Units < -m, -l, -t, -k, -i > (u / v.getValue());
}

template < int M, int L, int T, int K, int I >
inline
Units < M, L, T, K, I >
operator/(const Units < M, L, T, K, I > u, int v) {
	return Units < M, L, T, K, I > (u.getValue() / v);
}


// COMPARISON

template < int M, int L, int T, int K, int I >
inline
bool
operator==(const Units < M, L, T, K, I > u, const Units < M, L,
           T, K, I > v) {
	return u.getValue() == v.getValue();
}

template < int M, int L, int T, int K, int I >
inline
bool
operator!=(const Units < M, L, T, K, I > u, const Units < M, L,
           T, K, I > v) {
	return u.getValue() != v.getValue();
}

template < int M, int L, int T, int K, int I >
inline
bool
operator<(const Units < M, L, T, K, I > u, const Units < M, L,
          T, K, I > v) {
	return u.getValue() < v.getValue();
}

template < int M, int L, int T, int K, int I >
inline
bool
operator<=(const Units < M, L, T, K, I > u, const Units < M, L,
           T, K, I > v) {
	return u.getValue() <= v.getValue();
}

template < int M, int L, int T, int K, int I >
inline
bool
operator>(const Units < M, L, T, K, I > u, const Units < M, L,
           T, K, I > v) {
	return u.getValue() > v.getValue();
}


template < int M, int L, int T, int K, int I >
inline
bool
operator>=(const Units < M, L, T, K, I > u, const Units < M, L,
           T, K, I > v) {
	return u.getValue() >= v.getValue();
}

/// Equality with some tolerance
template < int M, int L, int T, int K, int I >
inline
bool
eq(const Units < M, L, T, K, I > u, const Units < M, L, T, K,
   I > v, double tol = UNITS_DEFAULT_TOL) {
	double d = fabs(u.getValue() - v.getValue());
	if (d > tol) return false; return true;
}


// ADDITION AND SUBTRACTION

template < int M, int L, int T, int K, int I >
Units < M, L, T, K, I >
operator+(const Units < M, L, T, K, I > u, const Units < M, L,
          T, K, I > v) {
	return Units < M, L, T, K, I > (u.getValue() + v.getValue());
}

template < int M, int L, int T, int K, int I >
Units < M, L, T, K, I >
operator-(const Units < M, L, T, K, I > u, const Units < M, L,
          T, K, I > v) {
	return Units < M, L, T, K, I > (u.getValue() - v.getValue());
}

template < int M, int L, int T, int K, int I >
Units < M, L, T, K, I >
operator-(const Units < M, L, T, K, I > u) {
	return Units < M, L, T, K, I > (-u.getValue());
}

// Absolute Values

template < int M, int L, int T, int K, int I >
inline Units < M, L, T, K, I >
fabs(const Units < M, L, T, K, I > u) {
	if (u.getValue() < 0) {
		return Units < M, L, T, K, I > (-u.getValue());
	} else {
		return Units < M, L, T, K, I > (u.getValue());
	}
}

// Squaring

template < int M, int L, int T, int K, int I >
inline Units<2*M, 2*L, 2*T, 2*K, 2*I>
sq(const Units < M, L, T, K, I > u) {
	return Units<2*M, 2*L, 2*T, 2*K, 2*I>(sq(u.getValue()));
}
	
// OUTPUT

template < int m, int l, int t, int k, int i >
inline
std::ostream & operator <<(std::ostream & os, const Units < m,
                           l, t, k, i > &u) {
	os << u.getValue();
	os.flags() & std::ios_base::showbase && os << " (dim)";
	return os;
}

inline
std::ostream & operator <<(std::ostream & os, const Units < 0,
                           0, 0, 0, 0 > &u) {
	os << u.getValue(); return os;
}

#define DEFINE_OUTPUT_METHOD(MM,LL,TT,KK,II,UNITS) \
		inline \
		std::ostream& operator <<(std::ostream &os,const Units<MM,LL,TT,KK,II> &u){ \
			os << u.getValue(); \
			os.flags() & std::ios_base::showbase && os << " " << UNITS; \
			return os; \
		}

DEFINE_OUTPUT_METHOD(0, 1, 0, 0, 0, "m");
DEFINE_OUTPUT_METHOD(0, 0, 1, 0, 0, "s");
DEFINE_OUTPUT_METHOD(0, 1, -1, 0, 0, "m/s");
DEFINE_OUTPUT_METHOD(1, 0, 0, 0, 0, "kg");
DEFINE_OUTPUT_METHOD(0, 0, 0, 1, 0, "K");
//--------------------------------
// BASE MEASURES
typedef Units < 1 > Mass;
typedef Units < 0, 1 > Length;
typedef Units < 0, 0, 1 > Time;
typedef Units < 0, 0, 0, 1 > Temperature;
typedef Units < 0, 0, 0, 0, 1 > Current;
//--------------------------------
// DERIVED MEASURES
typedef Units < 0, 2 > Area;
typedef Units < 0, 3 > Volume;
typedef Units < 1, 1, -2 > Force;
typedef Units < 1, -1, -2 > Pressure;
typedef Units < 0, 1, -1 > Velocity;
typedef Units < 0, 1, -2 > Acceleration;
typedef Units < 1, 2, -2 > Torque;
typedef Units < 1, 2, -2 > Energy;
typedef Units < 1, 2, -3 > Power;
typedef Units < 0, 2, -2 > SpecificEnergy;
typedef Units < 1, 2, -2, -1 > Entropy;
typedef Units < 0, 2, -2, -1 > SpecificEntropy;
typedef Units < 1, -3 > Density;
typedef Units < -1, 3 > SpecificVolume;
typedef Units < 1, -1, -1 > DynamicViscosity;
typedef Units < 1, 1, -3, -1, 0 > Conductivity;
typedef Units < 1, 0, -3, -1, 0 > HeatTransferCoefficient;
typedef Units < 0, 3, -1, 0, 0 > Flowrate;
// Electrical
typedef Units < 0, 0, 1, 0, 1 > Charge;
typedef Units < 1, 2, -3, 0, -1 > ElecPotential;
typedef Units < 1, 2, -4, 0, -2 > Capacitance;
typedef Units < 1, 2, -3, 0, -2 > Resistance;
typedef Units < -1, -2, 3, 0, 2 > Conductance;
#else

// Fancy Units template becomes just a scalar
typedef double Units;

//--------------------------------
// BASE MEASURES

typedef Units Mass;
typedef Units Length;
typedef Units Time;
typedef Units Temperature;
typedef Units Current;

//--------------------------------
// DERIVED MEASURES

typedef Units Area;
typedef Units Volume;
typedef Units Force;
typedef Units Pressure;
typedef Units Velocity;
typedef Units Acceleration;
typedef Units Torque;
typedef Units Energy;
typedef Units Power;

typedef Units SpecificEnergy;
typedef Units Entropy;
typedef Units SpecificEntropy;
typedef Units Density;
typedef Units SpecificVolume;
typedef Units DynamicViscosity;

typedef Units Conductivity;
typedef Units HeatTransferCoefficient typedef Units Flowrate;

// Electrical
typedef Units Charge;
typedef Units ElecPotential;
typedef Units Capacitance;
typedef Units Resistance;
typedef Units Conductance;


#endif				// CHECK_UNITS

//----------------------------------------------
// BASE UNITS FOR BASE MEASURES

const Mass kilogram = Mass
                      (1.0);
const Length metre = Length(1.0);
const Time second = Time(1.0);
const Temperature Kelvin = Temperature(1.0);
const Current ampere = Current(1.0);

//------------------------------------
// SOME ALTERNATIVE NAMES

typedef Velocity Speed;
typedef Length Distance;
typedef Energy Heat;
typedef Heat Work;		// nice
typedef Pressure Stress;
typedef HeatTransferCoefficient HTCoeff;
typedef SpecificEntropy SpecificHeatCapacity;	// not the same but the units are the same
typedef SpecificHeatCapacity SpecHeatCap;
typedef SpecificHeatCapacity SpecificGasConstant;
typedef SpecificGasConstant SpecGasConst;
//------------------------------------
// SI MULTIPLIERS

const double Tera = 1e12;
const double Giga = 1e9;
const double Mega = 1e6;
const double kilo = 1e3;
const double hecta = 1e2;
const double Deca = 10;
const double deci = 0.1;
const double centi = 1e-2;
const double milli = 1e-3;
const double micro = 1e-6;

//------------------------------------
// COMMON MEASURES (SI)

const Mass gram = milli * kilogram;
const Mass kg = kilogram;

const Length centimetre = metre / 100.0;
const Length kilometre = 1000.0 * metre;

const Area metre2 = metre * metre;
const Area hectare = (100.0 * metre) * (100.0 * metre);

const Volume metre3 = metre2 * metre;
const Volume litre = milli * metre3;
const Volume centimetre3 =
    (centi * metre) * (centi * metre) * (centi * metre);

const Time minute = 60.0 * second;
const Time hour = 60.0 * minute;
const Time day = 24.0 * hour;

const Force Newton = kilogram * metre / (second * second);

const Pressure Pascal = Newton / (metre * metre);
const Pressure bar = 100.0 * kilo * Pascal;
const Pressure MPa = Mega * Pascal;
const Energy Joule = Newton * metre;

const Power Watt = Joule / second;

//const ElecPotential volt = Watt / ampere;
//const Charge Coulomb = ampere * second;
//const Capacitance Farad = volt / Coulomb;
//const Resistance Ohm = volt / ampere;

//------------------------------------
// COMMON MEASURES (NON-SI)

// ...

//------------------------------------
// THERMODYNAMIC MEASURES

const SpecificEnergy kJ_kg = kilo * Joule / kilogram;
const SpecificEntropy kJ_kgK = kilo * Joule / kilogram / Kelvin;
const HeatTransferCoefficient W_m2K = Watt / metre2 / Kelvin;
const Conductivity W_mK = Watt / metre / Kelvin;
const Conductivity mW_mK = milli * W_mK;
const Density kg_m3 = kilogram / metre3;

//------------------------------------
// HANDLING TEMPERATURE CONVERSIONS

// CELCIUS

inline double
tocelsius(const Temperature& T){
	return(T.getValue() - 273.15);
}


inline Temperature
fromcelsius(double T_C){
	return Temperature(T_C + 273.15);
}



#endif				// UNITS_H
