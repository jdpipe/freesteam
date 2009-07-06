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
#include "steam_uv.h"

#include "region1.h"
#include "region2.h"
#include "region3.h"
#include "region4.h"
#include "b23.h"
#include "backwards.h"
#include "solver2.h"
#include "zeroin.h"

#include "common.h"

#include <stdio.h>
#include <stdlib.h>

/* required helper functions */

static double uf_p(double p){
	Tsat = freesteam_region4_Tsat_p(p);
	if(T < T13){
		return freesteam_region1_u_pT(p, Tsat);
	}else{
		/* region 3 part of the saturation line */
	}
}

static double ug_p(double p){
	Tsat = freesteam_region4_Tsat_p(p);
	if(T < T13){
		return freesteam_region2_u_pT(p, Tsat);
	}else{
		/* region 3 part of the saturation line */
	}
}

static double vf_u(double u){
	/* zeroin for T such that uf(T) = u */	
	return freesteam_region1_v_pT(psat(T), T);
}

static double ug_v(double v){
	/* zeroin for T such that vg(T) = v */
	return freesteam_region2_u_pT(psat(T), T)
}

static double u13_v(double v){
	/* iterate on p to find v_pT(p, T13) = v */
	return u_pT(p, T13)
}

static double u23_v(double v){
	/* iterate on T to get freesteam_region2_v_pT(p23(T), T) = v */
	return freesteam_region2_u_pT(p, T);

	/* is there a better way using region 3 (rho, T) ?? */
}

int freesteam_bounds_uv(double u, double v, int verbose){

	/* if u < uf(pmin) */
	/*     out of bounds */

	/* else if u < ug(pmin) */
	/*     check v < v(pmin, u) region 4 */

	/* else if u > ug(pmin) */
	/*     check v > v(pmin, u) region 2 */
	
	if(u < freesteam_region2_u_pT(PMAX, TMAX)){
	/*     check v > v(pmax, u) region 1, 3, or 2! */

	}/* else if u > u(pmax, Tmax) */
	/*     check u < u(Tmax, v) */
}

int freesteam_region_uv(double u, double v){
	
	/* if u < u_crit */
	/*     if v > vf(u) */
	/*          region 4 */
	/*     else if u > u13(v) */
	/*          region 3 */
    /*     else */
	/*          region 1 */

	/* if u < ug(v) */
	/*     region 4 */
	/* else if v > v_234 || u > u_23(v) */
    /*     region 2 */
	/* else */
    /*     region 3 */

}

SteamState freesteam_set_uv(double u, double v){

	int region = /* work out the region */

	if(region == 1){
		double vf = /* solved Tsat(u) */
		/* iterate on p, T to solve u, v */
	}else if(region == 2){
		/* two-way iteration with p,T to solve u,v */
	}else if(region == 3){
		/* one-way iteration on T to solve v(rho,T). */
	}else if(region == 4){
		/* initial guess ? */
		/* iterate on T, x to solve u, v */
	}
}

