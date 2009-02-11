#include "steam.h"
#include <stdlib.h>
#include <stdio.h>

double freesteam_T(SteamState S){
	switch(S.region){
		case 1:	
			return S.R1.T;
		case 2:
			return S.R2.T;
		case 3:
			return S.R3.T;
		case 4:
			return S.R4.T;
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_T\n");
			exit(1);
	}
}

double freesteam_p(SteamState S){
	switch(S.region){
		case 1:	
			return S.R1.p;
		case 2:
			return S.R2.p;
		case 3:
			return freesteam_region3_p_rhoT(S.R3.rho, S.R3.T);
		case 4:
			return freesteam_region4_psat_T(S.R4.T);
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_p\n");
			exit(1);
	}
}


double freesteam_v(SteamState S){
	switch(S.region){
		case 1:
			return freesteam_region1_v_pT(S.R1.p,S.R1.T);
		case 2:
			return freesteam_region2_v_pT(S.R2.p,S.R2.T);
		case 3:
			return freesteam_region3_v_rhoT(S.R3.rho,S.R3.T);
		case 4:
			return freesteam_region4_v_px(S.R4.T, S.R4.x);
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_v\n");
			exit(1);
	}
}

double freesteam_u(SteamState S){
	switch(S.region){
		case 1:
			return freesteam_region1_u_pT(S.R1.p, S.R1.T);
		case 2:
			return freesteam_region2_u_pT(S.R2.p, S.R2.T);
		case 3:
			return freesteam_region3_u_rhoT(S.R3.rho,S.R3.T);
		case 4:
			return freesteam_region4_u_Tx(S.R4.T, S.R4.x);
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_u\n");
			exit(1);
	}
}

double freesteam_h(SteamState S){
	switch(S.region){
		case 1:
			return freesteam_region1_h_pT(S.R1.p, S.R1.T);
		case 2:
			return freesteam_region2_h_pT(S.R2.p, S.R2.T);
		case 3:
			return freesteam_region3_h_rhoT(S.R3.rho,S.R3.T);
		case 4:
			return freesteam_region4_h_Tx(S.R4.T, S.R4.x);
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_h\n");
			exit(1);
	}
}


double freesteam_s(SteamState S){
	switch(S.region){
		case 1:
			return freesteam_region1_s_pT(S.R1.p, S.R1.T);
		case 2:
			return freesteam_region2_s_pT(S.R2.p, S.R2.T);
		case 3:
			return freesteam_region3_s_rhoT(S.R3.rho,S.R3.T);
		case 4:
			return freesteam_region4_s_px(S.R4.T, S.R4.x);
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_s\n");
			exit(1);
	}
}

double freesteam_cp(SteamState S){
	switch(S.region){
		case 1:
			return freesteam_region1_cp_pT(S.R1.p, S.R1.T);
		case 2:
			return freesteam_region2_cp_pT(S.R2.p, S.R2.T);
		case 3:
			return freesteam_region3_cp_rhoT(S.R3.rho,S.R3.T);
		case 4:
			return freesteam_region4_cp_px(S.R4.T, S.R4.x);
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_cp\n");
			exit(1);
	}
}
double freesteam_cv(SteamState S){
	switch(S.region){
		case 1:
			return freesteam_region1_cv_pT(S.R1.p, S.R1.T);
		case 2:
			return freesteam_region2_cv_pT(S.R2.p, S.R2.T);
		case 3:
			return freesteam_region3_cv_rhoT(S.R3.rho,S.R3.T);
		case 4:
			return freesteam_region4_cv_px(S.R4.T, S.R4.x);
		default:
			fprintf(stderr,"ERROR: invalid region in freesteam_cv\n");
			exit(1);
	}
}


