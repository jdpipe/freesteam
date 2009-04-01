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

%include "typemaps.i"

%rename(steam_ph) freesteam_set_ph;
%rename(steam_ps) freesteam_set_ps;
%rename(steam_pT) freesteam_set_pT;
%rename(steam_pu) freesteam_set_pu;
%rename(steam_Ts) freesteam_set_Ts;

%rename(bounds_ps) freesteam_bounds_ps;
%rename(bounds_ph) freesteam_bounds_ph;
%rename(bounds_Ts) freesteam_bounds_Ts;

%rename(region_ps) freesteam_region_ps;
%rename(region_ph) freesteam_region_ph;
%rename(region_Ts) freesteam_region_Ts;

%rename(bound_pmax_T) freesteam_bound_pmax_T;

%rename(solver2_region1) freesteam_solver2_region1;
%rename(solver2_region2) freesteam_solver2_region2;
%rename(solver2_region3) freesteam_solver2_region3;
%rename(solver2_region4) freesteam_solver2_region4;

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
%rename(PTRIPLE) IAPWS97_PTRIPLE;

%rename(psat_T) freesteam_region4_psat_T;
%rename(dpsatdT_T) freesteam_region4_dpsatdT_T;
%rename(Tsat_p) freesteam_region4_Tsat_p;
%rename(rhof_T) freesteam_region4_rhof_T;
%rename(rhog_T) freesteam_region4_rhog_T;
%rename(drhofdT_T) freesteam_drhofdT_T;
%rename(drhogdT_T) freesteam_drhogdT_T;
%rename(dpsatdT_T) freesteam_region4_dpsatdT_T;

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
%ignore ipow;

%include "common.h"

%{
#include "steam.h"
#include "steam_ph.h"
#include "steam_ps.h"
#include "steam_pT.h"
#include "steam_pu.h"
#include "steam_Ts.h"
#include "region1.h"
#include "region2.h"
#include "region3.h"
#include "region4.h"
#include "derivs.h"
#include "solver2.h"
#include "bounds.h"
%}

/* TODO clean up region4.h so we can just include it here */
double freesteam_region4_psat_T(double T);
double freesteam_region4_Tsat_p(double p);
double freesteam_region4_rhof_T(double T);
double freesteam_region4_rhog_T(double T);
double freesteam_region4_dpsatdT_T(double T);

%include "steam.h";
%include "steam_ph.h";
%include "steam_ps.h";
%include "steam_pT.h";
%include "steam_pu.h";
%include "steam_Ts.h";
%include "bounds.h";
%include "derivs.h";

/* Rewrite the solver2 functions to include */
%pythoncode %{
def set_transform(im,x):
   a = new_mat44()
   for i in range(4):
       for j in range(4):
           mat44_set(a,i,j,x[i][j])
   _example.set_transform(im,a)
   free_mat44(a)
%}


/* FIXME convert solver2 routines to throw exception instead of return status */
SteamState freesteam_solver2_region3(char X, char Y, double x, double y, SteamState guess, int *OUTPUT);
SteamState freesteam_solver2_region1(char X, char Y, double x, double y, SteamState guess, int *OUTPUT);
SteamState freesteam_solver2_region2(char X, char Y, double x, double y, SteamState guess, int *OUTPUT);
SteamState freesteam_solver2_region4(char X, char Y, double x, double y, SteamState guess, int *OUTPUT);

/* An experiment: make solver2 errors become Python exceptions. */
%pythoncode %{
def solver2_region3(X, Y, x, y, guess):
	S, err = _freesteam.solver2_region3(X,Y,x,y,guess)
	if(err):
		raise ValueError("solver2_region3 returned error %d" % err);
	return S
%}

/* SteamState in python is an object with attributes that are the properties,
  calculated when requested (note: *every time* they are requested) */
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
	const double u;
	const double h;
	const double v;
	const double s;
	const double cp;
	const double cv;	
	const double w;
	const double rho;
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

double SteamState_rho_get(SteamState *S){
	return freesteam_rho(*S);
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
