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
%define DOCSTRING
"The freesteam module provides the ability to calculate
properties of water and steam from the IAPWS-IF97 steam
property correlations, published by the Internation
Association for the Properties of Water & Steam (IAPWS).

Freesteam is based on the IAPWS-IF97 releases, but is not
officialy endorsed by IAPWS. Freesteam is written by
John Pye. Freesteam is released under the GPL license. 
You may not use it in commercially-released software."
%enddef

%feature("autodoc", "1");

%module(docstring=DOCSTRING) freesteam

%rename(steam_ph) freesteam_set_ph;
%rename(region1_pT) freesteam_region1_set_pT;
%rename(region2_pT) freesteam_region2_set_pT;

%rename(region3_rhoT) freesteam_region3_set_rhoT;
%rename(region4_Tx) freesteam_region4_set_Tx;

%rename(PCRIT) IAPWS97_PCRIT;
%rename(TCRIT) IAPWS97_TCRIT;
%rename(TMIN) IAPWS97_TMIN;
%rename(RHOCRIT) IAPWS97_RHOCRIT;
%rename(PMAX) IAPWS97_PMAX;
%rename(R) IAPWS97_R;

%rename(psat_T) freesteam_region4_psat_T;
%rename(Tsat_p) freesteam_region4_Tsat_p;
%rename(rhof_T) freesteam_region4_rhof_T;
%rename(rhog_T) freesteam_region4_rhog_T;

%ignore SteamState;
%ignore freesteam_T;
%ignore freesteam_p;
%ignore freesteam_h;
%ignore freesteam_u;
%ignore freesteam_v;
%ignore freesteam_s;
%ignore freesteam_cp;
%ignore freesteam_cv;
%ignore freesteam_w;
%ignore freesteam_fprint;
%ignore freesteam_region;
%ignore freesteam_region_ph;
%ignore ipow;

%include "common.h"

%{
#include "steam.h"
#include "steam_ph.h"
#include "region1.h"
#include "region2.h"
#include "region3.h"
#include "region4.h"
%}

/* TODO clean up region4.h so we can just include it here */
double freesteam_region4_psat_T(double T);
double freesteam_region4_Tsat_p(double p);
double freesteam_region4_rhof_T(double T);
double freesteam_region4_rhog_T(double T);

%include "steam.h";
%include "steam_ph.h";

%extend SteamState{
	SteamState(){
		SteamState *S;
		S = (SteamState *)malloc(sizeof(SteamState));
		S->region = 0;
		S->R1.T = 0;
		S->R1.p = 0;
		return S;
	}

	~SteamState(){
		free($self);
	}
	
	%immutable;
	const double p;
	const double T;
	const double h;
	const double v;
	const double s;
	const double cp;
	const double cv;	
	const double w;
};

%{
double SteamState_p_get(SteamState *S){
	return freesteam_p(*S);
}

double SteamState_T_get(SteamState *S){
	return freesteam_T(*S);
}

double SteamState_h_get(SteamState *S){
	return freesteam_h(*S);
}

double SteamState_u_get(SteamState *S){
	return freesteam_u(*S);
}

double SteamState_v_get(SteamState *S){
	return freesteam_v(*S);
}

double SteamState_s_get(SteamState *S){
	return freesteam_s(*S);
}

double SteamState_cp_get(SteamState *S){
	return freesteam_cp(*S);
}

double SteamState_cv_get(SteamState *S){
	return freesteam_cv(*S);
}

double SteamState_w_get(SteamState *S){
	return freesteam_w(*S);
}


%}
