%define DOCSTRING
"The freesteam module provides the ability to calculate
properties of water and steam from the IAPWS-IF97 steam
property correlations, published by the Internation
Association for the Properties of Water & Steam (IAPWS).

Freesteam is based on the IAPWS-IF97 releases, but is not
official endorsed by IAPWS. Freesteam is written by
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
	const double u;
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
