#include "boundaries.h"
#include "steamcalculator_macros.h"
#include "steamcalculatorexception.h"

#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;

#define STM_SATURATION_TOL 5e-7

//----------------------------------------------------------------
// COEFFICIENT TABLE DATA

/**
	Coefficients for saturation curve
	@see Table 34 of IF-97
*/
const Num REGION4_N[11] = {
                                     0, 0.11670521452767E+04, -0.72421316703206E+06,
                                     -0.17073846940092E+02, 0.12020824702470E+05, -0.32325550322333E+07, 0.14915108613530E+02,
                                     -0.48232657361591E+04, 0.40511340542057E+06, -0.23855557567849E+00, 0.65017534844798E+03
                                 };

/**
	Coefficients for the 'B23 curve' between Region 2 and Region 3
	@see Table 1 of IF-97
*/
const Num REGION2_NB[6] = {
                                     0, 0.34805185628969E+03, -0.11671859879975E+01, 0.10192970039326E-02,
                                     0.57254459862746E+03, 0.13918839778870E+02
                                 };


//--------------------------------------------------------------
// PUBLIC STATIC METHODS

/// Overall pressure/temperature validity check
/**
	Check that the pressure and temperature are within limits, and optionally throw an error if they are out of bounds.
*/
bool SteamRegionBoundaries::isValid_pT(Pressure p, Temperature T,
                                       bool throw_me) {
	//fprintf(stderr,"SteamRegionBoundaries::isValid_pT - testing point validity p=%.5f,T=%.5f\n",p,T);

	bool isvalid = true;

	if (p < P_MIN - STEAM_P_EPS) {
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

/// Check for a pressure/temperature being in the saturated zone (within some tolerance)
/**
	Check that the temperature and pressure given indicate saturated conditions.
	
	Note that the STM_SATURATION_TOL is used to allow a small region of error on this test.
	
	@param p pressure / [MPa]
	@param T temperature / [K]
*/
bool SteamRegionBoundaries::isSat_pT(Pressure p, Temperature T,
                                     bool throw_me) {

	if (!isValid_pT(p, T, true)) {
		return false;
	}

	if (T > T_CRIT) {
		if (throw_me) {
			throw new SteamCalculatorException(p, T, STM_BEYOND_SAT_CURVE);
		}
		return false;
	}

	Num Tsat = SteamRegionBoundaries::getSatTemp_p(p);
	Num dT = T - Tsat;

	if (dT == 0) {
		return true;
	} else {
		if (dT < 0)
			dT = -dT;


		if (dT / Tsat < STM_SATURATION_TOL) {
			if (throw_me)
				throw new SteamAlmostSaturatedException(p, T);
			return true;
		}

		if (throw_me) {
			stringstream s;
			s << "Steam not saturated @ p = " << MPA_TO_BAR(p) << ": T=" <<
			T << " != Tsat=" << Tsat << " (rel err " << (dT /
			        Tsat) << ")";
			throw new Exception(s.str());
		}
		//cerr << "Point is not saturated" << endl;
		return false;
	}


}

/// Get the saturation temperature for a specific pressure.
/**
	This is based on the 'Backward equation' for Region 4
	@see IF-97 p 34
*/
Temperature SteamRegionBoundaries::getSatTemp_p(Pressure p) {

	Num b = beta(p);
	Num d = D(E(b), F(b), G(b));

	return (REGION4_N[10] + d -
	        pow(sq(REGION4_N[10] + d) -
	            4 * (REGION4_N[9] + REGION4_N[10] * d),
	            (Num) 0.5)) / 2;

}

/// Get the saturation pressure for a specific temperature
/**
	This the 'Basic equation' for Region 4,
	@see IF-97 p 33
*/
Pressure SteamRegionBoundaries::getSatPres_T(Temperature T) {

	//cerr << "getSatPres_T: T = " << K_TO_C(T) << "°C" << endl;

	REQUIRE(T <= T_CRIT);
	REQUIRE(T >= T_MIN);

	Num u = SteamRegionBoundaries::upsilon(T);
	Num a = A(u);
	Num b = B(u);
	Num c = C(u);

	Num psat =
	    pow(2 * c / (-b + pow(sq(b) - 4 * a * c, (Num) 0.5)),
	        (Num) 4);

	//cerr << "getSatPres_T: psat = " << setprecision(15) << psat << endl;
	//ENSURE(psat <= P_CRIT + STEAM_P_EPS);
	//ENSURE(psat >= REG4_P_MIN);

	//cerr << "... gives p = " << psat << " MPa" << endl;

	return psat;
}

/// Test for a pressure/temperature being in Region 1
/**
	@param p pressure / [MPa]
	@param T temperature / [K]]
	@param throw_me Throw an error if point is not in Region 1
*/
bool SteamRegionBoundaries::isRegion1_pT(Pressure p, Temperature T,
        bool throw_me) {

	if (!isValid_pT(p, T, throw_me)) {
		return false;
	}

	if (T > REG1_T_HIGH) {
		if (throw_me)
			throw new SteamCalculatorException(p, T, REG1_RANGE_T_HIGH);
		return false;

	} else if (T > getSatTemp_p(p) + STM_SATURATION_TOL * T) {
		if (throw_me) {
			cerr << "Temperature is too high, by " << (T - getSatTemp_p(p)) << endl;
			throw new SteamCalculatorException(p, T, REG1_RANGE_P_LOW);
		}
		return false;
	}

	return true;
}

/// Test for a pressure/temperature being in Region 2
/**
	@param p pressure / [MPa]
	@param T temperature / [K]]
	@param throw_me Throw an error if point is not in Region 2
*/
bool SteamRegionBoundaries::isRegion2_pT(Pressure p, Temperature T,
        bool throw_me) {

	if (!isValid_pT(p, T, throw_me)) {
		return false;
	}
	// check region 2 under region 1/4
	if (T <= TB_LOW) {
		if (T < getSatTemp_p(p) - STM_SATURATION_TOL * T) {
			if (throw_me)
				throw new SteamCalculatorException(p, T,
				                                   REG2_RANGE_T_LOW_TS);
			return false;
		}
		// check region 2 under region 3
	} else if (T <= TB_HIGH) {
		if (p > getpbound_T(T)) {
			if (throw_me)
				throw new SteamCalculatorException(p, T,
				                                   REG2_RANGE_P_HIGH_PB);
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
bool SteamRegionBoundaries::isRegion3_pT(Pressure p, Temperature T,
        bool throw_me) {

	// if scanning to see what region you're in, scan region 3 last 'cause it's less slick (just a little)

	if (!isValid_pT(p, T, throw_me)) {
		return false;
	}

	if (T < TB_LOW) {
		if (throw_me)
			throw new SteamCalculatorException(p, T, REG3_RANGE_T_LOW_TB);
		return false;

	} else if (getpbound_T(T, throw_me) > p) {
		if (throw_me)
			throw new SteamCalculatorException(p, T, REG3_RANGE_P_LOW_PB);
		return false;
	}

	return true;
}

//-------------------------------------------
// PRIVATE FUNCTIONS

//-----------------
// For Tsat(p)...

/**
	@see IF-97, p 33, Eqn 29a
*/
Num SteamRegionBoundaries::beta(Pressure p) {
	return pow(p, (Num) 0.25);
}

/**
	@see IF-97, p 34, Eqn 31
*/
Num SteamRegionBoundaries::E(Num bet) {
	return sq(bet) + REGION4_N[3] * bet + REGION4_N[6];
}

/**
	@see IF-97, p 34, Eqn 31
*/
Num SteamRegionBoundaries::F(Num bet) {
	return REGION4_N[1] * sq(bet) + REGION4_N[4] * bet + REGION4_N[7];
}

/**
	@see IF-97, p 34, Eqn 31
*/
Num SteamRegionBoundaries::G(Num bet) {
	return REGION4_N[2] * sq(bet) + REGION4_N[5] * bet + REGION4_N[8];
}

/**
	@see IF-97, p 34, Eqn 31
*/
Num
SteamRegionBoundaries::D(Num E, Num F, Num G) {
	return 2 * G / (-F - pow(sq(F) - 4 * E * G, (Num) 0.5));
}

//------------------
// For p_sat(T)...

/**
	@see IF-97, p 33, Eqn 29b
*/
Num SteamRegionBoundaries::upsilon(Temperature T) {
	return T + REGION4_N[9] / (T - REGION4_N[10]);
}

/**
	@see IF-97, p 33, Eqn 30
*/
Num SteamRegionBoundaries::A(Num ups) {

	return sq(ups) + REGION4_N[1] * ups + REGION4_N[2];
}

/**
	@see IF-97, p 33, Eqn 30
*/
Num SteamRegionBoundaries::B(Num ups) {

	return REGION4_N[3] * sq(ups) + REGION4_N[4] * ups + REGION4_N[5];
}

/**
	@see IF-97, p 33, Eqn 30
*/
Num SteamRegionBoundaries::C(Num ups) {

	return REGION4_N[6] * sq(ups) + REGION4_N[7] * ups + REGION4_N[8];
}

//---------------
// For B23:

Pressure SteamRegionBoundaries::getpbound_T(Temperature T, bool throw_me) {

	if (T < TB_LOW) {
		if (throw_me) {
			throw new SteamCalculatorException(-1, T, REG2_RANGE_TB_LOW);
		}
	} else if (T > TB_HIGH) {
		if (throw_me) {
			throw new SteamCalculatorException(-1, T, REG2_RANGE_TB_HIGH);
		}
	}

	Num thet = T;	// T in Kelvin

	Num pi =
	    REGION2_NB[1] + REGION2_NB[2] * thet + REGION2_NB[3] * sq(thet);

	//SHOW_VALUE(pi);

	return pi * 1;

}

Temperature SteamRegionBoundaries::getTbound_p(Pressure p, bool throw_me) {

	if (p < PB_LOW) {
		if (throw_me) {
			throw new SteamCalculatorException(p, -1, REG2_RANGE_PB_LOW);
		}
	} else if (p > REG2_P_HIGH) {
		if (throw_me) {
			throw new SteamCalculatorException(p, -1, REG2_RANGE_PB_HIGH);
		}
	}

	Num pi = p;

	Num thet =
	    REGION2_NB[4] + pow((pi - REGION2_NB[5]) / REGION2_NB[3],
	                        (Num) 0.5);

	return thet * 1;
}
