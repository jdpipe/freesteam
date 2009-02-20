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

#include "region1.h"
#include "region2.h"
#include "region3.h"

#include <stdlib.h>

/* forward decls */

/*
The following functions basically look up the contents of Table 1 and Table 2
from the IAPWS Guideline.
*/

typedef double PartialDerivFn(char,SteamState);

/*
The following are the functions

	 ⎰ ∂x ⎱   ___\   VTn
	 ⎱ ∂v ⎰T     /

etc., for each of the different regions (n).
*/
static PartialDerivFn VT3, TV3, PT1, TP1, PT2, TP2;

/*------------------------------------------------------------------------------
  EXPORTED FUNCTION(S)
*/

/**
	Calculates the derivative 

	 ⎰ ∂z ⎱
	 ⎱ ∂x ⎰y

	@param S steam state, already calculated using steam_ph, etc.
	@param x in above equation, character one of 'pTvuhsgaf'.
	@param y in above equation, character one of pTvuhsgaf.
	@param z in above equation, character one of pTvuhsgaf.
	Note that Helmholtz free energy can be signified by either 'a' or 'f'.

	@NOTE that the variables ARE NOT IN ALPHABETICAL ORDER.

	@return the numerical value of the derivative (∂z/∂x)y.
*/
double freesteam_deriv(SteamState S, char z, char x, char y){
	PartialDerivFn *AB, *BA;

	fprintf(stderr,"CALCULATING (∂%c/∂%c)%c... ",z,x,y);

	freesteam_fprint(stderr,S);
	switch(S.region){
		case 1:	AB = PT1; BA = TP1; break;
		case 2: AB = PT2, BA = TP2; break;
		case 3: AB = VT3; BA = TV3; break;
		case 4:
		default:
			fprintf(stderr,"ERROR: %s (%s:%d) Invalid or not-implemented region '%d'\n"
				,__func__,__FILE__,__LINE__,S.region
			);
			exit(1);
	}
	double ZAB = (*AB)(z,S);
	double ZBA = (*BA)(z,S);
	double XAB = (*AB)(x,S);
	double XBA = (*BA)(x,S);
	double YAB = (*AB)(y,S);
	double YBA = (*BA)(y,S);
	return ((ZAB*YBA-ZBA*YAB)/(XAB*YBA-XBA*YAB));
}

/*------------------------------------------------------------------------------
  REGION 3 DERIVATIVES
*/

/*
FIXME the following macros avoid calculating unneeded results eg within VT3 
but at the level of freesteam_deriv, there is wasted effort, because eg 'p' 
will be calculated several times in different calls to VT3.
*/
#define rho S.R3.rho
#define T S.R3.T
#define p freesteam_region3_p_rhoT(rho,T)
#define cv freesteam_region3_cv_rhoT(rho,T)
#define v (1./rho)
#define s freesteam_region3_s_rhoT(rho,T)
#define alphap freesteam_region3_alphap_rhoT(rho,T)
#define betap freesteam_region3_betap_rhoT(rho,T)

/**
	TODO convert char to enum for better compiler checking capability
*/
double VT3(char x, SteamState S){
	double res;
	switch(x){
		case 'p': res = -p*betap; break;
		case 'T': res = 0; break;
		case 'v': res = 1; break;
		case 'u': res = p*(T*alphap-1.); break;
		case 'h': res = p*(T*alphap-v*betap); break;
		case 's': res = p*alphap; break;
		case 'g': res = -p*v*betap; break;
		case 'a':
		case 'f': res = -p; break;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid variable '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
	fprintf(stderr,"(∂%c/∂v)T = %f\n",x,res);
	return res;
}

double TV3(char x, SteamState S){
	double res;
	switch(x){
		case 'p': res = p*alphap; break;
		case 'T': res = 1; break;
		case 'v': res = 0; break;
		case 'u': res = cv; break;
		case 'h': res = cv + p*v*alphap; break;
		case 's': res = cv/alphap; break;
		case 'g': res = p*v*alphap - s; break;
		case 'a':
		case 'f': res = -s; break;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid variable '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
	fprintf(stderr,"(∂%c/∂T)v = %f\n",x,res);
	return res;
}

#undef rho
#undef T
#undef p
#undef cv
#undef v
#undef s
#undef alphap
#undef betap

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
double TP1(char x, SteamState S){
	double res;
	switch(x){
		case 'p': res = 0; break;
		case 'T': res = 1; break;
		case 'v': res = v*alphav; break;
		case 'u': res = cp-p*v*alphav; break;
		case 'h': res = cp; break;
		case 's': res = cp/T; break;
		case 'g': res = -s; break;
		case 'a':
		case 'f': res = -p*v*alphav-s; break;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid character x = '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	fprintf(stderr,"(∂%c/∂T)p = %g\n",x,res);
	return res;
	}
}

double PT1(char x, SteamState S){
	double res;
	switch(x){
		case 'p': res = 1; break;
		case 'T': res = 0; break;
		case 'v': res = -v*kappaT; break;
		case 'u': res = v*(p*kappaT-T*alphav); break;
		case 'h': res = v*(1.-T*alphav); break;
		case 's': res = -v*alphav; break;
		case 'g': res = v; break;
		case 'a':
		case 'f': res = p*v*kappaT; break;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid character x = '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
	fprintf(stderr,"(∂%c/∂p)T = %g\n",x,res);
	return res;
}

#undef p
#undef T
#undef cp
#undef v
#undef s
#undef alphav
#undef kappaT


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
double TP2(char x, SteamState S){
	double res;
	switch(x){
		case 'p': res = 0; break;
		case 'T': res = 1; break;
		case 'v': res = v*alphav; break;
		case 'u': res = cp-p*v*alphav; break;
		case 'h': res = cp; break;
		case 's': res = cp/T; break;
		case 'g': res = -s; break;
		case 'a':
		case 'f': res = -p*v*alphav-s; break;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid character x = '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
	fprintf(stderr,"(∂%c/∂T)p = %g\n",x,res);
	if(x=='v'){
		fprintf(stderr,"(∂ρ/∂T)p = %g\n",-1/SQ(v)*res);
	}
	return res;
}

double PT2(char x, SteamState S){
	double res;
	switch(x){
		case 'p': res = 1; break;
		case 'T': res = 0; break;
		case 'v': res = -v*kappaT; break;
		case 'u': res = v*(p*kappaT-T*alphav); break;
		case 'h': res = v*(1.-T*alphav); break;
		case 's': res = -v*alphav; break;
		case 'g': res = v; break;
		case 'a':
		case 'f': res = p*v*kappaT; break;
		default:
			fprintf(stderr,"%s (%s:%d): Invalid character x = '%c'\n", __func__,__FILE__,__LINE__,x);
			exit(1);
	}
	fprintf(stderr,"(∂%c/∂p)T = %g\n",x,res);
	if(x=='v'){
		fprintf(stderr,"(∂ρ/∂p)T = %g\n",-1/SQ(v)*res);
	}
	return res;
}

#undef p
#undef T
#undef cp
#undef v
#undef s
#undef alphav
#undef kappaT

