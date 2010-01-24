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

#include "region2.h"
#include "steamcalculator.h"
#include "steamcalculator_macros.h"
#include "isinfnan.h"

Region2 *Region2::_instance = 0;

Region2::Region2() {}

SteamState *Region2::Instance() {
	if (_instance == 0) {
		_instance = new Region2();
	}
	return _instance;
}

int Region2::getRegion() const{
	return 2;
}

void Region2::set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x) {
	c.p = p;
	c.pi = p / (1.0 * MPa);			// p / 1 MPa
	c.T = T;
	c.tau = REG2_TEMP_REF / T;
	c.x = 1.0;
	c.isset = true;
	ENSURE(c.whichRegion() == 2);
}

//------------------------------------------------------------------------------
// REGION 2 PROPERTIES (SAME AS REGION 1!)

/**
* @return Temperature of water/steam [K]
*/
inline Temperature
Region2::temp(const SteamCalculator &c) const{
	REQUIRE(c.T > 0.0 * Kelvin);
	return c.T;
}

/**
* @return Pressure of steam/water[MPa]
*/
inline Pressure
Region2::pres(const SteamCalculator &c) const{
	REQUIRE(c.p >= 0.0 * MPa);
	return c.p;
}

/**
* @return Specific volume of steam [m³/kg]
*/
inline SpecificVolume
Region2::specvol(const SteamCalculator &c) const{
	return (R * c.T / c.p) * c.pi * gampi(c);
}

/**
* @return Density of steam [kg/m³]
*/
inline Density
Region2::dens(const SteamCalculator &c) const{
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
Region2::specienergy(const SteamCalculator &c) const{
	return (R * c.T) * (c.tau * gamtau(c) - c.pi * gampi(c));
}

/**
* @return Specific entropy water/steam [kJ/kg]
*/
SpecificEntropy
Region2::specentropy(const SteamCalculator &c) const{
	return R * (c.tau * gamtau(c) - gam(c));
}

/**
* @return Specific enthalpy of water/steam [kJ/kg]
*/
SpecificEnergy
Region2::specenthalpy(const SteamCalculator &c) const{
	return R * c.T * (c.tau * gamtau(c));
}

/**
* @return Isobaric specific heat capacity of water/steam [kJ/kgK]
*/
SpecHeatCap
Region2::speccp(const SteamCalculator &c) const{
	return R * (-sq(c.tau) * gamtautau(c));
}

/**
* @return Isochoric specific heat capacity of water/steam [kJ/kgK]
*/
SpecHeatCap
Region2::speccv(const SteamCalculator &c) const{
	return R * (-sq(c.tau) * gamtautau(c) +
                           sq(gampi(c) -
                              c.tau * gampitau(c)) / gampipi(c));
}

//------------------------------------------------------------------------------
// CORRELATION DATA

// Ideal Gas Series Data

#define REG2I_COUNT 9

const int REGION2_J0[REG2I_COUNT] = { 0, 1, -5, -4, -3, -2, -1, 2, 3 };

const Num REGION2_N0[REG2I_COUNT] = {
            -0.96927686500217E+01, 0.10086655968018E+02, -0.56087911283020E-02,
            0.71452738081455E-01, -0.40710498223928E+00, 0.14240819171444E+01, -0.43839511319450E+01,
            -0.28408632460772E+00, 0.21268463753307E-01
        };

// Residual Series Data

#define REG2R_COUNT 43

const int REGION2_I[REG2R_COUNT] = {
                                       1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 6, 6, 7,
                                       7, 7, 8, 8, 9, 10, 10, 10, 16, 16, 18, 20, 20, 20, 21, 22, 23, 24, 24, 24
                                   };

const int REGION2_J[REG2R_COUNT] = {
                                       0, 1, 2, 3, 6, 1, 2, 4, 7, 36, 0, 1, 3, 6, 35, 1, 2, 3, 7, 3, 16, 35,
                                       0, 11, 25, 8, 36, 13, 4, 10, 14, 29, 50, 57, 20, 35, 48, 21, 53, 39, 26, 40, 58
                                   };

const Num REGION2_N[REG2R_COUNT] = {
            -0.17731742473213E-02, -0.17834862292358E-01, -0.45996013696365E-01,
            -0.57581259083432E-01, -0.50325278727930E-01, -0.33032641670203E-04, -0.18948987516315E-03,
            -0.39392777243355E-02, -0.43797295650573E-01, -0.26674547914087E-04, 0.20481737692309E-07,
            0.43870667284435E-06, -0.32277677238570E-04, -0.15033924542148E-02, -0.40668253562649E-01,
            -0.78847309559367E-09, 0.12790717852285E-07, 0.48225372718507E-06, 0.22922076337661E-05,
            -0.16714766451061E-10, -0.21171472321355E-02, -0.23895741934104E+02, -0.59059564324270E-17,
            -0.12621808899101E-05, -0.38946842435739E-01, 0.11256211360459E-10, -0.82311340897998E+01,
            0.19809712802088E-07, 0.10406965210174E-18, -0.10234747095929E-12, -0.10018179379511E-08,
            -0.80882908646985E-10, 0.10693031879409E+00, -0.33662250574171E+00, 0.89185845355421E-24,
            0.30629316876232E-12, -0.42002467698208E-05, -0.59056029685639E-25, 0.37826947613457E-05,
            -0.12768608934681E-14, 0.73087610595061E-28, 0.55414715350778E-16, -0.94369707241210E-06
        };

//-------------------------------------------------------------------------

#define LOOP_REG2I(FUNC,EXPR) LOOP_SUM_FUNC_OBJ(Region2,FUNC,EXPR,REG2I_COUNT)
#define LOOP_REG2R(FUNC,EXPR) LOOP_SUM_FUNC_OBJ(Region2,FUNC,EXPR,REG2R_COUNT)
#define EVAL_REG2(FUNC,EXPR) EXPR_FUNC_OBJ(Region2,FUNC,EXPR,Num)

#include <cstdlib>
#include <cmath>
#include <cstdio>

// implement gam etc as sum of ideal gas part and residual part

EVAL_REG2(gam, gam0(c) + gamr(c))

EVAL_REG2(gamtau, gamtau0(c) + gamtaur(c))

EVAL_REG2(gampi, gampi0(c) + gampir(c))

EVAL_REG2(gamtautau, gamtautau0(c) + gamtautaur(c))

EVAL_REG2(gampipi, gampipi0(c) + gampipir(c))

EVAL_REG2(gampitau, gampitau0(c) + gampitaur(c))
// ideal gas part
EVAL_REG2(gam0, log(c.pi) + gam0sum(c))

EVAL_REG2(gampi0, 1 / c.pi)

EVAL_REG2(gampipi0, -1 / sq(c.pi))
// ideal gas part: series expansions:
LOOP_REG2I(gam0sum, REGION2_N0[i] * pow(c.tau, REGION2_J0[i]));

LOOP_REG2I(gamtau0,
           REGION2_N0[i] * REGION2_J0[i] * pow(c.tau, REGION2_J0[i] - 1));

LOOP_REG2I(gamtautau0,
           REGION2_N0[i] * REGION2_J0[i] * (REGION2_J0[i] -
                                            1) * pow(c.tau,
                                                     REGION2_J0[i] - 2));
/**
	@todo
		Make gampitau0 not require a steamcalculator argument

	@todo
		Convert LOOP_REG2I etc etc to expression template format a la Blitz++ etc
*/
LOOP_REG2I(gampitau0, 0);

// residual parts

LOOP_REG2R(gamr,
           REGION2_N[i] * pow(c.pi, REGION2_I[i]) * pow(c.tau - 0.5,
                   REGION2_J[i]));

LOOP_REG2R(gampir,
           REGION2_N[i] * REGION2_I[i] * pow(c.pi,
                                             REGION2_I[i] -
                                             1) * pow(c.tau - 0.5,
                                                      REGION2_J[i]));

LOOP_REG2R(gampipir,
           REGION2_N[i] * REGION2_I[i] * (REGION2_I[i] - 1) * pow(c.pi,
                   REGION2_I
                   [i] -
                   2) *
           pow(c.tau - 0.5, REGION2_J[i]));

LOOP_REG2R(gamtaur,
           REGION2_N[i] * pow(c.pi,
                              REGION2_I[i]) * REGION2_J[i] * pow(c.tau -
                                                                 0.5,
                                                                 REGION2_J
                                                                 [i] - 1));

LOOP_REG2R(gamtautaur,
           REGION2_N[i] * pow(c.pi,
                              REGION2_I[i]) * REGION2_J[i] *
           (REGION2_J[i] - 1) * pow(c.tau - 0.5, REGION2_J[i] - 2));

LOOP_REG2R(gampitaur,
           REGION2_N[i] * REGION2_I[i] * pow(c.pi,
                                             REGION2_I[i] -
                                             1) * REGION2_J[i] *
           pow(c.tau - 0.5, REGION2_J[i] - 1));

// for thermal conductivity...

/**
	@see http://www.cheresources.com/iapwsif972.pdf#page=5 at 'Region 2':
*/
Num Region2::pitau_iaps85(const SteamCalculator &c) const {
	Num pitau_iaps85 = 0;
	IS_VALID_REF(c);
	REQUIRE(c.T > 0.0*Kelvin);
	REQUIRE(gampipi(c) != 0);
	REQUIRE(!isinf(gampitau(c)));
	REQUIRE(!isinf(gampi(c)));
	REQUIRE(!isinf(c.T));

	pitau_iaps85 = IAPS85_TEMP_REF / IAPS85_PRES_REF \
		* REG1_PRES_REF * (gampitau(c) * IAPS85_TEMP_REG2_REF - gampi(c) * c.T) \
		/ (sq(c.T) * gampipi(c));

	//cerr << "pitau_iaps85 evaluation... value = " << pitau_iaps85 << endl;

	ENSURE(!isinf(pitau_iaps85));
	ENSURE(!isnan(pitau_iaps85));
	return pitau_iaps85;
}


/**
	@see http://www.cheresources.com/iapwsif972.pdf#page=5 at 'Region 2':
*/
Num Region2::delpi_iaps85(const SteamCalculator &c) const {
	Num delpi_iaps85 = 0;
	//Num gp = gampi(c);
	delpi_iaps85 = -IAPS85_PRES_REF / IAPS85_DENS_REF / R / c.T * gampipi(c) / sq(gampi(c));
	ENSURE(!isnan(delpi_iaps85));
	return delpi_iaps85;
}
