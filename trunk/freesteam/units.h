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

//#ifdef CHECK_UNITS

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


template< int M ,int L=0 ,int T=0 ,int K=0 ,int I=0 >
class Units {
		
	public:

		Units():d_val(1.0){}	// only used to consturct base units
		Units( const Units& u ):d_val( u.d_val ){}
		const Units& operator=( const Units& u ){	d_val=u.d_val; return *this; }

		// scalar mulitplication & division
		Units operator*( double d ) const { Units u; u.d_val=d_val*d; return u; }
		Units operator/( double d ) const { Units u; u.d_val=d_val/d; return u; }
		const Units& operator*=( double d ) { d_val*=d; return *this; }
		const Units& operator/=( double d ) { d_val/=d; return *this; }

		// unit additions & subtraction
		Units operator+( const Units& u ) const { Units v; v.d_val=d_val+u.d_val; return v; }
		Units operator-( const Units& u ) const { Units v; v.d_val=d_val-u.d_val; return v; }
		Units& operator+=( const Units& u ) { d_val+=u.d_val; return *this; }
		Units& operator-=( const Units& u ) { d_val-=u.d_val; return *this; }
		Units operator-() const { Units u; u.d_val=-d_val; return u; }

		// comparison
		bool operator==( const Units& u ) const { return d_val==u.d_val; }
		bool operator!=( const Units& u ) const { return d_val!=u.d_val; }
		bool operator< ( const Units& u ) const { return d_val< u.d_val; }
		bool operator<=( const Units& u ) const { return d_val<=u.d_val; }
		bool operator> ( const Units& u ) const { return d_val> u.d_val; }
		bool operator>=( const Units& u ) const { return d_val>=u.d_val; }

		// scalar typecast
		operator double() const;// not defined here because only unitless can cast to double

		// Unit Division
		template< int m, int l, int t, int k, int i >
		Units<M-m,L-l,T-t,K-k,I-i>
		operator/( const Units<m,l,t,k,i>& u2 ) const {
			Units<M-m,L-l,T-t,K-k,I-i> r;
			*reinterpret_cast<double*>(&r) = d_val/ *reinterpret_cast<const double*>(&u2);
			return r;
		}

	private:

		//bool hello;
		double d_val;
		
		// used by */+- to make returning values easy
		//Units( double val ):d_val(val){}

};


// only defined for unitless types
inline Units<0,0,0,0,0>::operator double() const {
	return d_val;
}


// Scalar Multiplication & Division
template< int m, int l, int t, int k, int i >
inline
Units<m,l,t,k,i> 
operator*( double d, const Units<m,l,t,k,i> &u) {
	return u*d;
}


/*
// Scalar Multiplication & Division
template< int m, int l, int t, int k, int i >
inline
Units<m,l,t,k,i> 
operator*(const Units<m,l,t,k,i> &u,  double d) {
	return u*d;
}
*/

template< int m, int l, int t, int k,int i >
inline
Units<-m,-l,-t,-k,-i> 
operator/( double d, const Units<m,l,t,k,i>& u) {
	Units< -m, -l, -t, -k, -i > r;
	*reinterpret_cast<double*>(&r) = d / *reinterpret_cast<const double*>(&u);
	return r;
}


// Unit Multiplication

template<int M, int L, int T, int K, int I,  int m, int l, int t, int k, int i >
Units<M+m,L+l,T+t,K+k,I+i >
operator*(const Units<M,L,T,K,I>& u1, const Units<m,l,t,k,i>& u2 ){
	Units<M+m,L+l,T+t,K+k,I+i> r;
	*reinterpret_cast<double*>(&r) = *reinterpret_cast<const double*>(&u1) * *reinterpret_cast<const double*>(&u2);
	return r;
}

/*
/// Equality with some tolerance
template < int M, int L, int T, int K, int I >
inline
bool
eq(const Units<M,L,T,K,I> u, const Units<M,L,T,K,I> v, double tol = UNITS_DEFAULT_TOL) {
	double d = fabs(*reinterpret_cast<const double*>(&u) - *reinterpret_cast<const double*>(&v));
	if (d > tol) return false;
	return true;
}
*/

/// Equality with some tolerance
template < int M, int L, int T, int K, int I >
inline
bool
eq(const Units<M,L,T,K,I> &u, const Units<M,L,T,K,I> &v, const Units<M,L,T,K,I> &tol) {
	if(fabs(u-v) < tol){
		return true;
	}
	return false;
}

// Absolute Values

template < int M, int L, int T, int K, int I >
inline
Units<M,L,T,K,I>
fabs(const Units<M,L,T,K,I> u) {
	if (*reinterpret_cast<const double*>(&u) < 0) {
		Units < M, L, T, K, I > r;
		*reinterpret_cast<double*>(&r) = - *reinterpret_cast<const double*>(&u);
		return r;
	} else {
		return u;
	}
}

// Squaring

template < int M, int L, int T, int K, int I >
inline Units<2*M, 2*L, 2*T, 2*K, 2*I>
sq(const Units < M, L, T, K, I > u) {
	Units<2*M, 2*L, 2*T, 2*K, 2*I> r;
	cerr << "sq(U)->";
	*reinterpret_cast<double*>(&r) = sq(*reinterpret_cast<const double*>(&u));
	return r;
}
	
// Not-a-Number test

template < int M, int L, int T, int K, int I >
inline bool isnan(const Units<M,L,T,K,I>& u){
	return isnan(*reinterpret_cast<const double*>(&u));
}

// Infinity test

template < int M, int L, int T, int K, int I >
inline bool isinf(const Units<M,L,T,K,I>& u){
	return isinf(*reinterpret_cast<const double*>(&u));
}

// OUTPUT

template < int m, int l, int t, int k, int i >
inline
std::ostream & operator <<(std::ostream & os, const Units < m,
                           l, t, k, i > &u) {
	double d = *reinterpret_cast<const double*>(&u);
	os << d;
	os.flags() & std::ios_base::showbase && os << " (dim)";
	return os;
}

inline
std::ostream & operator <<(std::ostream & os, const Units < 0,
                           0, 0, 0, 0 > &u) {
	double d = *reinterpret_cast<const double*>(&u);
	os << d;
	return os;
}

#define DEFINE_OUTPUT_METHOD(MM,LL,TT,KK,II,UNITS) \
		inline \
		std::ostream& operator <<(std::ostream &os,const Units<MM,LL,TT,KK,II> &u){ \
			double d = *reinterpret_cast<const double*>(&u); \
			os << d; \
			os.flags() & std::ios_base::showbase && os << " " << UNITS; \
			return os; \
		}

DEFINE_OUTPUT_METHOD( 0,  1,  0,  0,  0, "m");
DEFINE_OUTPUT_METHOD( 0,  0,  1,  0,  0, "s");
DEFINE_OUTPUT_METHOD( 0,  1, -1,  0,  0, "m/s");
DEFINE_OUTPUT_METHOD( 1,  0,  0,  0,  0, "kg");
DEFINE_OUTPUT_METHOD( 0,  0,  0,  1,  0, "K");
DEFINE_OUTPUT_METHOD( 1, -1, -2,  0,  0, "Pa");
DEFINE_OUTPUT_METHOD(-1,  3,  0,  0,  0, "m³/kg");
DEFINE_OUTPUT_METHOD( 0,  2, -2, -1,  0, "J/kgK");
DEFINE_OUTPUT_METHOD( 0,  2, -2,  0,  0, "J/kg");
DEFINE_OUTPUT_METHOD( 1, -3,  0,  0,  0, "kg/m³");

//--------------------------------
// BASE MEASURES
typedef Units < 1,0,0,0,0 > Mass;
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
// Thermodynamics
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


/*#else

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
*/

//----------------------------------------------
// BASE UNITS FOR BASE MEASURES

const Mass kilogram;
const Length metre;
const Time second;
const Temperature Kelvin;
const Current ampere;

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
const Energy kJ = kilo * Joule;

const Power Watt = Joule / second;

const double Percent = 1.0 / 100;

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
	double d = *reinterpret_cast<const double*>(&T);
	return d - 273.15;
}


inline Temperature
fromcelsius(double T_C){
	return (T_C + 273.15) * Kelvin;
}



#endif				// UNITS_H
