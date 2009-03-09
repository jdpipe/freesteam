#ifndef UNITS_H
#define UNITS_H

#include <iostream>
#include <sstream>
#include <cmath>

#define CHECK_UNITS

// All of the reinterpret casts are work-arounds to let us make
// val & Units(double) private without using friends


/// Unit Checking/tracking Class
/**
	Purpose: Template class to create a 'Units' data type.
		- It checks that units are correct at *COMPILE* time.
		- It hides conversion constants.
		- It enforces self documenting code.

	Original Author:  Christopher Rettig ( rettigcd@bigfoot.com )
	\n (This is not the original version)

	Dev platform:
		. Win2k -- gcc version 3.3.1 (cygming special)

	Tested on:
		. NT4.0 -- VC++ 6.0
		. Win2k -- gcc version 3.2.3 (mingw special 20030504-1)
		. Linux -- gcc version 3.2.2 20030222 (Red Hat Linux 3.2.2-5)

	@todo
		Get the units stuff working with a specific units-tracking flag. Should be possible to say that
		all pressures get output in MPa, all temperatures in �C, etc etc.

	@see
		http://www.embedded.com/shared/printableArticle.jhtml?articleID=9900094 \n
		http://www.embedded.com/code/2001code.htm \n
		http://www.experts-exchange.com/Programming/Programming_Languages/Cplusplus/Q_21215508.html \n
		http://www.experts-exchange.com/Programming/Programming_Languages/Cplusplus/Q_21199495.html
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
		inline operator double() const; // Deliberately not defined, will cause linker errors if called.

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
	*reinterpret_cast<double*>(&r) = (*reinterpret_cast<const double*>(&u))*(*reinterpret_cast<const double*>(&u));
	return r;
}

// Cubing

template < int M, int L, int T, int K, int I >
inline Units<3*M, 3*L, 3*T, 3*K, 3*I>
cube(const Units < M, L, T, K, I > u) {
	Units<3*M, 3*L, 3*T, 3*K, 3*I> r;
	double n = (*reinterpret_cast<const double*>(&u));
	*reinterpret_cast<double*>(&r) = n*n*n;
	return r;
}

// Square root (integer powers of units only)
template < int M, int L, int T, int K, int I >
inline Units<M, L, T, K, I>
sqrt(const Units < 2*M, 2*L, 2*T, 2*K, 2*I > u) {
	Units<M, L, T, K, I> r;
	double n = (*reinterpret_cast<const double*>(&u));
	*reinterpret_cast<double*>(&r) = sqrt(n);
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
DEFINE_OUTPUT_METHOD(-1,  3,  0,  0,  0, "m�/kg");
DEFINE_OUTPUT_METHOD( 0,  2, -2, -1,  0, "J/kgK");
DEFINE_OUTPUT_METHOD( 0,  2, -2,  0,  0, "J/kg");
DEFINE_OUTPUT_METHOD( 1, -3,  0,  0,  0, "kg/m�");

//--------------------------------
// BASE MEASURES

typedef Units < 1 > Mass;
typedef Units < 0,  1 > Length;
typedef Units < 0,  0,  1 > Time;
typedef Units < 0,  0,  0,  1 > Temperature;
typedef Units < 0,  0,  0,  0,  1 > Current;

//--------------------------------
// DERIVED MEASURES

typedef Units < 0,  2 > Area;
typedef Units < 0,  3 > Volume;

typedef Units < 1, -3 > Density;
typedef Units <-1,  3 > SpecificVolume;

typedef Units < 1, -3, -1> DensityPerTime;

typedef Units < 0,  0, -1> Frequency;

typedef Units < 1,  1, -2 > Force;
typedef Units < 1, -1, -2 > Pressure;
typedef Units < 0,  1, -1 > Velocity;
typedef Units < 0,  1, -2 > Acceleration;
typedef Units < 1,  2, -2 > Torque;
typedef Units < 1,  2, -2 > Energy;
typedef Units < 1,  2, -3 > Power;
typedef Units < 0,  2, -2 > SpecificEnergy;

typedef Units < 1, -1, -1 > DynamicViscosity;

typedef Units < 1,  1, -3 > PowerPerLength;
typedef Units < 1, -2, -2 > PressurePerLength;
typedef Units < 1,  0, -2 > ForcePerLength;

typedef Units < 1, -1, -3 > DensitySpecificEnergyPerTime;

// Thermodynamics

typedef Units < 1,  2, -2, -1 > Entropy;
typedef Units < 0,  2, -2, -1 > SpecificEntropy;
typedef Units < 1,  1, -3, -1 > Conductivity;
typedef Units < 1,  0, -3, -1 > HeatTransferCoefficient;
typedef Units <-1,  0,  3,  1 > ThermalResistance;

typedef Units < 1,  1, -2, -1 > HeatCapacityPerLength;
typedef Units < 1,  2, -3, -1 > PowerPerTemperature;

typedef Units < 0,  0,  0, -1 > ThermalExpansionCoefficient;

typedef Units < 0,  3, -1 > VolFlowRate;
typedef Units < 1,  0, -1 > MassFlowRate;
typedef Units < 1, -1, -1 > MassFlowRatePerLength;
typedef Units < 1,  0, -2 > MassFlowRatePerTime;

typedef Units < 1,  0, -3 > HeatFlux;
typedef Units < 1, -2, -1 > MassFlux;

// Electrical

typedef Units < 0,  0,  1,  0,  1 > Charge;
typedef Units < 1,  2, -3,  0, -1 > ElecPotential;
typedef Units < 1,  2, -4,  0, -2 > Capacitance;
typedef Units < 1,  2, -3,  0, -2 > Resistance;
typedef Units <-1, -2,  3,  0,  2 > Conductance;

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

typedef Power HeatRate;
typedef PowerPerLength HeatRatePerLength;
typedef PowerPerTemperature HeatRatePerTemperature;

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

const Frequency Hertz = 1.0 / second;

const Force Newton = kilogram * metre / (second * second);

const Pressure Pascal = Newton / (metre * metre);
const Pressure bar = 100.0 * kilo * Pascal;
const Pressure MPa = Mega * Pascal;
const Pressure kPa = kilo * Pascal;
const Energy Joule = Newton * metre;
const Energy kJ = kilo * Joule;

const Power Watt = Joule / second;

const HeatFlux W_m2 = Watt / metre2;

const double Percent = 1.0 / 100;

//------------------------------------
// COMMON MEASURES (NON-SI)

const Length mile = 1.609 * kilometre;
const Length foot = 304.8 * milli * metre;
const Length inch = 25.4 * milli * metre;

const Temperature Rankin = 0.556 * Kelvin;

const Frequency RPM = 1.0 / minute;

//------------------------------------
// THERMODYNAMIC MEASURES

const SpecificEnergy kJ_kg = kilo * Joule / kilogram;
const SpecificEnergy J_kg = Joule / kilogram;

const SpecificEntropy kJ_kgK = kilo * Joule / kilogram / Kelvin;
const SpecificEntropy J_kgK = Joule / kilogram / Kelvin;

const HeatTransferCoefficient W_m2K = Watt / metre2 / Kelvin;
const Conductivity W_mK = Watt / metre / Kelvin;
const Conductivity mW_mK = milli * W_mK;
const Density kg_m3 = kilogram / metre3;
const SpecificVolume m3_kg = metre3 / kilogram;

const MassFlowRate kg_s = kilogram / second;
const VolFlowRate m3_s = metre3 / second;

const HeatCapacityPerLength J_mK = Joule / metre / Kelvin;

//------------------------------------
// ELECTRICAL STUFF

const ElecPotential volt = Watt / ampere;
const Charge Coulomb = ampere * second;
const Capacitance Farad = volt / Coulomb;
const Resistance Ohm = volt / ampere;

//------------------------------------
// HANDLING TEMPERATURES

const Temperature ZeroCelsius = 273.15 * Kelvin;
const Temperature ZeroFahrenheit = ZeroCelsius - 32.0 * Rankin;

/**
	Convert a temperature (in Kelvin) to Celsius.
	@return the temperature, as a plain 'double' type
*/
inline double
tocelsius(const Temperature& T){
	double d = *reinterpret_cast<const double*>(&T);
	return d - 273.15;
}

/**
	Convert a Celsius temperature to Kelvin
	@param T_C double value for the temperature in degrees
	@return Temperature object (Kelvin)
*/
inline Temperature
fromcelsius(const double &T_C){
	return (T_C * Kelvin) + ZeroCelsius;
}

/**
	Convert from Fahrenheit temperature to Temperature object (Kelvin)
*/
inline double
tofahrenheit(const Temperature &T){
	return (T - ZeroFahrenheit) / Rankin;
}

/// Convert Temperature object to Fahrenheit
/**
	@return temperature in Fahrenheit (as type 'double')
*/
inline Temperature
fromfahrenheit(const double &T_F){
	return (T_F * Rankin) + ZeroFahrenheit;
}

#endif				// UNITS_H