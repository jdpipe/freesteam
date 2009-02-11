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

#include "steam_ph.h"

#include "region1.h"
#include "region2.h"
#include "region3.h"
#include "region4.h"
#include "b23.h"

#include "common.h"

#include <stdio.h>
#include <stdlib.h>

int freesteam_region_ph(double p, double h){
	SteamState S = freesteam_set_ph(p, h);
	return S.region;
	/* Note: there are some optimisations possible here due to calculations
	needed to fully specify S.R1, S.R2, etc. */
}

SteamState freesteam_set_ph(double p, double h){
	SteamState S;

	/* this function is structurally the same as region_ph but we store
	the useful intermediate calculation results. */

	/* give warnings about outer limits */
	double hmax = freesteam_region2_h_pT(p,REGION2_TMAX);
	if(h>hmax){
		fprintf(stderr,"WARNING: freesteam_region_ph: h > hmax\n");
	}
	if(p > IAPWS97_PMAX){
		fprintf(stderr,"WARNING: freesteam_region_ph: p > pmax\n");
	}
	if(p <= 0){
		fprintf(stderr,"WARNING: freesteam_region_ph: p <= 0\n");
	}
	double hmin = freesteam_region1_h_pT(p,IAPWS97_TMIN);
	if(h < hmin){
		fprintf(stderr,"WARNING: freesteam_region_ph: h < hmin\n");
	}

	double p13 = freesteam_region4_psat_T(REGION1_TMAX);

	if(p <= p13){
		double Tsat = freesteam_region4_Tsat_p(p);
		double hf = freesteam_region1_h_pT(p,Tsat);
		if(h<hf){
			S.region = 1;
			S.R1.p = p;
			S.R1.T = freesteam_region1_T_ph(p, h);
			return S;
		}
		double hg = freesteam_region2_h_pT(p,Tsat);
		if(h>hg){
			S.region = 2;
			S.R2.p = p;
			S.R2.T = freesteam_region2_T_ph(p, h);
			return S;
		}
		/* this is the low-pressure portion of region 4 */
		S.region = 4;
		S.R4.T = freesteam_region4_Tsat_p(p);
		/* TODO iteratively improve estimate of T */
		S.R4.x = (h - hg)/(hf - hg);
		return S;
	}
	
	double h13 = freesteam_region1_h_pT(p,REGION1_TMAX);
	if(h <= h13){
		S.region = 1;
		S.R1.p = p;
		S.R1.T = freesteam_region1_T_ph(p, h);
		return S;
	}

	double T23 = freesteam_b23_T_p(p);
	double h23 = freesteam_region1_h_pT(p,T23);
	if(h >= h23){
		S.region = 2;
		S.R2.p = p;
		S.R2.T = freesteam_region2_T_ph(p, h);
		return S;
	}

	double psat = freesteam_region3_psat_h(h);
	if(p > psat){
		S.region = 3;
		S.R3.rho = 1./freesteam_region3_v_ph(p, h);
		S.R3.T = freesteam_region3_T_ph(p, h);
		return S;
	}

	/* high-pressure portion of region 4 */
	S.region = 4;
	double Tsat = freesteam_region4_Tsat_p(psat);
	/* FIXME iteratively improve this estimate of Tsat */
	S.R4.T = Tsat;
	double rhof = freesteam_region4_rhof_T(Tsat);
	double rhog = freesteam_region4_rhog_T(Tsat);
	double hf = freesteam_region3_h_rhoT(rhof,Tsat);
	double hg = freesteam_region2_h_rhoT(rhog,Tsat);
	S.R4.x = (h - hf)/(hg - hf);
	return S;
}

