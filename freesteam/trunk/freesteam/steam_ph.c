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
		if(h<hf)return 1;
		double hg = freesteam_region2_h_pT(p,Tsat);
		if(h>hg)return 2;
		return 4;
	}
	
	double h13 = freesteam_region1_h_pT(p,REGION1_TMAX);
	if(h <= h13)return 1;

	double T23 = freesteam_b23_T_p(p);
	double h23 = freesteam_region1_h_pT(p,T23);
	if(h >= h23)return 2;

	double psat = freesteam_region3_psat_h(h);
	if(p > psat)return 3;

	return 4;
}

double freesteam_T_ph(double p, double h){
	int r = freesteam_region_ph(p,h);
	switch(r){
		case 1:	
			return freesteam_region1_T_ph(p,h);
		case 2:
			return freesteam_region2_T_ph(p,h);
		case 3:
			return freesteam_region3_T_ph(p,h);
		case 4:
			return freesteam_region4_Tsat_p(p);
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_T_ph\n");
			exit(1);
	}
}

double freesteam_v_ph(double p, double h){
	int r = freesteam_region_ph(p,h);
	double rho,T;
	switch(r){
		case 1:
			T = freesteam_region1_T_ph(p,h);
			return freesteam_region1_v_pT(p,T);
		case 2:
			T = freesteam_region2_T_ph(p,h);
			return freesteam_region2_v_pT(p,T);
		case 3:
			rho = freesteam_region3_rho_ph(p,h);
			T = freesteam_region3_T_ph(p,h);
			return freesteam_region3_v_rhoT(rho,T);
		case 4:
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_T_ph\n");
			exit(1);
	}
}

double freesteam_u_ph(double p, double h){
	int r = freesteam_region_ph(p,h);
	double rho,T;
	switch(r){
		case 1:
			T = freesteam_region1_T_ph(p,h);
			return freesteam_region1_u_pT(p,T);
		case 2:
			T = freesteam_region2_T_ph(p,h);
			return freesteam_region2_u_pT(p,T);
		case 3:
			rho = freesteam_region3_rho_ph(p,h);
			T = freesteam_region3_T_ph(p,h);
			return freesteam_region3_u_rhoT(rho,T);
		case 4:
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_T_ph\n");
			exit(1);
	}
}


double freesteam_s_ph(double p, double h){
	int r = freesteam_region_ph(p,h);
	double rho,T;
	switch(r){
		case 1:
			T = freesteam_region1_T_ph(p,h);
			return freesteam_region1_s_pT(p,T);
		case 2:
			T = freesteam_region2_T_ph(p,h);
			return freesteam_region2_s_pT(p,T);
		case 3:
			rho = freesteam_region3_rho_ph(p,h);
			T = freesteam_region3_T_ph(p,h);
			return freesteam_region3_s_rhoT(rho,T);
		case 4:
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_T_ph\n");
			exit(1);
	}
}

double freesteam_cp_ph(double p, double h){
	int r = freesteam_region_ph(p,h);
	double rho,T;
	switch(r){
		case 1:
			T = freesteam_region1_T_ph(p,h);
			return freesteam_region1_u_pT(p,T);
		case 2:
			T = freesteam_region2_T_ph(p,h);
			return freesteam_region2_u_pT(p,T);
		case 3:
			rho = freesteam_region3_rho_ph(p,h);
			T = freesteam_region3_T_ph(p,h);
			return freesteam_region3_u_rhoT(rho,T);
		case 4:
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_T_ph\n");
			exit(1);
	}
}

double freesteam_cv_ph(double p, double h){
	int r = freesteam_region_ph(p,h);
	double rho,T;
	switch(r){
		case 1:
			T = freesteam_region1_T_ph(p,h);
			return freesteam_region1_cv_pT(p,T);
		case 2:
			T = freesteam_region2_T_ph(p,h);
			return freesteam_region2_cv_pT(p,T);
		case 3:
			rho = freesteam_region3_rho_ph(p,h);
			T = freesteam_region3_T_ph(p,h);
			return freesteam_region3_cv_rhoT(rho,T);
		case 4:
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_T_ph\n");
			exit(1);
	}
}

