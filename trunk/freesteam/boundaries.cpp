#include "common.h"
#include "boundaries.h"
//#include "steamcalculator_macros.h"
#include "steamcalculatorexception.h"

#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;

const double STM_SATURATION_TOL=5e-7;

//----------------------------------------------------------------
// COEFFICIENT TABLE DATA

/**
	Coefficients for saturation curve
	@see Table 34 of IF-97
*/
const Num REGION4_N[11]
	= { 0, 0.11670521452767E+04, -0.72421316703206E+06, -0.17073846940092E+02, 0.12020824702470E+05, -0.32325550322333E+07, 0.14915108613530E+02, -0.48232657361591E+04, 0.40511340542057E+06, -0.23855557567849E+00, 0.65017534844798E+03 };

/**
	Coefficients for the 'B23 curve' between Region 2 and Region 3, IF-97

	@see http://www.iapws.org/relguide/IF97.pdf#page=6, Table 1
*/
const Num B23_N[6]
	= { 0, 0.34805185628969E+03, -0.11671859879975E+01, 0.10192970039326E-02, 0.57254459862746E+03, 0.13918839778870E+02 };

/**
	Coefficients for getSatDensWater_T
*/
const Num REGION43_B[7]
	= { 0, 1.99274064, 1.09965342, -0.510839303, -1.75493479, -45.5170352, -6.74694450E+05 };

/**
	Coefficients for getSatDensSteam_T
*/
const Num REGION43_C[7]
	= { 0, -2.03150240, -2.68302940, -5.38626492, -17.2991605, -44.7586581, -63.9201063 };



//------------------------------------------------------------------
// SATURATION CURVES (NORMAL IAPWS-IF97 CURVES PSAT(T) and TSAT(P)

/// Get the saturation temperature for a specific pressure.
/**
	This is based on the 'Backward equation' for Region 4
	@see IF-97 p 34
*/
Temperature
Boundaries::getSatTemp_p(Pressure p) {

	throw new Exception("Boundaries::getSatTemp_p: disabled due for numerical consistency on saturation line");

	Num b = beta(p);
	Num d = D(E(b), F(b), G(b));

	double thet = (REGION4_N[10] + d - pow(sq(REGION4_N[10] + d) - 4.0 * (REGION4_N[9] + REGION4_N[10] * d), (Num) 0.5)) / 2;

	return thet * Kelvin;

}

/// Get the saturation pressure for a specific temperature
/**
	This the 'Basic equation' for Region 4,
	@see IF-97 p 33
*/
Pressure
Boundaries::getSatPres_T(Temperature T) {

	//cerr << "getSatPres_T: T = " << K_TO_C(T) << "°C" << endl;

	REQUIRE(T <= TB_LOW);
	REQUIRE(T >= T_MIN);

	Num u = Boundaries::upsilon(T);
	Num a = A(u);
	Num b = B(u);
	Num c = C(u);

	Num p_ratio= pow(2 * c / (-b + pow(sq(b) - 4.0 * a * c, 0.5)), 4);

	//cerr << "getSatPres_T: psat = " << setprecision(15) << psat << endl;
	//ENSURE(psat <= P_CRIT + STEAM_P_EPS);
	//ENSURE(psat >= REG4_P_MIN);

	//cerr << "... gives p = " << psat << " MPa" << endl;

	return p_ratio * (1.0 * MPa);
}

//------------------------------------------------------------------
// SATURATION CURVES (SUPPLEMENTARY RHO(T) CURVES)

/**
	@todo
		Limit getSatDensWater_T from being called with T <= TB_LOW
*/
Density
Boundaries::getSatDensWater_T(const Temperature &T){

	REQUIRE(T >= T_TRIPLE);
	//REQUIRE(T > TB_LOW);
	REQUIRE(T <= T_CRIT);

	Num tau = 1 - T / T_CRIT;

	Num tau_1_3 = pow(tau,1.0/3);

	Num tau_2_3 = sq(tau_1_3);
	Num tau_5_3 = tau * tau_2_3;
	Num tau_16_3 = sq(tau_5_3) * tau_5_3 * tau_1_3;
	Num tau_43_3 = sq(tau_16_3) * sq(tau_5_3) * tau_1_3;
	Num tau_110_3 = sq(tau_43_3) * tau_16_3 * tau_5_3 * tau;

	Num delta = 1
		+ REGION43_B[1]*tau_1_3
		+ REGION43_B[2]*tau_2_3
		+ REGION43_B[3]*tau_5_3
		+ REGION43_B[4]*tau_16_3
		+ REGION43_B[5]*tau_43_3
		+ REGION43_B[6]*tau_110_3;

	ENSURE(!isnan(delta));

	return delta * RHO_CRIT;
}

/**
	@todo
		Limit getSatDensSteam_T from being called with T <= TB_LOW
*/
Density
Boundaries::getSatDensSteam_T(const Temperature &T){

	REQUIRE(T >= T_TRIPLE);
	//REQUIRE(T > TB_LOW);
	if(T > T_CRIT){
		throw new Exception("Boundaries::getSatDensSteam_T: T > T_CRIT, not allowed");
	}

	Num tau = 1 - T / T_CRIT;

	Num tau_1_6 = pow(tau,1.0/6);

	Num tau_2_6 = sq(tau_1_6);
	Num tau_4_6 = sq(tau_2_6);
	Num tau_8_6 = sq(tau_4_6);
	Num tau_16_6 = sq(tau_8_6);
	Num tau_18_6 = tau_16_6 * tau_2_6;
	Num tau_37_6 = sq(tau_18_6) * tau_1_6;
	Num tau_71_6 = tau_37_6 * tau_18_6 * tau_16_6;

	Num ln_delta =
		  REGION43_C[1]*tau_2_6
		+ REGION43_C[2]*tau_4_6
		+ REGION43_C[3]*tau_8_6
		+ REGION43_C[4]*tau_18_6
		+ REGION43_C[5]*tau_37_6
		+ REGION43_C[6]*tau_71_6;

	return exp(ln_delta) * RHO_CRIT;
}

//--------------------------------------------------------------------------
// REGION BOUNDARY B23

Pressure
Boundaries::getpbound_T(Temperature T, bool throw_me) {

	if (T < T_REG1_REG3) {
		if (throw_me) {
			throw new SteamCalculatorException(-1.0 * MPa, T, REG2_RANGE_TB_LOW);
		}
	} else if (T > TB_HIGH) {
		if (throw_me) {
			throw new SteamCalculatorException(-1.0 * MPa, T, REG2_RANGE_TB_HIGH);
		}
	}

	Num thet = T / (1.0 * Kelvin);	// T in Kelvin

	Num pi = B23_N[1] + B23_N[2] * thet + B23_N[3] * sq(thet);

	//SHOW_VALUE(pi);

	return pi * (1.0 * MPa);

}

Temperature
Boundaries::getTbound_p(Pressure p, bool throw_me) {

	if (p < PB_LOW) {
		if (throw_me) {
			throw new SteamCalculatorException(p, -1.0 * Kelvin, REG2_RANGE_PB_LOW);
		}
	} else if (p > REG2_P_HIGH) {
		if (throw_me) {
			throw new SteamCalculatorException(p, -1.0 * Kelvin, REG2_RANGE_PB_HIGH);
		}
	}

	Num pi = p / (1.0 * MPa);

	double thet = ( B23_N[4] + pow((pi - B23_N[5]) / B23_N[3], (Num) 0.5) );

	return thet * Kelvin;
}

//--------------------------------------------------------------
// HIGH-LEVEL TESTS : SATURATION AND VALIDITY OF P,T IN IAPWS-IF97

/// Overall pressure/temperature validity check
/**
	Check that the pressure and temperature are within limits, and optionally throw an error if they are out of bounds.
*/
bool
Boundaries::isValid_pT(Pressure p, Temperature T,
                                       bool throw_me) {
	//fprintf(stderr,"Boundaries::isValid_pT - testing point validity p=%.5f,T=%.5f\n",p,T);

	bool isvalid = true;

	if (p < P_MIN) {
		if (throw_me)
			throw new SteamCalculatorException(p, T, STM_PRESSURE_LOW);
		isvalid = false;
	} else if (p > P_MAX + STEAM_P_EPS) {
		if (throw_me)
			throw new SteamCalculatorException(p, T, STM_PRESSURE_HIGH);
		isvalid = false;
	} else if (T > T_MAX + STEAM_T_EPS) {
		if (throw_me)
			throw new SteamCalculatorException(p, T, STM_TEMPERATURE_HIGH);
		isvalid = false;
	} else if (T < T_MIN - STEAM_T_EPS) {
		if (throw_me)
			throw new SteamCalculatorException(p, T, STM_TEMPERATURE_LOW);
		isvalid = false;
	}

	return isvalid;
}

bool
Boundaries::isSat_Tx(const Temperature &T, const Num &x, const bool throw_me){

	if(isnan(T)){
		if(throw_me){
			stringstream s;s << "Boundaries::isSat_Tx: T is not a number: T = " << T << ", x = " << x;
			throw new Exception(s.str());
		}
		return false;
	}
	if(isnan(x)){
		if(throw_me){
			stringstream s;s << "Boundaries::isSat_Tx: x is not a number: T = " << T << ", x = " << x;
			throw new Exception(s.str());
		}
		return false;
	}

	if(T < T_TRIPLE){
		if(throw_me){
			stringstream s;s << "Boundaries::isSat_Tx: T < T_TRIPLE: T = " << T << ", x = " << x;
			throw new Exception(s.str());
		}
		return false;
	}else if(T > T_CRIT){
		if(throw_me){
			stringstream s;s << "Boundaries::isSat_Tx: T > T_CRIT: T = " << T << ", x = " << x;
			throw new Exception(s.str());
		}
		return false;
	}else if(x < 0.0){
		if(throw_me){
			stringstream s;s << "Boundaries::isSat_Tx: x < 0.0: T = " << T << ", x = " << x;
			throw new Exception(s.str());
		}
		return false;
	}else if(x > 1.0){
		if(throw_me){
			stringstream s;s << "Boundaries::isSat_Tx: x > 1.0: T = " << T << ", x = " << x;
			throw new Exception(s.str());
		}
		return false;
	}

	return true;
}

/// Check for a pressure/temperature being in the saturated zone (within some tolerance)
/**
	Check that the temperature and pressure given indicate saturated conditions.

	Note that the STM_SATURATION_TOL is used to allow a small region of error on this test.

	@param p pressure / [MPa]
	@param T temperature / [K]
*/
/*bool
Boundaries::isSat_pT(Pressure p, Temperature T,
                                     bool throw_me) {
	Pressure psat;

	if (!isValid_pT(p, T, true)) {
		return false;
	}

	if (T > T_CRIT) {
		if (throw_me) {
			throw new SteamCalculatorException(p, T, STM_BEYOND_SAT_CURVE);
		}
		return false;
	}

	// try the forwards equation

	if(T < TB_LOW){

	}else{
		Density rho_gas = Boundaries::getSatDensSteam_T(T);
		Pressure psat =
	Pressure psat = Boundaries::getSatPres_T(T);
	if(psat == p){
		return true;
	}

	\*
	// Try the backwards equation:
	Temperature Tsat = Boundaries::getSatTemp_p(p);
	if (Tsat == T) {

		// MESSAGE("EXACTLY SATURATED");

		return true;
	}
	*\

	// Allow almost-saturated -- this tolerates use of the backward equation

	\*
	if( fabs(T-Tsat)/Tsat < STM_SATURATION_TOL) {
		if (throw_me)
			throw new SteamAlmostSaturatedException(p, T);

		// MESSAGE("APPROXIMATELY SATURATED");

		return true;
	}
	*\

	if (throw_me) {
		stringstream s;
		s << "Steam not saturated @ T = " << T << ": p=" << p/bar<< "bar != psat=" << psat/bar<< "bar (rel err " << ((psat-p) / psat) << ")";
		throw new Exception(s.str());
	}

	//cerr << "Point is not saturated" << endl;
	return false;

}
*/
//--------------------------------------------------------------------------
// REGION MEMBERSHIP TESTS

/// Test for a pressure/temperature being in Region 1
/**
	@param p pressure / [MPa]
	@param T temperature / [K]]
	@param throw_me Throw an error if point is not in Region 1
*/
bool
Boundaries::isRegion1_pT(Pressure p, Temperature T,
        bool throw_me) {

	//MESSAGE("  IS POINT IN REGION 1?");

	if (!isValid_pT(p, T, throw_me)) {
		return false;
	}

	if (T > T_REG1_REG3) {

		//MESSAGE("  NO, TEMPERATURE > T_REG1_REG3");

		if (throw_me)
			throw new SteamCalculatorException(p, T, REG1_RANGE_T_HIGH);
		return false;

	}else{
		// Use the forwards equation by choice!

		Pressure p_sat = getSatPres_T(T);
		if(p < p_sat){
		//Temperature T_sat=getSatTemp_p(p); /* getSatTemp_p(p) + STM_SATURATION_TOL * T */
		//if (T > T_sat) {
			//cerr << endl << "p = " << p << ", p_sat = " << p_sat << endl;

			//MESSAGE("  NO, PRESSURE BELOW SATURATION CURVE");

			if (throw_me) {
				cerr << "  Pressure is too high, by " << (p - p_sat) << endl;
				throw new SteamCalculatorException(p, T, REG1_RANGE_P_LOW);
			}
			return false;
		}
	}

	//MESSAGE("  YES.");
	return true;
}

/// Test for a pressure/temperature being in Region 2
/**
	@param p pressure / [MPa]
	@param T temperature / [K]]
	@param throw_me Throw an error if point is not in Region 2
*/
bool
Boundaries::isRegion2_pT(Pressure p, Temperature T,
        bool throw_me) {

	if (!isValid_pT(p, T, throw_me)) {
		return false;
	}
	// check region 2 under region 1/4
	if (T <= T_REG1_REG3) {
		if (p > getSatPres_T(T)/* T < getSatTemp_p(p) - STM_SATURATION_TOL * T*/) {
			//MESSAGE("  p > PSAT");
			if (throw_me)
				throw new SteamCalculatorException(p,T,REG2_RANGE_T_LOW_TS);
			return false;
		}
		// check region 2 under region 3
	} else if (T <= TB_HIGH) {
		if (p > getpbound_T(T)) {
			//MESSAGE("  NOT REG 2: p > PBOUND");
			if (throw_me)
				throw new SteamCalculatorException(p,T,REG2_RANGE_P_HIGH_PB);
			return false;
		}
	}

	return true;
}

/// Test for a pressure/temperutare being in Region 3
/**
	@param p pressure / [MPa]
	@param T temperature / [K]]
	@param throw_me Throw an error if point is not in Region 3
*/
bool
Boundaries::isRegion3_pT(Pressure p, Temperature T,
        bool throw_me) {

	// if scanning to see what region you're in, scan region 3 last 'cause it's less slick (just a little)

	if (!isValid_pT(p, T, throw_me)) {
		return false;
	}

	if (T < T_REG1_REG3) {
		if (throw_me)
			throw new SteamCalculatorException(p, T, REG3_RANGE_T_LOW_TB);
		return false;

	}else if(getpbound_T(T, throw_me) > p) {
		if (throw_me)
			throw new SteamCalculatorException(p, T, REG3_RANGE_P_LOW_PB);
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------
// PRIVATE FUNCTIONS

//-----------------
// For Tsat(p)...

/**

	@see http://www.iapws.org/relguide/IF97.pdf#page=33, Eqn 29a
*/
Num
Boundaries::beta(Pressure p) {
	return pow(p / (1.0 * MPa), 0.25);
}

/**
	@see http://www.iapws.org/relguide/IF97.pdf#page=34, Eqn 31
*/
Num
Boundaries::E(Num bet) {
	return sq(bet) + REGION4_N[3] * bet + REGION4_N[6];
}

/**
	@see http://www.iapws.org/relguide/IF97.pdf#page=34, Eqn 31
*/
Num
Boundaries::F(Num bet) {
	return REGION4_N[1] * sq(bet) + REGION4_N[4] * bet + REGION4_N[7];
}

/**
	@see http://www.iapws.org/relguide/IF97.pdf#page=34, Eqn 31
*/
Num
Boundaries::G(Num bet) {
	return REGION4_N[2] * sq(bet) + REGION4_N[5] * bet + REGION4_N[8];
}

/**
	@see http://www.iapws.org/relguide/IF97.pdf#page=34, Eqn 31
*/
Num
Boundaries::D(Num E, Num F, Num G) {
	return 2 * G / (-F - pow(sq(F) - 4 * E * G, (Num) 0.5));
}

//------------------
// For p_sat(T)...

/**
	@see http://www.iapws.org/relguide/IF97.pdf#page=33, Eqn 29b
*/
Num
Boundaries::upsilon(Temperature T) {

	Num T_ratio = T / (1.0 * Kelvin);
	return T_ratio + REGION4_N[9] / (T_ratio - REGION4_N[10]);
}

/**
	@see http://www.iapws.org/relguide/IF97.pdf#page=33, Eqn 30
*/
Num
Boundaries::A(Num ups) {

	return sq(ups) + REGION4_N[1] * ups + REGION4_N[2];
}

/**
	@see http://www.iapws.org/relguide/IF97.pdf#page=33, Eqn 30
*/
Num
Boundaries::B(Num ups) {

	return REGION4_N[3] * sq(ups) + REGION4_N[4] * ups + REGION4_N[5];
}

/**
	@see http://www.iapws.org/relguide/IF97.pdf#page=33, Eqn 30
*/
Num
Boundaries::C(Num ups) {

	return REGION4_N[6] * sq(ups) + REGION4_N[7] * ups + REGION4_N[8];
}

