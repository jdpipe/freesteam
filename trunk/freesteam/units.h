/*

freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2005  John Pye

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

For this file,
	Original Author:  Christopher Rettig ( rettigcd@bigfoot.com )

*/

#ifndef UNITS_H

#define UNITS_H

#include "config.h"
#include "isinfnan.h"

#include <iostream>
#include <sstream>
#include <cmath>

#ifdef UNITS_CAST_THROW
# include <stdexcept>
#endif

#define UNITS_ALLOW_CAST_TO_DOUBLE

#define UNITS_ALLOW_GET_RAW_VALUE

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
	    - (This is not the original version)

	Dev platform:
		. Win2k -- gcc version 3.3.1 (cygming special)

	Tested on:
		. NT4.0 -- VC++ 6.0
		. Win2k -- gcc version 3.2.3 (mingw special 20030504-1)
		. Linux -- gcc version 3.2.2 20030222 (Red Hat Linux 3.2.2-5)

	@todo
		Get the units stuff working with a specific units-tracking flag. Should
		be possible to say that all pressures should be output in MPa, all
		temperatures be output in °C, etc etc.

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

		// Scalar mulitplication & division
		Units operator*( const double d ) const { Units u; u.d_val=d_val*d; return u; }
		Units operator/( const double d ) const { Units u; u.d_val=d_val/d; return u; }

		// Integer mult and div
		//Units operator*( const int i ) const { Units u; u.d_val=d_val*i; return u; }
		//Units operator/( const int i ) const { Units u; u.d_val=d_val/i; return u; }

		const Units& operator*=( const double d ) { d_val*=d; return *this; }
		const Units& operator/=( const double d ) { d_val/=d; return *this; }

		// Unit additions & subtraction
		Units operator+( const Units& u ) const { Units v; v.d_val=d_val+u.d_val; return v; }
		Units operator-( const Units& u ) const { Units v; v.d_val=d_val-u.d_val; return v; }
		Units& operator+=( const Units& u ) { d_val+=u.d_val; return *this; }
		Units& operator-=( const Units& u ) { d_val-=u.d_val; return *this; }
		Units operator-() const { Units u; u.d_val=-d_val; return u; }

		// Comparisons
		bool operator==( const Units& u ) const { return d_val==u.d_val; } ///<Equality
		bool operator!=( const Units& u ) const { return d_val!=u.d_val; } ///<Inequality
		bool operator< ( const Units& u ) const { return d_val< u.d_val; } ///<Less-than
		bool operator<=( const Units& u ) const { return d_val<=u.d_val; } ///<Less-or-equal
		bool operator> ( const Units& u ) const { return d_val> u.d_val; } ///<Greater-than
		bool operator>=( const Units& u ) const { return d_val>=u.d_val; } ///<Greater-or-equal

		// Scalar typecast
#ifndef UNITS_CAST_THROW
# ifdef UNITS_ALLOW_CAST_TO_DOUBLE
		inline operator double() const; // Deliberately not defined, will cause linker errors if called.
# endif
#else
		/**
			This will be used by Python wrappers that need to access
			values of unitful variables
		*/
		inline operator double() const {
			throw new std::runtime_error("Invalid cast to double!");
		}
#endif

#ifdef UNITS_ALLOW_GET_RAW_VALUE
		inline double getRawValue() const{
			return d_val;
		}
#endif

		/// Division
		/**
			SWIG can't handle the following, so this lot can't be wrapped easily, alas :-(
		*/
		template< int m, int l, int t, int k, int i >
		Units<M-m,L-l,T-t,K-k,I-i>
		operator/( const Units<m,l,t,k,i>& u2 ) const {
			// This kills Borland C Builder 5.5. Don't think there's a workaround.
			Units<(M-m),(L-l),(T-t),(K-k),(I-i)> r;
			*reinterpret_cast<double*>(&r) = d_val/ *reinterpret_cast<const double*>(&u2);
			return r;
		}

		/// Integer Powers
		/**
			Usage should be something like...
			@code
			   Temperature T = 300. * Kelvin;
			   HeatFlux Q = SIGMA_C * ( T.pow<4>() - (300.*Kelvin).pow<4>())
			@endcode
		*/
		template<int n>
		Units<M*n,L*n,T*n,K*n,I*n>
		ipow() const{
			Units<M*n,L*n,T*n,K*n,I*n> r;
			*reinterpret_cast<double*>(&r) = pow(double(d_val),n);
			return r;
		}

		/// Square root (not such nice syntax)
		/**
			Usage like this:
			@code
				Area ha = (100. * metre)*(100. * metre);
				Length side = Length::sqrt(ha);
			@endcode
		*/
		static inline Units
		sqrt(const Units <2*M,2*L,2*T,2*K,2*I> u) {
			Units r;
			double n = (*reinterpret_cast<const double*>(&u));
			*reinterpret_cast<double*>(&r) = std::sqrt(n);
			return r;
		}

	private:

		//bool hello;
		double d_val;

		// used by */+- to make returning values easy
		//Units( double val ):d_val(val){}

};


/// Cast to double
/**
	Only defined for dimensionless values!
*/
#ifdef UNITS_ALLOW_CAST_TO_DOUBLE
template<>
inline Units<0,0,0,0,0>::operator double() const {
	return d_val;
}
#endif

/*
#ifdef UNITS_CAST_THROW
#include <stdexcept>
template< int m, int l, int t, int k, int i >
inline Units<m,l,t,k,i>::operator double() const {
	throw new std::runtime_error("Invalid cast to double!");
}
#endif
*/

/// Scalar multiplication
template< int m, int l, int t, int k, int i >
inline
Units<m,l,t,k,i>
operator*( const double d, const Units<m,l,t,k,i> &u) {
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

/// Division
template< int m, int l, int t, int k,int i >
inline
Units<-m,-l,-t,-k,-i>
operator/( const double d, const Units<m,l,t,k,i>& u) {
	// This kills Borland C Builder 5.5. Don't think there's a workaround.
	Units< -m, -l, -t, -k, -i > r;
	*reinterpret_cast<double*>(&r) = d / *reinterpret_cast<const double*>(&u);
	return r;
}

/// Multiplication
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

// Absolute value
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

/// Square of a value
template < int M, int L, int T, int K, int I >
inline Units<2*M, 2*L, 2*T, 2*K, 2*I>
sq(const Units < M, L, T, K, I > u) {
	return u*u;
}

/// Cube of a value
template < int M, int L, int T, int K, int I >
inline Units<3*M, 3*L, 3*T, 3*K, 3*I>
cube(const Units < M, L, T, K, I > u) {
	return u * u * u;
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

// Min/Max

/* Macros for MAX, MIN and ABS... */
#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
# define MAX(X,Y) \
	( { __typeof__ (X) x_ = (X); \
    	__typeof__ (Y) y_ = (Y); \
    	(x_ > y_) ? x_ : y_; \
	} )
# define MIN(X,Y) \
	( { __typeof__ (X) x_ = (X); \
    	__typeof__ (Y) y_ = (Y); \
    	(x_ < y_) ? x_ : y_; \
	} )
# define ABS(X) \
	( { __typeof__ (X) x_ = (X); \
    	(x_ > 0) ? x_ : -x_; \
	} )
#else
# define MAX(a,b) ( (a) < (b) ? (b) : (a) )
# define MIN(a,b) ( (a) < (b) ? (a) : (b) )
# define ABS(x) ( ((x) > 0) ? (x) : -(x) )
#endif


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

DEFINE_OUTPUT_METHOD( 1,  0,  0,  0,  0, "kg");
DEFINE_OUTPUT_METHOD( 0,  1,  0,  0,  0, "m");
DEFINE_OUTPUT_METHOD( 0,  2,  0,  0,  0, "m²");
DEFINE_OUTPUT_METHOD( 0,  3,  0,  0,  0, "m³");
DEFINE_OUTPUT_METHOD( 1, -1,  0,  0,  0, "kg/m");
DEFINE_OUTPUT_METHOD( 1, -2,  0,  0,  0, "kg/m²");
DEFINE_OUTPUT_METHOD( 1, -3,  0,  0,  0, "kg/m³");
DEFINE_OUTPUT_METHOD(-1,  3,  0,  0,  0, "m³/kg");
DEFINE_OUTPUT_METHOD( 0,  0,  1,  0,  0, "s");
DEFINE_OUTPUT_METHOD( 0,  0, -1,  0,  0, "Hz");
DEFINE_OUTPUT_METHOD( 1,  0, -1,  0,  0, "kg/s");
DEFINE_OUTPUT_METHOD( 1,  0, -3,  0,  0, "W/m²");
DEFINE_OUTPUT_METHOD( 0,  1, -1,  0,  0, "m/s");
DEFINE_OUTPUT_METHOD( 0,  2, -2,  0,  0, "J/kg");
DEFINE_OUTPUT_METHOD( 1, -1, -2,  0,  0, "Pa");
DEFINE_OUTPUT_METHOD( 1, -1, -1,  0,  0, "Pa·s");
DEFINE_OUTPUT_METHOD( 1,  2, -3,  0,  0, "W");
DEFINE_OUTPUT_METHOD( 1, -2, -1,  0,  0, "kg/s/m²");
DEFINE_OUTPUT_METHOD( 0,  0,  0,  1,  0, "K");
DEFINE_OUTPUT_METHOD( 0,  2, -2, -1,  0, "J/kgK");

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
typedef Units < 0,  2, -1 > KinematicViscosity;
typedef Units < 1,  1, -3 > PowerPerLength;
typedef Units < 1, -2, -2 > PressurePerLength;
typedef Units < 1,  0, -2 > ForcePerLength;
typedef Units < 0,  2, -3 > PowerPerMass;

typedef Units < 1, -1, -3 > DensitySpecificEnergyPerTime;

typedef Units < 0,  3, -1 > VolFlowRate;
typedef Units < 1,  0, -1 > MassFlowRate;
typedef Units < 1, -1, -1 > MassFlowRatePerLength;
typedef Units < 1,  0, -2 > MassFlowRatePerTime;

typedef Units < 1,  0, -3 > HeatFlux;
typedef Units < 1, -2, -1 > MassFlux;

// Thermodynamics

typedef Units < 1,  2, -2, -1 > Entropy;
typedef Units < 0,  2, -2, -1 > SpecificEntropy;
typedef Units < 1,  1, -3, -1 > ThermalConductivity;
typedef Units < 1,  0, -3, -1 > HeatTransferCoefficient;
typedef Units <-1,  0,  3,  1 > ThermalResistance;

typedef Units < 1,  1, -2, -1 > HeatCapacityPerLength;
typedef Units < 1,  2, -3, -1 > PowerPerTemperature;

typedef Units < 0,  0,  0, -1 > ThermalExpansionCoefficient;

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

const Frequency RPM = 1. / minute;

//------------------------------------
// THERMODYNAMIC MEASURES

const SpecificEnergy kJ_kg = kilo * Joule / kilogram;
const SpecificEnergy J_kg = Joule / kilogram;

const SpecificEntropy kJ_kgK = kilo * Joule / kilogram / Kelvin;
const SpecificEntropy J_kgK = Joule / kilogram / Kelvin;

const HeatTransferCoefficient W_m2K = Watt / metre2 / Kelvin;
const ThermalConductivity W_mK = Watt / metre / Kelvin;
const ThermalConductivity mW_mK = milli * W_mK;
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

// USEFUL CONSTANTS

/// Stefan-Boltzmann Constant (radiation)
const Units<1,0,-3,-4> SIGMA_C = (5.670e-8) * W_m2 /sq(sq(Kelvin));

#endif				// UNITS_H

