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

*/

#include "region1.h"
#include "steamcalculator_macros.h"
#include <cmath>
#include <cstdio>

Region1 *Region1::_instance = 0;

Region1::Region1(){}

SteamState *Region1::Instance() {
	if (_instance == 0) {
		_instance = new Region1();
	}
	return _instance;
}

int Region1::getRegion() const{
	return 1;
}

void Region1::set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x) {
	c.p = p;
	c.pi = p / REG1_PRES_REF;
	c.T = T;
	c.tau = REG1_TEMP_REF / T;
	c.x = 0.0;
	c.isset = true;
	//cerr << "Region1::set_pT(p=" << c->p << ",T=" << c->T << ")"<< endl;
	ENSURE(c.whichRegion() == 1);
}

//----------------------------------------------------------------
// REGION 1 PROPERTIES (SAME AS REGION 2!)

/**
* @return Temperature of water/steam [K]
*/
inline Temperature
Region1::temp(const SteamCalculator &c) const{
	REQUIRE(c.T > 0.0 * Kelvin);
	return c.T;
}

/**
* @return Pressure of steam/water[MPa]
*/
inline Pressure
Region1::pres(const SteamCalculator &c) const{
	REQUIRE(c.p >= 0.0 * MPa);
	return c.p;
}

/**
* @return Specific volume of steam [m³/kg]
*/
inline SpecificVolume
Region1::specvol(const SteamCalculator &c) const{
	return (R * c.T / c.p) * c.pi * gampi(c);
}

/**
* @return Density of steam [kg/m³]
*/
inline Density
Region1::dens(const SteamCalculator &c) const{
	return 1.0 / specvol(c);
}

/*
Num Steam::dens(){
	Num dens=0;
	dens=(1/specvol());
	printf("NOTE: in Steam::dens(), dens = %10.8e (region %d)\n",dens,whereami());
	return dens;
}
*/
/**
* @return Specific internal energy of water/steam [kJ/kg]
*/
SpecificEnergy
Region1::specienergy(const SteamCalculator &c) const{
	return (R * c.T) * (c.tau * gamtau(c) - c.pi * gampi(c));
}

/**
* @return Specific entropy water/steam [kJ/kg]
*/
SpecificEntropy
Region1::specentropy(const SteamCalculator &c) const{
	return R * (c.tau * gamtau(c) - gam(c));
}

/**
* @return Specific enthalpy of water/steam [kJ/kg]
*/
SpecificEnergy
Region1::specenthalpy(const SteamCalculator &c) const{
	return R * c.T * (c.tau * gamtau(c));
}

/**
* @return Isobaric specific heat capacity of water/steam [kJ/kgK]
*/
SpecHeatCap
Region1::speccp(const SteamCalculator &c) const{
	return R * (-sq(c.tau) * gamtautau(c));
}

/**
* @return Isochoric specific heat capacity of water/steam [kJ/kgK]
*/
SpecHeatCap
Region1::speccv(const SteamCalculator &c) const{
	return R * (-sq(c.tau) * gamtautau(c) +
                           sq(gampi(c) -
                              c.tau * gampitau(c)) / gampipi(c));
}


//----------------------------------------------------------------
// REGION 1 CORRELATION DATA

#define REG1_COUNT 34

const int REGION1_I[REG1_COUNT] = {
                                      0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4,
                                      4, 4, 5, 8, 8, 21, 23, 29, 30, 31, 32
                                  };

const int REGION1_J[REG1_COUNT] = {
                                      -2, -1, 0, 1, 2, 3, 4, 5, -9, -7, -1, 0, 1, 3, -3, 0, 1, 3, 17, -4,
                                      0, 6, -5, -2, 10, -8, -11, -6, -29, -31, -38, -39, -40, -41
                                  };

const Num REGION1_N[REG1_COUNT] = {
            0.14632971213167E+00, -0.84548187169114E+00, -0.37563603672040E+01,
            0.33855169168385E+01, -0.95791963387872E+00, 0.15772038513228E+00, -0.16616417199501E-01,
            0.81214629983568E-03, 0.28319080123804E-03, -0.60706301565874E-03, -0.18990068218419E-01,
            -0.32529748770505E-01, -0.21841717175414E-01, -0.52838357969930E-04, -0.47184321073267E-03,
            -0.30001780793026E-03, 0.47661393906987E-04, -0.44141845330846E-05, -0.72694996297594E-15,
            -0.31679644845054E-04, -0.28270797985312E-05, -0.85205128120103E-09, -0.22425281908000E-05,
            -0.65171222895601E-06, -0.14341729937924E-12, -0.40516996860117E-06, -0.12734301741641E-08,
            -0.17424871230634E-09, -0.68762131295531E-18, 0.14478307828521E-19, 0.26335781662795E-22,
            -0.11947622640071E-22, 0.18228094581404E-23, -0.93537087292458E-25
        };

//----------------------------------------------------------------
// REGION 1 PARAMETER CALCULATIONS

// these macros define methods of Region1 to evaluate the power series expansions for gam, gampi, etc. to calculate the material properties, methods of the base Steam class call these here methods.

#define LOOP_REG1(FUNC,EXPR) LOOP_SUM_FUNC_OBJ(Region1,FUNC,EXPR,REG1_COUNT)

LOOP_REG1(gam,
          REGION1_N[i] * pow(7.1 - c.pi,
                             REGION1_I[i]) * pow(c.tau - 1.222,
                                                 REGION1_J[i])
         );


LOOP_REG1(gampi,
          -REGION1_N[i] * REGION1_I[i] * pow(7.1 - c.pi,
                                             REGION1_I[i] -
                                             1) * pow(c.tau - 1.222,
                                                      REGION1_J[i])
         );

LOOP_REG1(gampipi,
          REGION1_N[i] * REGION1_I[i] * (REGION1_I[i] - 1) * pow(7.1 -
                  c.pi,
                  REGION1_I
                  [i] -
                  2) *
          pow(c.tau - 1.222, REGION1_J[i])
         );


LOOP_REG1(gamtau,
          REGION1_N[i] * pow(7.1 - c.pi,
                             REGION1_I[i]) * REGION1_J[i] * pow(c.tau -
                                                                1.222,
                                                                REGION1_J
                                                                [i] - 1)
         );

LOOP_REG1(gamtautau,
          REGION1_N[i] * pow(7.1 - c.pi,
                             REGION1_I[i]) * REGION1_J[i] * (REGION1_J[i] -
                                                             1) *
          pow(c.tau - 1.222, REGION1_J[i] - 2)
         );

LOOP_REG1(gampitau,
          -REGION1_N[i] * REGION1_I[i] * pow(7.1 - c.pi,
                                             REGION1_I[i] -
                                             1) * REGION1_J[i] *
          pow(c.tau - 1.222, REGION1_J[i] - 1));

// for use in thermal conductivity

#define EVAL_REG1(fun,expr) EXPR_FUNC_OBJ(Region1,fun,expr,Num)

EVAL_REG1(pitau_iaps85,
	IAPS85_TEMP_REF * REG1_PRES_REF
		* (gampitau(c) * REG1_TEMP_REF - gampi(c) * c.T)
		/ (IAPS85_PRES_REF * sq(c.T) * gampipi(c))
);

EVAL_REG1(delpi_iaps85,
	- IAPS85_PRES_REF
		/ IAPS85_DENS_REF
		/ R
		/ c.T
		* gampipi(c)
		/ sq(gampi(c))
);
