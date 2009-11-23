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

/* Appendix B: Recommended Interpolating equation for Industrial Use */
/* see http://www.iapws.org/relguide/thcond.pdf */

#define FREESTEAM_BUILDING_LIB
#include "thcond.h"

#include <math.h>

#define THCOND_DEBUG

#ifdef THCOND_DEBUG
# include <assert.h>
# include <stdio.h>
#endif

#define IAPWS_THCOND_T_REF 647.26
#define IAPWS_THCOND_DENS_REF 317.7
#define IAPWS_THCON_REF 1.0


double freesteam_k_rhoT(double rho, double T){
	double Tbar = T / IAPWS_THCOND_T_REF;
	double rhobar = rho / IAPWS_THCOND_DENS_REF;

	/* lam0 -- fast implementation without 'pow' function */
	double Troot = sqrt(Tbar);
	double Tpow = Troot;
	double lam0 = 0;

#define THCON_a_COUNT 4
	const double THCON_a[THCON_a_COUNT] = {
		0.0102811
		,0.0299621
		,0.0156146
		,-0.00422464
	};

	int k;
	for(k = 0; k < THCON_a_COUNT; ++k) {
		lam0 += THCON_a[k] * Tpow;
		Tpow *= Tbar;
	}

#define THCON_b0 -0.397070
#define THCON_b1 0.400302
#define THCON_b2 1.060000
#define THCON_B1 -0.171587
#define THCON_B2 2.392190

	double lam1 = THCON_b0 + THCON_b1 * rhobar + THCON_b2 * exp(THCON_B1 * SQ(rhobar + THCON_B2));

#define THCON_C1 0.642857
#define THCON_C2 -4.11717
#define THCON_C3 -6.17937
#define THCON_C4 0.00308976
#define THCON_C5 0.0822994
#define THCON_C6 10.0932
	
	double DTbar = fabs(Tbar - 1) + THCON_C4;
	double Q = 2. + THCON_C5 / pow(DTbar,0.6);

	double S;
	if(Tbar >= 1){
		S = 1. / DTbar;
	}else{
		S = THCON_C6 / pow(DTbar,0.6);
	}

#define THCON_d1 0.0701309
#define THCON_d2 0.0118520
#define THCON_d3 0.00169937
#define THCON_d4 -1.0200

#if 0
	double lam2 = 
		(THCON_d1 / pow(Tbar,10) + THCON_d2) * pow(rhobar,1.8) * 
			exp(THCON_C1 * (1 - pow(rhobar,2.8)))
		+ THCON_d3 * S * pow(rhobar,Q) *
			exp((Q/(1+Q))*(1 - pow(rhobar,1+Q)))
		+ THCON_d4 *
			exp(THCON_C2 * pow(Tbar,3./2) + THCON_C3 / pow(rhobar,5));
#endif

	double lam2_1_1 = (THCON_d1 / pow(Tbar,10) + THCON_d2);

#ifdef THCOND_DEBUG
	fprintf(stderr,"pow(Tbar,10) = %f, lam2_1_1 = %f\n",pow(Tbar,10), lam2_1_1);
#endif	

	double lam2_1 = lam2_1_1 * pow(rhobar,1.8) * 
			exp(THCON_C1 * (1. - pow(rhobar,2.8)));
		
	double lam2_2 = THCON_d3 * S * pow(rhobar,Q) *
			exp((Q/(1.+Q))*(1. - pow(rhobar,1+Q)));

	double lam2_3 = THCON_d4 *
			exp(THCON_C2 * pow(Tbar,1.5) + THCON_C3 / pow(rhobar,5));

#ifdef THCOND_DEBUG
	fprintf(stderr,"lam2_1 = %f, lam2_2 = %f, lam2_3 = %f\n",lam2_1, lam2_2, lam2_3);
#endif	
	
	double lam2 = lam2_1 + lam2_2 + lam2_3;

#ifdef THCOND_DEBUG
	fprintf(stderr,"lam0 = %f, lam1 = %f, lam2 = %f\n",lam0, lam1, lam2);
	fprintf(stderr,"k0 =%f, k1 = %f, k2 = %f\n",IAPWS_THCON_REF*lam0, IAPWS_THCON_REF*lam1, IAPWS_THCON_REF*lam2);
	fprintf(stderr,"returning k = %f\n", IAPWS_THCON_REF * (lam0 + lam1 + lam2));
#endif

	return IAPWS_THCON_REF * (lam0 + lam1 + lam2);
}

