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
#define FREESTEAM_BUILDING_LIB
#include "derivs.h"

#include <stdlib.h>

/* forward decls */

/*
The following functions basically look up the contents of Table 1 and Table 2
from the IAPWS Guideline.
*/

static double VT3(char x, SteamState S);
static double TV3(char x, SteamState S);

static double PT1(char x, SteamState S);
static double TP1(char x, SteamState S);

static double PT2(char x, SteamState S);
static double TP2(char x, SteamState S);

static double freesteam_region3_alphap_rhoT(double rho, double T);
static double freesteam_region3_betap_rhoT(double rho, double T);

/* macros to help with propert evals */

#define ZXY(VT,TV,S) ((VT(z,S)*TV(y,S)-TV(z,S)*VT(y,S))/(VT(x,S)*TV(y,S)-TV(x,S)*VT(y,S)))

/*------------------------------------------------------------------------------
  EXPORTED FUNCTION(S)
*/

/**
	Calculates the derivative 

	 ⎰ ∂z ⎱
     ⎱ ∂x ⎰y

	@param S steam state, already calculated using steam_ph, etc.
	@param x in above equation, character one of pTvuhsgaf.
	@param y in above equation, character one of pTvuhsgaf.
	@param z in above equation, character one of pTvuhsgaf.
	Note that Helmholtz free energy can be signified by either 'a' or 'f'.

	@NOTE that the variables ARE NOT IN ALPHABETICAL ORDER.

	@return the numerical value of the derivative (∂z/∂x)y.
*/
double freesteam_deriv(SteamState S, char z, char x, char y){
	switch(S.region){
		case 1:	return ZXY(PT1,TP1,S);
		case 2: return ZXY(PT2,TP2,S);
		case 3: return ZXY(VT3,TV3,S);
	}
}

/*------------------------------------------------------------------------------
  REGION 3 DERIVATIVES
*/

#define rho S.R3.rho
#define T S.R3.T
#define p freesteam_region3_p_rhoT(rho,T)
#define cv freesteam_region3_cv_rhoT(rho,T)
#define v (1./rho)
#define s freesteam_region3_s_rhoT(rho,T)
#define alphap freesteam_region3_alphap_pT(rho,T)
#define betap freesteam_region3_betap_pT(rho,T)

/**
	TODO convert char to enum for better compiler checking capability
*/
double VT3(char x, SteamState S){
	switch(x){
		case 'p': return -p*betap;
		case 'T': return 0;
		case 'v': return 1;
		case 'u': return p*(T*alphap-1.);
		case 'h': return p*(T*alphap-v*betap);
		case 's': return p*alphap;
		case 'g': return -p*v*betap;
		case 'a':
		case 'f': return -p;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid character x = '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
}

double TV3(char x, SteamState S){
	switch(x){
		case 'p': return p*alphap;
		case 'T': return 1;
		case 'v': return 0;
		case 'u': return cv;
		case 'h': return cv + p*v*alphap;
		case 's': return cv/alphap;
		case 'g': return p*v*alphap - s;
		case 'a':
		case 'f': return -s;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid character x = '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
}

#undef rho
#undef T
#undef p
#undef cv
#undef v
#undef s
#undef alphap
#undef betap


/* FIXME copied from region3.c, not a good way of doing things */
#define REGION3_ARHOT_TSTAR 647.096 /* K */
#define REGION3_ARHOT_RHOSTAR 322. /* K */
#define DEFINE_DELTAU(RHO,T) \
	double del = rho / REGION3_ARHOT_RHOSTAR; \
	double tau = REGION3_ARHOT_TSTAR / T

double freesteam_region3_alphap_rhoT(double rho, double T){
	DEFINE_DELTAU(RHO,T);
	return 1./T * (1. - tau*phideltau(del,tau)/phidel(del,tau));
}

double freesteam_region3_betap_pT(double p, double T);
	DEFINE_DELTAU(RHO,T);
	return rho*(2. + del * phideldel(del,tau)/phidel(del,tau));
}


/*------------------------------------------------------------------------------
  REGION 1 DERIVATIVES
*/

#define p S.R1.p
#define T S.R1.T
#define cp freesteam_region1_cp_pT(p,T)
#define v freesteam_region1_v_pT(p,T)
#define s freesteam_region1_s_pT(p,T)
#define alphav freesteam_region1_alphav_pT(p,T)
#define kappaT freesteam_region1_kappaT_pT(p,T)

/**
	TODO convert char to enum for better compiler checking capability
*/
double PT1(char x, SteamState S){
	switch(x){
		case 'p': return 0;
		case 'T': return 1;
		case 'v': return v*alphav;
		case 'u': return cp-p*v*alphav;
		case 'h': return cp;
		case 's': return cp/T;
		case 'g': return -s;
		case 'a':
		case 'f': return -p*v*alphav-s;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid character x = '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
}

double TP1(char x, SteamState S){
	switch(x){
		case 'p': return 1;
		case 'T': return 0;
		case 'v': return -v*kappaT;
		case 'u': return v*(p*kappaT-T*alphav);
		case 'h': return v*(1.-T*alphav);
		case 's': return -v*alphav;
		case 'g': return v;
		case 'a':
		case 'f': return p*v*kappaT;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid character x = '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
}

#undef p
#undef T
#undef cp
#undef v
#undef s
#undef alphap
#undef betap


/*------------------------------------------------------------------------------
  REGION 2 DERIVATIVES
*/

#define p S.R2.p
#define T S.R2.T
#define cp freesteam_region2_cp_pT(p,T)
#define v freesteam_region2_v_pT(p,T)
#define s freesteam_region2_s_pT(p,T)
#define alphav freesteam_region2_alphav_pT(p,T)
#define kappaT freesteam_region2_kappaT_pT(p,T)

/**
	TODO convert char to enum for better compiler checking capability
*/
double PT2(char x, SteamState S){
	switch(x){
		case 'p': return 0;
		case 'T': return 1;
		case 'v': return v*alphav;
		case 'u': return cp-p*v*alphav;
		case 'h': return cp;
		case 's': return cp/T;
		case 'g': return -s;
		case 'a':
		case 'f': return -p*v*alphav-s;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid character x = '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
}

double TP2(char x, SteamState S){
	switch(x){
		case 'p': return 1;
		case 'T': return 0;
		case 'v': return -v*kappaT;
		case 'u': return v*(p*kappaT-T*alphav);
		case 'h': return v*(1.-T*alphav);
		case 's': return -v*alphav;
		case 'g': return v;
		case 'a':
		case 'f': return p*v*kappaT;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid character x = '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
}

#undef p
#undef T
#undef cp
#undef v
#undef s
#undef alphap
#undef betap






