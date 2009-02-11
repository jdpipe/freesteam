%module freesteam

%{
#include "steam.h"
#include "steam_ph.h"
%}

%include "steam.h"
%include "steam_ph.h"

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

	void set_ph(double p, double h){
		SteamState S = freesteam_set_ph(p,h);
		$self->region = S.region;
		$self->R1.p = S.R1.p;
		$self->R1.T = S.R1.T;
	}

	const double p;
	const double h;
	const double u;
	const double v;
	const double s;
	const double cp;
	const double cv;		
};

%{
double SteamState_p_get(SteamState *S){
	fprintf(stderr,"steam region = %d\n");
	return freesteam_p(*S);
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

int SteamState_region_get(SteamState *S){
	return freesteam_region(*S);
}

%}

