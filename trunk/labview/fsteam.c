#include "fsteam.h"
#include "freesteam/steam.h"
#include "freesteam/steam_pT.h"

double density(double TK, double pPa) {

	SteamState S;
	double rho;
	
	S = freesteam_set_pT(pPa, TK);
	rho = freesteam_rho(S);
			
	return rho;
	
}

double enthalpy(double TK, double pPa) {

	SteamState S;
	double h;
	
	S = freesteam_set_pT(pPa, TK);
	h = freesteam_h(S);
			
	return h;
	
}

double entropy(double TK, double pPa) {

	SteamState S;
	double s;
	
	S = freesteam_set_pT(pPa, TK);
	s = freesteam_s(S);
			
	return s;
	
}

double internal_energy(double TK, double pPa) {

	SteamState S;
	double u;
	
	S = freesteam_set_pT(pPa, TK);
	u = freesteam_u(S);
			
	return u;
	
}

double specific_volume(double TK, double pPa) {

	SteamState S;
	double v;

	S = freesteam_set_pT(pPa, TK);
	v = freesteam_v(S);

	return v;

}

double isobaric_heat_capacity(double TK, double pPa) {

	SteamState S;
	double cp;

	S = freesteam_set_pT(pPa, TK);
	cp = freesteam_cp(S);

	return cp;

}

double isochoric_heat_capacity(double TK, double pPa) {

	SteamState S;
	double cv;

	S = freesteam_set_pT(pPa, TK);
	cv = freesteam_cv(S);

	return cv;

}

double speed_of_sound(double TK, double pPa) {

	SteamState S;
	double w;

	S = freesteam_set_pT(pPa, TK);
	w = freesteam_w(S);

	return w;

}

double saturated_quality(double TK, double pPa) {

	SteamState S;
	double x;

	S = freesteam_set_pT(pPa, TK);
	x = freesteam_x(S);

	return x;

}

double dynamic_viscosity(double TK, double pPa) {

	SteamState S;
	double mu;

	S = freesteam_set_pT(pPa, TK);
	mu = freesteam_mu(S);

	return mu;

}

double thermal_conductivity(double TK, double pPa) {

	SteamState S;
	double k;

	S = freesteam_set_pT(pPa, TK);
	k = freesteam_k(S);

	return k;

}
