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
#include "steam_ph.h"

#include "region1.h"
#include "region2.h"
#include "region3.h"
#include "region4.h"
#include "b23.h"
#include "backwards.h"

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

	//fprintf(stderr,"freesteam_set_ph(p = %f, h = %f)\n",p,h);

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

	double p13 = 0;
	p13 = freesteam_region4_psat_T(REGION1_TMAX);

	//fprintf(stderr,"p13 = %lf MPa\n",p13/1.e6);
	//fprintf(stderr,"check: %f\n",freesteam_region4_psat_T(REGION1_TMAX));

	if(p <= p13){
		double Tsat = freesteam_region4_Tsat_p(p);
		double hf = freesteam_region1_h_pT(p,Tsat);
		if(h<hf){
			S.region = 1;
			S.R1.p = p;
			S.R1.T = freesteam_region1_T_ph(p, h);
			FREESTEAM_DEBUG("(h < hf)",S);
			return S;
		}
		double hg = freesteam_region2_h_pT(p,Tsat);
		if(h>hg){
			S.region = 2;
			S.R2.p = p;
			S.R2.T = freesteam_region2_T_ph(p, h);
			FREESTEAM_DEBUG("(h > hg)",S);
			return S;
		}
		/* this is the low-pressure portion of region 4 */
		S.region = 4;
		S.R4.T = freesteam_region4_Tsat_p(p);
		/* TODO iteratively improve estimate of T */
		S.R4.x = (h - hf)/(hg - hf);
		FREESTEAM_DEBUG("(hf < h < hg)",S);
		return S;
	}

	double h13 = freesteam_region1_h_pT(p,REGION1_TMAX);
	if(h <= h13){
		S.region = 1;
		S.R1.p = p;
		S.R1.T = freesteam_region1_T_ph(p, h);
		FREESTEAM_DEBUG("(h <= h(p,T1max)",S);
		return S;
	}

	double T23 = freesteam_b23_T_p(p);
	//fprintf(stderr,"p = %f MPa --> T23(p) = %f K (%f °C)\n",p/1e6,T23,T23-273.15);
	double h23 = freesteam_region2_h_pT(p,T23);
	if(h >= h23){
		S.region = 2;
		S.R2.p = p;
		S.R2.T = freesteam_region2_T_ph(p, h);
		//fprintf(stderr,"T23(p) = %f, h23(p,T23) = %f\n",T23,h23);
		FREESTEAM_DEBUG("(high pressure region 2, h > h2(p,Tb23(p))",S);
		//fprintf(stderr,"--> h = %f kJ/kg\n",freesteam_region2_h_pT(S.R2.p,S.R2.T)/1e3);
		return S;
	}

	double psat = freesteam_region3_psat_h(h);
	if(p > psat){
		S.region = 3;
		S.R3.rho = 1./freesteam_region3_v_ph(p, h);
		S.R3.T = freesteam_region3_T_ph(p, h);
		FREESTEAM_DEBUG("(p > psat(h))",S);
		return S;
	}

	/* high-pressure portion of region 4 */
	S.region = 4;
	double Tsat = freesteam_region4_Tsat_p(p);
	/* FIXME iteratively improve this estimate of Tsat */
	S.R4.T = Tsat;
	double rhof = freesteam_region4_rhof_T(Tsat);
	double rhog = freesteam_region4_rhog_T(Tsat);
	/* FIXME iteratively improve these estimates of rhof, rhog */
	double hf = freesteam_region3_h_rhoT(rhof,Tsat);
	double hg = freesteam_region3_h_rhoT(rhog,Tsat);

	//fprintf(stderr,"T = %f, rhof = %f, rhog = %f (vf = %f, vg = %f), hf = %f, hg = %f\n", Tsat, rhof, rhog, 1./rhof, 1./rhog, hf, hg);
	S.R4.x = (h - hf)/(hg - hf);
	FREESTEAM_DEBUG("(high-pressure)",S);
	return S;
}

