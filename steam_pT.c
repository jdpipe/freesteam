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
#include "steam_pT.h"
#include "region1.h"
#include <stdlib.h>

/**
	This function will never return region 4, because it's not possible
	to sit on the knife of saturation. If you need to set saturated states,
	you should use another function such as freesteam_region1_set_Tx.
*/
SteamState freesteam_set_pT(double p, double T){
	SteamState S;
	if(T < REGION1_TMAX){
		if(p > freesteam_region4_psat_T(T)){
			S.region = 1;
			S.R1.T = T;
			S.R1.p = p;
			return S;
		}else{
			S.region = 2;
			S.R2.T = T;
			S.R2.p = p;
			return S;
		}
	}else{
		S.region = 3;
		S.R3.T = T;
		/* FIXME need to iterate to find rho */
		fprintf(stderr,"Not implemented...\n");
		abort();
	}
}		

