#ifndef COMMON_H
#define COMMON_H

#include "units.h"

// this is the precision of all the parameters used in the Steam Calculator:
typedef double Num;
#define NUM_DEFINED

// define a method (for use in header files)
#define METHOD_H_OBJ(funcname) Num funcname(SteamCalculator *c)

const SpecificGasConstant R=0.461526 * kJ_kgK; // Specific gas constant for water from IF97

#define REG4_TOL 0.001		// relative err on pressures considerd to be equal to psat.

const Temperature TB_HIGH = 863.15 * Kelvin;
const Temperature T_MIN = ZeroCelsius;
const Temperature T_MAX = 1073.15 * Kelvin;
const Temperature T_CRIT = 647.096 * Kelvin;	// critical-point temperature
const Temperature T_TRIPLE = 273.16 * Kelvin;	// triple-point temperature
const Temperature REG2_TEMP_REF = 540.0 * Kelvin;
const Temperature REG1_TEMP_REF = 1386.0 * Kelvin;
const Temperature REG1_T_LOW = ZeroCelsius;
const Temperature REG2_T_LOW = ZeroCelsius;
const Temperature REG2_T_HIGH = T_MAX;

const Temperature T_REG1_REG3 = 623.15 * Kelvin;
const Temperature TB_LOW = T_REG1_REG3;

const Temperature T_MIN_VOL = fromcelsius(3.984);

const Pressure P_MAX = 100.0 * MPa;
const Pressure PB_LOW = 16.5292 * MPa;
const Pressure P_MIN = 0.0 * Pascal;
const Pressure P_CRIT = 22.064 * MPa;	// critical-point pressure
const Pressure P_TRIPLE = 611.657 * Pascal;	// triple-point pressure
const Pressure REG4_P_MIN = 611.213 * Pascal;	// minimum pressure for region 4 (IF-97 eq 31 & p 35) / [MPa]
const Pressure REG2_P_HIGH = P_MAX;
const Pressure REG1_P_HIGH = P_MAX;
const Pressure REG1_PRES_REF = 16.53 * MPa;
const Pressure REG2_PRES_REF = 1.0 * MPa;

const Density RHO_CRIT = 322.0 * kg / metre3;	// critical-point density / [kg/m³]

/// @see http://www.iapws.org/relguide/visc.pdf#page=7 Eq (4)
const DynamicViscosity IAPS85_VISC_REF = 55.071 * micro * Pascal * second;

/// @see http://www.iapws.org/relguide/visc.pdf#page=7 Eq (2)
const Density IAPS85_DENS_REF = 317.763 * kg_m3;

/// @see http://www.iapws.org/relguide/visc.pdf#page=7 Eq (1)
const Temperature IAPS85_TEMP_REF = 647.226 * Kelvin;

/// @see http://www.iapws.org/relguide/visc.pdf#page=7 Eq (4)
const Pressure IAPS85_PRES_REF = 22.115 * MPa;	// MPa (THIS IS *NOT* EQUAL TO P_CRIT!)

/// @see http://www.iapws.org/relguide/visc.pdf#page=7
const Conductivity IAPS85_THCOND_REF = 0.49450 * Watt / metre / Kelvin;

const Temperature IAPS85_TEMP_REG2_REF = 540.0 * Kelvin;

const Pressure STEAM_P_EPS = 1.0e-5 * MPa;
const Temperature STEAM_T_EPS = 5.0e-4 * Kelvin;

const Temperature EPS_T_CRIT=0.00007 * Kelvin;

const Temperature T_CRIT_PLUS=(T_CRIT + STEAM_T_EPS);

const Density REG3_ZEROIN_DENS_MAX = 765.0 * kg_m3;
const Density REG3_ZEROIN_TOL= 1e-18 * kg_m3;

#define MPA_TO_BAR(PRES) (((Num)(PRES)) * 10.0      )
#define BAR_TO_MPA(PRES) (((Num)(PRES)) * 0.1       )
#define PA_TO_MPA(PRES)  (((Num)(PRES)) * 0.000001  )
#define MPA_TO_PA(PRES)  (((Num)(PRES)) * 1.0E6     )
#define KJKG_TO_JKG(JKG) (((Num)(KJKG)) * 1000.0    )
#define BAR_TO_PA(PRES)  (((Num)(PRES)) * 100.0E3   )
#define KPA_TO_MPA(PRES) (((Num)(PRES)) * 0.001     )

#define W_TO_KW(W) (((Num)(W))*0.001)
#define KJ_TO_J(KJ) (((Num)(KJ))*0.001)
#define J_TO_KJ(J) (((Num)(J))*0.001)

const Acceleration GRAV = 9.81 * Newton / kg;	// N/kg, gravitation acceleration

#define PI 3.14159265358

// Property 'Alternative' codes for use with SatCurve and B23Curve and Solver2

const int SOLVE_IENERGY=0;
const int SOLVE_ENTHALPY=1;

const int SOLVE_ENTROPY=0;
const int SOLVE_CV=1;
const int SOLVE_CP=2;

// square a value but only evaluate it once in GNU C's CPP.

template<class T>
T sq(const T& t){
	return t*t;
}

#endif

