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

#include "steam.h"
#include "steam_ph.h"
#include "region4.h"
#include "backwards.h"
#include "b23.h"
#include "derivs.h"
#include "zeroin.h"
#include "region3.h"
#include "solver2.h"
#include "steam_ps.h"
#include "steam_Ts.h"
#include "steam_pT.h"
#include "steam_pv.h"
#include "region1.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int errorflag = 0;
double maxrelerr = 0;
int verbose = 0;

#define CHECK_VAL(EXPR, VAL, RELTOL){ \
	double calc = (EXPR); \
	double error = calc - (VAL);\
	double relerr;\
	relerr = (VAL==0) ? error : error / (VAL);\
	if(fabs(relerr)>maxrelerr)maxrelerr=fabs(relerr);\
	if(fabs(relerr) > RELTOL){\
		fprintf(stderr,"ERROR (%s:%d): %s = %e, should be %e, error %10e %% exceeds tol %10e %%\n",\
			__FILE__,__LINE__,#EXPR, calc, (VAL), relerr*100., RELTOL*100.\
		);\
		errorflag = 1; \
	 }else if(verbose){ \
		fprintf(stderr,"OK: %s = %f with %e %% error (test value = %f).\n", #EXPR, calc, error/(VAL)*100,(VAL)); \
	} \
}

#define CHECK_INT(EXPR, VAL){ \
	int calc = (EXPR); \
	if(calc != (VAL)){\
		fprintf(stderr,"ERROR (%s:%d): %s = %d, should be %d!\n",\
			__FILE__,__LINE__,#EXPR, calc, (VAL));\
		errorflag = 1; \
	 }else if(verbose){ \
		fprintf(stderr,"OK: %s = %d\n", #EXPR, (VAL)); \
	} \
}

/*------------------------------------------------------------------------------
  REGION 1: FORWARDS
*/

#define RELTOL 5e-9

void test_region1_point(double T,double p, double v,double h,double u, double s, double cp, double w){

/*
	Note: inputs to this function need units conversion!
		Temperature temp;    ///< K
		Pressure pres;       ///< MPa
		SpecificVolume v;    ///< m³/kg
		SpecificEnergy h;    ///< enthalpy / kJ/kg
		SpecificEnergy u;    ///< internal energy / kJ/kg
		SpecificEntropy s;   ///< kJ/kg.K
		SpecHeatCap cp;      ///< specific heat capacity at constant pressure
		Velocity w;          ///< speed of sound
*/

	SteamState S = freesteam_region1_set_pT(p*1e6, T);
	CHECK_VAL(freesteam_p(S),p*1e6,RELTOL);
	CHECK_VAL(freesteam_T(S),T,RELTOL);
	CHECK_VAL(freesteam_v(S),v,RELTOL);
	CHECK_VAL(freesteam_h(S),h*1e3,RELTOL);
	CHECK_VAL(freesteam_u(S),u*1e3,RELTOL);
	CHECK_VAL(freesteam_s(S),s*1e3,RELTOL);
	CHECK_VAL(freesteam_cp(S),cp*1e3,RELTOL);
	CHECK_VAL(freesteam_w(S),w,RELTOL);

}

void testregion1(void){
	fprintf(stderr,"REGION 1 TESTS\n");
	test_region1_point(300., 3., 0.100215168E-2, 0.115331273E3, 0.112324818E3, 0.392294792, 0.417301218E1, 0.150773921E4);
	test_region1_point(300., 80., 0.971180894E-3, 0.184142828E3, 0.106448356E3, 0.368563852, 0.401008987E1, 0.163469054E4);
	test_region1_point(500., 3., 0.120241800E-2, 0.975542239E3, 0.971934985E3, 0.258041912E1, 0.465580682E1, 0.124071337E4);
}

/*------------------------------------------------------------------------------
  REGION 2: FORWARDS
*/

void test_region2_point(double T,double p, double v,double h,double u, double s, double cp, double w){

	/* units of measurement as for region1 test */

	SteamState S = freesteam_region2_set_pT(p*1e6, T);
	CHECK_VAL(freesteam_p(S),p*1e6,RELTOL);
	CHECK_VAL(freesteam_T(S),T,RELTOL);
	CHECK_VAL(freesteam_v(S),v,RELTOL);
	CHECK_VAL(freesteam_h(S),h*1e3,RELTOL);
	CHECK_VAL(freesteam_u(S),u*1e3,RELTOL);
	CHECK_VAL(freesteam_s(S),s*1e3,RELTOL);
	CHECK_VAL(freesteam_cp(S),cp*1e3,RELTOL);
	CHECK_VAL(freesteam_w(S),w,RELTOL);
}

void testregion2(void){
	fprintf(stderr,"REGION 2 TESTS\n");
	test_region2_point(300., 0.0035, 0.394913866E2, 0.254991145E4, 0.241169160E4, 0.852238967E1, 0.191300162E1, 0.427920172E3);
	test_region2_point(700., 0.0035, 0.923015898E2, 0.333568375E4, 0.301262819E4, 0.101749996E2, 0.208141274E1, 0.644289068E3);
	test_region2_point(700., 30., 0.542946619E-02, 0.263149474E+4, 0.246861076E+4, 0.517540298E+1, 0.103505092E+2, 0.480386523E+3);
}

/*------------------------------------------------------------------------------
  REGION 3: FORWARDS
*/

void test_region3_point(double T,double rho, double p,double h,double u, double s, double cp, double w){

	/* units of measurement as for region1 test */

	SteamState S = freesteam_region3_set_rhoT(rho, T);
	CHECK_VAL(freesteam_p(S),p*1e6,RELTOL);
	CHECK_VAL(freesteam_T(S),T,RELTOL);
	CHECK_VAL(freesteam_v(S),1./rho,RELTOL);
	CHECK_VAL(freesteam_h(S),h*1e3,RELTOL);
	CHECK_VAL(freesteam_u(S),u*1e3,RELTOL);
	CHECK_VAL(freesteam_s(S),s*1e3,RELTOL);
	CHECK_VAL(freesteam_cp(S),cp*1e3,RELTOL);
	/* CHECK_VAL(freesteam_w(S),w,RELTOL); */
}

void testregion3(void){
	fprintf(stderr,"REGION 3 TESTS\n");

	test_region3_point(650., 500., 0.255837018E2,
			           0.186343019E4, 0.181226279E4, 0.405427273E1,
			           0.138935717E2, 0.502005554E3);

	test_region3_point(650., 200., 0.222930643E2,
			           0.237512401E4, 0.226365868E4, 0.485438792E1,
			           0.446579342E2, 0.383444594E3);

	test_region3_point(750., 500., 0.783095639E2, 0.225868845E4,
			           0.210206932E4, 0.446971906E1, 0.634165359E1,
			           0.760696041E3);
}

/*------------------------------------------------------------------------------
  REGION 4 SATURATION LINE
*/

void test_region4_point(double T,double p){
	SteamState S = freesteam_region4_set_Tx(T,0.);
	double p1 = freesteam_p(S);
	CHECK_VAL(p1,p*1e6,RELTOL);
	double T1 = freesteam_region4_Tsat_p(p1);
	CHECK_VAL(T1,T,RELTOL);
}

void testregion4(void){
	fprintf(stderr,"REGION 4 TESTS\n");
	test_region4_point(300.,	0.353658941E-2);
	test_region4_point(500.,	0.263889776E1);
	test_region4_point(600.,	0.123443146E2);
}

/*------------------------------------------------------------------------------
  REGION 1 BACKWARDS (P,H)
*/

void test_region1_ph_point(double p,double h, double T){
	double T1 = freesteam_region1_T_ph(p*1e6,h*1e3);
	CHECK_VAL(T1,T,RELTOL);
}

void testregion1ph(void){
	fprintf(stderr,"REGION 1 (P,H) TESTS\n");
	test_region1_ph_point(3.,	500.,	0.391798509e3);
	test_region1_ph_point(80.,	500.,	0.378108626e3);
	test_region1_ph_point(80.,	1500.,	0.611041229e3);
}

#if 0
void test_region1_ps_point(double p,double s, double T){
	double T1 = freesteam_region1_T_ps(p*1e6,s*1e3);
	CHECK_VAL(T1,T,RELTOL);
}

void testregion1ps(void){
	fprintf(stderr,"REGION 1 (P,S) TESTS\n");
	test_region1_ps_point(3.,	0.5.,  0.307842258e3);
	test_region1_ps_point(80.,	0.5.,  0.309979785e3);
	test_region1_ps_point(80.,	3.,    0.565899909E3);
}
#endif


/*------------------------------------------------------------------------------
  REGION 2 BACKWARDS (P,H)
*/

void test_region2_ph_point(double p,double h, double T){
	double T1 = freesteam_region2_T_ph(p*1e6,h*1e3);
	CHECK_VAL(T1,T,RELTOL);
}

void testregion2ph(void){
	fprintf(stderr,"REGION 2 (P,H) TESTS\n");
	test_region2_ph_point(0.001,3000.,	0.534433241e3);
	test_region2_ph_point(3.,	3000.,	0.575373370e3);
	test_region2_ph_point(3.,	4000.,	0.101077577e4);

	test_region2_ph_point(5.,	3500.,	0.801299102e3);
	test_region2_ph_point(5.,	4000.,	0.101531583e4);
	test_region2_ph_point(25.,	3500.,	0.875279054e3);

	test_region2_ph_point(40.,	2700.,	0.743056411e3);
	test_region2_ph_point(60., 	2700.,	0.791137067e3);
	test_region2_ph_point(60.,	3200.,	0.882756860e3);
}

/*------------------------------------------------------------------------------
  REGION 3 BACKWARDS (P,H)
*/

void test_region3_ph_point(double p,double h, double T, double v){
	double T1 = freesteam_region3_T_ph(p*1e6,h*1e3);
	CHECK_VAL(T1,T,RELTOL);
	double v1 = freesteam_region3_v_ph(p*1e6,h*1e3);
	CHECK_VAL(v1,v,RELTOL);
}

void testregion3ph(void){
	fprintf(stderr,"REGION 3 (P,H) TESTS\n");
	test_region3_ph_point(20.,	1700.,	6.293083892e2, 1.749903962e-3);
	test_region3_ph_point(50.,	2000.,	6.905718338e2, 1.908139035e-3);
	test_region3_ph_point(100.,	2100.,	7.336163014e2, 1.676229776e-3);

	test_region3_ph_point(20.,	2500.,	6.418418053e2, 6.670547043e-3);
	test_region3_ph_point(50.,	2400.,	7.351848618e2, 2.801244590e-3);
	test_region3_ph_point(100.,	2700.,	8.420460876e2, 2.404234998e-3);
}

/*------------------------------------------------------------------------------
  REGION 3 PSAT(H)
*/

void test_region3_psath_point(double h,double p){
	double p1 = freesteam_region3_psat_h(h*1e3);
	CHECK_VAL(p1,p*1e6,RELTOL);
}

void testregion3psath(void){
	fprintf(stderr,"REGION 3 PSAT(H) TESTS\n");
	test_region3_psath_point(1700., 1.724175718e1);
	test_region3_psath_point(2000., 2.193442957e1);
	test_region3_psath_point(2400., 2.018090839e1);
}

/*------------------------------------------------------------------------------
  REGION 3 PSAT(S)
*/

void test_region3_psats_point(double s,double p){
	double p1 = freesteam_region3_psat_s(s*1e3);
	CHECK_VAL(p1,p*1e6,RELTOL);
}

void testregion3psats(void){
	fprintf(stderr,"REGION 3 PSAT(S) TESTS\n");
	test_region3_psats_point(3.8, 1.687755057e1);
	test_region3_psats_point(4.2, 2.164451789e1);
	test_region3_psats_point(5.2, 1.668968482e1);
}

/*------------------------------------------------------------------------------
  REGION 2-3 BOUNDARY
*/

void testb23(){
	double T = 623.15;
	double p = 0.165291643e8;
	fprintf(stderr,"REGION 2-3 BOUNDARY TESTS\n");
	double p1 = freesteam_b23_p_T(T);
	CHECK_VAL(p1,p,RELTOL);
	double T1 = freesteam_b23_T_p(p);
	CHECK_VAL(T1,T,RELTOL);
}

/*------------------------------------------------------------------------------
  FULL (P,H) ROUTINES
*/

/* #define PHRELTOL 6e-5 ---region 2 */
#define PHRELTOL 1e-3 /* region 1 */

void test_steam_ph(double p,double h){
	//fprintf(stderr,"------------\n");
	//fprintf(stderr,"p = %f MPa, h = %f kJ/kg\n",p, h);
	SteamState S = freesteam_set_ph(p*1e6,h*1e3);
	//if(S.region !=1)return;
	//fprintf(stderr,"--> region = %d\n", S.region);
	CHECK_VAL(freesteam_p(S),p*1e6,PHRELTOL);
	CHECK_VAL(freesteam_h(S),h*1e3,PHRELTOL);

};

void testph(void){
	const double pp[] = {0.001, 0.0035, 0.01, 0.1, 1, 2, 5, 10, 20, 22, 22.06, 22.064, 22.07, 23, 25, 30, 40, 50, 80, 90, 100};
	const int np = sizeof(pp)/sizeof(double);
	const double hh[] = {100, 300, 400, 450, 500, 800, 1000, 1500, 2000, 2107, 2108, 2109, 2500, 2600, 2650, 2700, 2800, 2900, 3000};
	const int nh = sizeof(hh)/sizeof(double);
	const double *p, *h;

	fprintf(stderr,"FULL (P,H) TESTS\n");
	for(p=pp; p<pp+np; ++p){
		for(h=hh; h<hh+nh; ++h){
			if(freesteam_bounds_ph(*p*1e6, *h*1e3, 0))continue;
			if(freesteam_region_ph(*p*1e6, *h*1e3)!=3)continue;
			test_steam_ph(*p,*h);
		}
	}
}

/*------------------------------------------------------------------------------
  PROPERTY EVALULATION WITHIN REGION 4
*/

#define R4RELTOL 4.3e-4

typedef struct R4TestProps_struct{
	double T, p, rhof, rhog, hf, hg, sf, sg;
} R4TestProps;

/**
	Test data from IAPWS95 for the saturation region. We should conform to this
	with reasonable accuracy.
*/
const R4TestProps r4testprops_data[] = {
	{275.,	0.698451167e-3,	0.999887406e3,	0.550664919e-2,	0.775972202e1,	0.250428995e4,	0.283094670e-1,	0.910660121e1}
	,{450.,	0.932203564,	0.890341250e3,	0.481200360e1,	0.749161585e3,	0.277441078e4,	0.210865845e1,	0.660921221e1}
	,{625.,	0.169082693e2,	0.567090385e3,	0.118290280e3,	0.168626976e4,	0.255071625e4,	0.380194683e1,	0.518506121e1}
};

const int r4testprops_max = sizeof(r4testprops_data)/sizeof(R4TestProps);

void test_steam_region4_props(const R4TestProps *P){
	SteamState S;
	S = freesteam_region4_set_Tx(P->T, 0.);
	CHECK_VAL(freesteam_p(S),P->p*1e6,R4RELTOL);
	CHECK_VAL(freesteam_v(S),1./P->rhof,R4RELTOL);
	CHECK_VAL(freesteam_h(S),P->hf*1e3,R4RELTOL);
	CHECK_VAL(freesteam_s(S),P->sf*1e3,R4RELTOL);
	S = freesteam_region4_set_Tx(P->T, 1.);
	CHECK_VAL(freesteam_p(S),P->p*1e6,R4RELTOL);
	CHECK_VAL(freesteam_v(S),1./P->rhog,R4RELTOL);
	CHECK_VAL(freesteam_h(S),P->hg*1e3,R4RELTOL);
	CHECK_VAL(freesteam_s(S),P->sg*1e3,R4RELTOL);
};

void testregion4props(void){
	int i;
	fprintf(stderr,"REGION 4 PROPERTY EVALUATION TESTS\n");
	for(i=0; i< r4testprops_max; ++i){
		test_steam_region4_props(&r4testprops_data[i]);
	}
}

/*------------------------------------------------------------------------------
  DERIVATIVE ROUTINES
*/

void test_ph_derivs(double p, double h){
	SteamState S;
	S = freesteam_set_ph(p,h);
	freesteam_fprint(stderr,S);

#if 1
	if(S.region!=3)return;
	double dp = 1.;
	SteamState Sdp;
	switch(S.region){
		case 1: Sdp = freesteam_region1_set_pT(p+dp,freesteam_region1_T_ph(p+dp,h)); break;
		case 2: Sdp = freesteam_region2_set_pT(p+dp,freesteam_region2_T_ph(p+dp,h)); break;
		case 3: Sdp = freesteam_region3_set_rhoT(1./freesteam_region3_v_ph(p+dp,h),freesteam_region3_T_ph(p+dp,h)); break;
		case 4: 
			{
				double T1 = freesteam_region4_Tsat_p(p+dp);
				double hf = freesteam_region4_h_Tx(T1,0.);
				double hg = freesteam_region4_h_Tx(T1,1.);
				double x1 = (h - hf)/(hg - hg);
				Sdp = freesteam_region4_set_Tx(T1,x1);
			}
			break;
	}
	//fprintf(stderr,"S(p+dp = %g, h = %g) = ",p+dp,h);
	//freesteam_fprint(stderr,Sdp);

	double dvdp_h_fdiff = (freesteam_v(Sdp) - freesteam_v(S))/dp;
	double dvdp_h = freesteam_deriv(S,'v','p','h');
	CHECK_VAL(dvdp_h,dvdp_h_fdiff,1e-3);
#endif

	double dh = 1.;
	SteamState Sdh;
	switch(S.region){
		case 1: Sdh = freesteam_region1_set_pT(p,freesteam_region1_T_ph(p,h+dh)); break;
		case 2: Sdh = freesteam_region2_set_pT(p,freesteam_region2_T_ph(p,h+dh)); break;
		case 3: Sdh = freesteam_region3_set_rhoT(1./freesteam_region3_v_ph(p,h+dh),freesteam_region3_T_ph(p,h+dh)); break;
		case 4: 
			{
				double hf = freesteam_region4_h_Tx(S.R4.T,0.);
				double hg = freesteam_region4_h_Tx(S.R4.T,1.);
				double x1 = ((h+dh) - hf)/(hg - hg);
				Sdh = freesteam_region4_set_Tx(S.R4.T,x1);
			}
	}

	//fprintf(stderr,"S(p+dp = %g, h = %g) = ",p+dp,h);
	//freesteam_fprint(stderr,Sdp);

	double dvdh_p_fdiff = (freesteam_v(Sdh) - freesteam_v(S))/dh;
	
	double dvdh_p = freesteam_deriv(S,'v','h','p');

	CHECK_VAL(dvdh_p,dvdh_p_fdiff,1e-3);


}

void testderivs(void){
	const double pp[] = {0.001, 0.0035, 0.01, 0.1, 1, 2, 5, 10, 20, 22, 22.06 , 22.064, 22.07, 23, 25, 30, 40, 50, 80, 90, 100};
	const int np = sizeof(pp)/sizeof(double);
	const double hh[] = {100, 300, 400, 450, 500, 800, 1000/*, 1500, 2000, 2107, 2108, 2109, 2200 2500, 2600, 2650, 2700, 2800, 2900, 3000*/};
	const int nh = sizeof(hh)/sizeof(double);
	const double *p, *h;
	
	fprintf(stderr,"DERIVATIVE ROUTINE TESTS\n");
	for(p=pp; p<pp+np; ++p){
		for(h=hh; h<hh+nh; ++h){
			test_ph_derivs((*p)*1e6,(*h)*4e3);
		}
	}
}

/*------------------------------------------------------------------------------
  ZEROIN TEST
*/

typedef struct{
	double a,b,c;
} TestQuadratic;

double test_zeroin_subject(double x, void *user_data){
#define Q ((TestQuadratic *)user_data)
	double res = Q->a*x*x + Q->b*x + Q->c;
	//fprintf(stderr,"f(x = %f) = %f x² + %f x + %f = %f\n",x,Q->a,Q->b, Q->c,res);
	return res;
#undef Q
}

void testzeroin(void){
	TestQuadratic Q1 = {1, 0, -4};
	fprintf(stderr,"BRENT SOLVER TESTS\n");
	double sol = 0, err = 0;
	zeroin_solve(&test_zeroin_subject,&Q1, -10, 4.566, 1e-10, &sol, &err);
	CHECK_VAL(sol,2.,1e-10);
}

/*------------------------------------------------------------------------------
  SOLVER2 TESTS
*/

void testsolver2(void){
	fprintf(stderr,"SOLVER2 TESTS\n");
	SteamState S;

	/* test in region 3 */
	S = freesteam_region3_set_rhoT(IAPWS97_RHOCRIT, IAPWS97_TCRIT + 50.);
	assert(S.region==3);
	double p = freesteam_p(S);
	double h = freesteam_h(S);
	int status;
	SteamState S2;
	fprintf(stderr,"Solving for p = %g MPa, h = %g kJ/kgK (rho = %g, T = %g)\n",p/1e6, h/1e3,S.R3.rho, S.R3.T);
	SteamState guess = freesteam_region3_set_rhoT(1./0.00317, 673.15);	
	S2 = freesteam_solver2_region3('p','h',p,h,guess,&status);
	assert(status==0);
	CHECK_VAL(freesteam_p(S2),p, 1e-7);
	CHECK_VAL(freesteam_h(S2),h, 1e-7);

	/* test in region 4 */
	S = freesteam_region4_set_Tx(440, 0.9);
	p = freesteam_p(S);
	h = freesteam_h(S);
	fprintf(stderr,"Solving for p = %g MPa, h = %g kJ/kgK (region 4: T = %g, x = %g)\n",p/1e6, h/1e3,S.R4.T, S.R4.x);
	guess = freesteam_region4_set_Tx(IAPWS97_TCRIT - 1.,0.5);
	S2 = freesteam_solver2_region4('p','h',p,h,guess,&status);
	assert(status==0);
	CHECK_VAL(freesteam_p(S2),p, 1e-7);
	CHECK_VAL(freesteam_h(S2),h, 1e-7);

	/* test in region 2 */
	S = freesteam_region2_set_pT(1e5, 273.15+180.);
	p = freesteam_p(S);
	h = freesteam_h(S);
	fprintf(stderr,"Solving for p = %g MPa, h = %g kJ/kgK (region 2: p = %g, T = %g)\n",p/1e6, h/1e3,S.R2.p, S.R2.T);
	guess = freesteam_region2_set_pT(200e5,273.15+500.);
	S2 = freesteam_solver2_region2('p','h',p,h,guess,&status);
	assert(status==0);
	CHECK_VAL(freesteam_p(S2),p, 1e-7);
	CHECK_VAL(freesteam_h(S2),h, 1e-7);

	/* test in region 1 */
	S = freesteam_region1_set_pT(1e5, 273.15+40.);
	p = freesteam_p(S);
	h = freesteam_h(S);
	fprintf(stderr,"Solving for p = %g MPa, h = %g kJ/kgK (region 1: p = %g, T = %g)\n",p/1e6, h/1e3,S.R1.p, S.R1.T);
	guess = freesteam_region1_set_pT(200e5,273.15+20.);
	S2 = freesteam_solver2_region1('p','h',p,h,guess,&status);
	assert(status==0);
	CHECK_VAL(freesteam_p(S2),p, 1e-7);
	CHECK_VAL(freesteam_h(S2),h, 1e-7);
}


/*------------------------------------------------------------------------------
  FULL (P,T) ROUTINES
*/

void test_point_pT(double p, double T){
	SteamState S = freesteam_set_pT(p,T);
	//fprintf(stderr,"region = %d\n",S.region);
	CHECK_VAL(freesteam_p(S),p,1e-7);
	CHECK_VAL(freesteam_T(S),T,1e-7);
}

void testpT(void){
	int np = 100, nT = 100;
	double p,T, dp = (IAPWS97_PMAX - 0.)/np, dT = (IAPWS97_TMAX - IAPWS97_TMIN)/nT;
	fprintf(stderr,"FULL (P,T) TESTS\n");
	for(p = 0.; p <= IAPWS97_PMAX; p += dp){
		for(T = IAPWS97_TMIN; T <= IAPWS97_TMAX; T += dT){
			test_point_pT(p,T);
		}
	}
}

/*------------------------------------------------------------------------------
  REGION 3 (p,s) TEST DATA
*/

void test_region3_ps_point(double p,double s, double T, double v){
	double T1 = freesteam_region3_T_ps(p*1e6,s*1e3);
	CHECK_VAL(T1,T,RELTOL);
	double v1 = freesteam_region3_v_ps(p*1e6,s*1e3);
	CHECK_VAL(v1,v,RELTOL);

	//SteamState S = freesteam_set_ps(p*1e6,s*1e3);
	//CHECK_VAL(freesteam_p(S)/1e6,p,RELTOL);
	//CHECK_VAL(freesteam_s(S)/1e3,s,RELTOL);
}

void testregion3ps(void){
	fprintf(stderr,"REGION 3 (P,S) TESTS\n");
	test_region3_ps_point(20.,	3.8,	6.282959869e2, 1.733791463e-3);
	test_region3_ps_point(50.,	3.6,	6.297158726e2, 1.469680170e-3);
	test_region3_ps_point(100.,	4.0,	7.056880237e2, 1.555893131e-3);

	test_region3_ps_point(20.,	5.0,	6.401176443e2, 6.262101987e-3);
	test_region3_ps_point(50.,	4.5,	7.163687517e2, 2.332634294e-3);
	test_region3_ps_point(100.,	5.0,	8.474332825e2, 2.449610757e-3);
}	

/*------------------------------------------------------------------------------
  FULL (P,S) ROUTINES
*/

/* #define PHRELTOL 6e-5 ---region 2 */
#define PHRELTOL 1e-3 /* region 1 */

void test_steam_ps(double p,double s){
	//fprintf(stderr,"------------\n");
	//fprintf(stderr,"%s: p = %f MPa, s = %f kJ/kgK\n",__func__, p, s);
	freesteam_bounds_ps(p*1e6,s*1e3,1);
	SteamState S = freesteam_set_ps(p*1e6,s*1e3);
	//if(S.region !=1)return;
	//fprintf(stderr,"--> region = %d\n", S.region);
	//if(S.region==4)fprintf(stderr,"--> p = %g\n", freesteam_region4_psat_T(S.R4.T));
	CHECK_VAL(freesteam_p(S),p*1e6,PHRELTOL);
	CHECK_VAL(freesteam_s(S),s*1e3,PHRELTOL);

};

void testps(void){
	const double pp[] = {0.001, 0.0035, 0.01, 0.1, 1, 2, 3, 5, 10, 17, 18, 20, 22, 22.06, 22.064, 22.07, 23, 25, 30, 40, 50, 80, 90, 100};
	const int np = sizeof(pp)/sizeof(double);
	const double ss[] = {0.01,1,2,3,3.5,4,5,6,7,8,9,10,11,12};
	const int ns = sizeof(ss)/sizeof(double);
	const double *p, *s;

	fprintf(stderr,"FULL (P,S) TESTS\n");
	for(p=pp; p<pp+np; ++p){
		for(s=ss; s<ss+ns; ++s){
			if(freesteam_bounds_ps(*p*1e6,*s*1e3,0))continue;
			//if(freesteam_region_ps(*p*1e6,*s*1e3)!=3)continue;
			test_steam_ps(*p,*s);
		}
	}
}

/*------------------------------------------------------------------------------
  FULL (T,S) ROUTINES
*/

void test_steam_Ts(double T,double s){
	//fprintf(stderr,"------------\n");
	//fprintf(stderr,"%s: T = %f K, s = %f kJ/kgK\n",__func__, T, s);
	freesteam_bounds_Ts(T,s*1e3,1);
	SteamState S = freesteam_set_Ts(T,s*1e3);
	//if(S.region !=1)return;
	//fprintf(stderr,"--> region = %d\n", S.region);
	//if(S.region==4)fprintf(stderr,"--> p = %g\n", freesteam_region4_psat_T(S.R4.T));
	CHECK_VAL(freesteam_T(S),T,RELTOL);
	CHECK_VAL(freesteam_s(S),s*1e3,RELTOL);
};

void testTs(void){
	const double TT[] = {273.15, 276.15, 283.15, 300, 400, 500, 600, 621
		, REGION1_TMAX, 630, 647, IAPWS97_TCRIT, 648, 680, 700,800,900
		, 1000,1073.15
	};
	const int nT = sizeof(TT)/sizeof(double);
	const double ss[] = {0,0.01,1,2,3,3.5,4,5,6,7,8,9,10,11,12};
	const int ns = sizeof(ss)/sizeof(double);
	const double *T, *s;

	fprintf(stderr,"FULL (T,S) TESTS\n");
	for(T=TT; T<TT+nT; ++T){
		for(s=ss; s<ss+ns; ++s){
			if(freesteam_bounds_Ts(*T,*s*1e3,0))continue;
			test_steam_Ts(*T,*s);
		}
	}
}

/*------------------------------------------------------------------------------
  FULL (P,V) ROUTINES
*/

#define PVRELTOL 1e-6

void test_steam_pv(double p,double v){
	fprintf(stderr,"------------\n");
	fprintf(stderr,"%s: p = %f MPa, v = %f m3/kg\n",__func__, p, v);
	freesteam_bounds_pv(p*1e6,v,1);
	SteamState S = freesteam_set_pv(p*1e6,v);
	//if(S.region !=1)return;
	//fprintf(stderr,"--> region = %d\n", S.region);
	//if(S.region==4)fprintf(stderr,"--> p = %g\n", freesteam_region4_psat_T(S.R4.T));
	CHECK_VAL(freesteam_p(S),p*1e6,PVRELTOL);
	CHECK_VAL(freesteam_s(S),v,PVRELTOL);

};

void testpv(void){
	const double pp[] = {0.001, 0.0035, 0.01, 0.1, 1, 2, 3, 5, 10, 17, 18, 20, 22, 22.06, 22.064, 22.07, 23, 25, 30, 40, 50, 80, 90, 100};
	const int np = sizeof(pp)/sizeof(double);
	const double vv[] = {0.0009,0.001,0.002,0.005,0.01,0.02,0.03,0.04,0.05,0.1,0.2,0.3,0.4};
	const int nv = sizeof(vv)/sizeof(double);
	const double *p, *s;

	fprintf(stderr,"FULL (P,S) TESTS\n");
	for(p=pp; p<pp+np; ++p){
		for(v=vv; v<vv+nv; ++v){
			if(freesteam_bounds_pv(*p*1e6,*s,0))continue;
			//if(freesteam_region_ps(*p*1e6,*s*1e3)!=3)continue;
			test_steam_pv(*p,*s);
		}
	}
}

/*------------------------------------------------------------------------------
  MAIN ROUTINE
*/

int main(void){
	errorflag = 0;

	testregion1();
	testregion2();
	testregion3();
	testregion4();
	testregion1ph();
	testregion2ph();
	testregion3ph();
	testregion3ps();
	testregion3psath();
	testregion3psats();
	testb23();

	fprintf(stderr,"%s Max rel err = %e %%\n",errorflag?"ERRORS ENCOUNTERED":"SUCCESS!",maxrelerr*100);

	maxrelerr = 0;
	fprintf(stderr,"\nFurther tests...\n");
	testps();
	testph();
	testpT();
	testTs();
	testpv();
	//testTu();
#if 0	
	/* the following tests cause the larger errors, and are not part of the
	formal validation of freesteam. It is *expected* that T(p,h) routines and
	v(p,h) routines will introduce some errors, and we are seeing this.

	Having said that, the big errors are coming from region 1 T(p,h); without
	that, the value of PHRELTOL could be reduced to 

		1e-3 (region 1)
		6e-5 (region 2)
		3e-4 (region 3)
		5e-7 (region 4)
	
	Also, the region4props test uses data from IAPWS95, which is not in
	perfect agreement with IAPWS-IF97. */

	// COMMENT OUT TO PERFORM THESE TESTS:
	
	//testregion4props();

	//testderivs();
	//testzeroin();
	testsolver2();
#endif
	
	if(!errorflag){
		fprintf(stderr,"SUCCESS! Max rel err = %e %%\n",maxrelerr*100);
	}else{
		fprintf(stderr,"ERRORS ENCOUNTERED. Max rel err = %e %% Return code %d.\n",maxrelerr*100, errorflag);
	}
	return errorflag;
}

