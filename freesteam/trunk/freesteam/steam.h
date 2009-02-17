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
#ifndef FREESTEAM_STEAM_H
#define FREESTEAM_STEAM_H
#include <stdio.h>

typedef struct SteamState_R1_struct{
	double p, T;
} SteamState_R1;

typedef struct SteamState_R2_struct{
	double p, T;
} SteamState_R2;

typedef struct SteamState_R3_struct{
	double rho, T;
} SteamState_R3;

typedef struct SteamState_R4_struct{
	double T, x;
} SteamState_R4;

typedef struct SteamState_struct{
	char region;
	union{
		SteamState_R1 R1;
		SteamState_R2 R2;
		SteamState_R3 R3;
		SteamState_R4 R4;
	};			
} SteamState;

int freesteam_region(SteamState S);

SteamState freesteam_region1_set_pT(double p, double T);
SteamState freesteam_region2_set_pT(double p, double T);
SteamState freesteam_region3_set_rhoT(double rho, double T);
SteamState freesteam_region4_set_Tx(double T, double x);

int freesteam_fprint(FILE *f, SteamState S);

#if 0
# define FREESTEAM_DEBUG(NAME,STATE)\
	fprintf(stderr,"%s (%s:%d): %s ",__func__,__FILE__,__LINE__,NAME);\
	freesteam_fprint(stderr,S);
#else
# define FREESTEAM_DEBUG(NAME,STATE)
#endif

double freesteam_p(SteamState S);
double freesteam_T(SteamState S);
double freesteam_v(SteamState S);
double freesteam_u(SteamState S);
double freesteam_h(SteamState S);
double freesteam_s(SteamState S);
double freesteam_cp(SteamState S);
double freesteam_cv(SteamState S);
double freesteam_w(SteamState S);

#endif


