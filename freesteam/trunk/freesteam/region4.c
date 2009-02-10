/*
freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2009  John Pye

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

#include "region4.h"

#include <math.h>

/*------------------------------------------------------------------------------
  REGION 4 SATURATION CURVE Tsat(p)
*/

const double REGION4_N[11] = { 0, 0.11670521452767E+04, -0.72421316703206E+06
	, -0.17073846940092E+02, 0.12020824702470E+05, -0.32325550322333E+07
	, 0.14915108613530E+02, -0.48232657361591E+04, 0.40511340542057E+06
	, -0.23855557567849E+00, 0.65017534844798E+03
};

#define REGION4_BETA(P) (pow(p/REGION4_PSTAR, 0.25))
#define REGION4_E(BET) (SQ(BET) + REGION4_N[3] * BET + REGION4_N[6])
#define REGION4_F(BET) (REGION4_N[1] * SQ(BET) + REGION4_N[4] * BET + REGION4_N[7])
#define REGION4_G(BET) (REGION4_N[2] * SQ(BET) + REGION4_N[5] * BET + REGION4_N[8])

#define REGION4_PSTAR 1e6 /* Pa */

double freesteam_region4_Tsat_p(double p){
	double beta = REGION4_BETA(p);
	double E = REGION4_E(beta);
	double F = REGION4_F(beta);
	double G = REGION4_G(beta);
	double D = 2. * G / (-F - sqrt(sq(F) - 4. * E * G));

	double theta = 0.5 * (REGION4_N[10] + D - sqrt(SQ(REGION4_N[10] + D) - 4.0 * (REGION4_N[9] + REGION4_N[10] * D)));

	return theta /* * REGION4_TSTAR = 1 K */;
}

#if 0

#define REGION4_A(UPS) (SQ(ups) + REGION4_N[1] * UPS + REGION4_N[2])
#define REGION4_B(UPS) (REGION4_N[3] * SQ(UPS) + REGION4_N[4] * UPS + REGION4_N[5])
#define REGION4_C(UPS) (REGION4_N[6] * SQ(UPS) + REGION4_N[7] * UPS + REGION4_N[8])

}


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


#endif

