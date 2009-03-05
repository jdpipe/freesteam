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
#include "steam_ps.h"

#include "region1.h"
#include "region2.h"
#include "region3.h"
#include "region4.h"
#include "zeroin.h"
#include "b23.h"

#include <stdlib.h>

/* to solve within a region:
	- two property values A=a, B=b
	- a first guess for the solution
	- function that evaluates A and B given the guess
		- if one of the variables is one of the standard vars for that region
		  use single variable iteration between upper/lower limit.
		- if neither match, use two-var iteration.
	- derivative function or estimate
	- convergence test or tolerance
	- return the SteamState
*/

int freesteam_region_ps(double p, double s){
	// FIXME add test/warning for max S

	// FIXME check p > min p, p < max p, s > 0

	if(p <= freesteam_region4_psat_T(REGION1_TMAX)){
		double T = freesteam_region4_Tsat_p(p);
		double sf = freesteam_region1_s_pT(p,T);
		double sg = freesteam_region2_s_pT(p,T);
		if(s <= sf){
			return 1;
		}
		if(s >= sg){
			return 2;
		}
		return 4;
	}

	if(p < IAPWS97_PCRIT){ /* but not in region 1/2 */
		double T = freesteam_region4_Tsat_p(p);
		double rhof = freesteam_region4_rhof_T(T);
		double rhog = freesteam_region4_rhog_T(T);
		double sf = freesteam_region3_s_rhoT(rhof,T);
		double sg = freesteam_region3_s_rhoT(rhog,T);
		if(s <= sf || s >= sg)return 3;
		return 4;
	}

	/* FIXME solve for s_b13(p) */
	double s_b13 = freesteam_region1_s_pT(p, REGION1_TMAX);

	if(s <= s_b13){
		return 1;
	}

	/* FIXME solve for s_b23(p) */
	double T_b23 = freesteam_b23_T_p(p);
	double s_b23 = freesteam_region2_s_pT(p,T_b23);
	if(s >= s_b23){
		return 2;
	}

	return 3;
}

typedef struct{
	double p, s, T;
} SolvePSData;

#define D ((SolvePSData *)user_data)
static ZeroInSubjectFunction ps_region1_fn, ps_region2_fn, ps_region4_fn;
double ps_region1_fn(double T, void *user_data){
	return D->s - freesteam_region1_s_pT(D->p, T);
}
double ps_region2_fn(double T, void *user_data){
	return D->s - freesteam_region2_s_pT(D->p, T);
}
double ps_region4_fn(double x, void *user_data){
	double T = freesteam_region4_Tsat_p(D->p);
	return D->s - freesteam_region4_s_Tx(T, x);
}
#undef D

SteamState freesteam_set_ps(double p, double s){
	double lb, ub, tol, sol, err;
	SolvePSData D = {p, s, 0.};

	int region = freesteam_region_ps(p,s);
	switch(region){
		case 1:
			lb = IAPWS97_TMIN;
			ub = REGION1_TMAX;
			tol = 1e-9; /* ??? */
			zeroin_solve(&ps_region1_fn, &D, lb, ub, tol, &sol, &err);
			return freesteam_region1_set_pT(p,sol);
		case 2:
			lb = IAPWS97_TMIN;
			ub = REGION2_TMAX;
			tol = 1e-9; /* ??? */
			zeroin_solve(&ps_region2_fn, &D, lb, ub, tol, &sol, &err);
			return freesteam_region2_set_pT(p,sol);
		case 4:
			lb = 0.;
			ub = 1.;
			tol = 1e-9; /* ??? */
			zeroin_solve(&ps_region4_fn, &D, lb, ub, tol, &sol, &err);
			return freesteam_region4_set_Tx(D.T,sol);
		case 3:
		default:
			/* ??? */
			fprintf(stderr,"%s (%s:%d): Region '%d' not implemented\n",__func__,__FILE__,__LINE__,region);
			exit(1);
	}
}


