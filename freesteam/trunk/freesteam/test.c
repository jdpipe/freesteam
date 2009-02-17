#include "steam.h"
#include "steam_ph.h"
#include "region4.h"
#include "backwards.h"
#include "b23.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int errorflag = 0;
double maxrelerr = 0;
int verbose = 1;

#define CHECK_VAL(EXPR, VAL, RELTOL){ \
	double calc = (EXPR); \
	double error = calc - (VAL);\
	double relerr = error / (VAL);\
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
	fprintf(stderr,"REGION 3 Psat(H) TESTS\n");
	test_region3_psath_point(1700., 1.724175718e1);
	test_region3_psath_point(2000., 2.193442957e1);
	test_region3_psath_point(2400., 2.018090839e1);
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

#define PHRELTOL 0.4e-2

void test_steam_ph(double p,double h){
	SteamState S = freesteam_set_ph(p*1e6,h*1e3);
	fprintf(stderr,"p = %f, h = %f: region = %d\n",p, h, S.region);
	CHECK_VAL(freesteam_p(S),p*1e6,RELTOL);
	CHECK_VAL(freesteam_h(S),h*1e3,PHRELTOL);

};

void testph(void){
	const double pp[] = {0.001, 0.01, 0.1, 1, 10/*, 20, 22, 22.064, 23, 30, 50, 100*/};
	const int np = sizeof(pp)/sizeof(double);
	const double hh[] = {10, 20, 50, 100, 300, 400, 450, 500, 1000, 1500, 2000, 2500, 3000, 5000};
	const int nh = sizeof(hh)/sizeof(double);
	const double *p, *h;

	fprintf(stderr,"FULL (P,H) TESTS\n");
	for(p=pp; p<pp+np; ++p){
		for(h=hh; h<hh+nh; ++h){
			test_steam_ph(*p,*h);
		}
	}
}

/*------------------------------------------------------------------------------
  MAIN ROUTINE
*/

int main(void){
	errorflag = 0;
#if 0
	testregion1();
	testregion2();
	testregion3();
	testregion4();
	testregion1ph();
	testregion2ph();
	testregion3ph();
	testregion3psath();
	testb23();
#endif
	testph();

#if 0
	SteamState S;
	S = freesteam_set_ph(100e5,2000);
	fprintf(stderr,"p = %f\n",freesteam_p(S));
	fprintf(stderr,"h = %f\n",freesteam_h(S));
	fprintf(stderr,"T = %f\n",freesteam_T(S));
	fprintf(stderr,"v = %f\n",freesteam_v(S));
	fprintf(stderr,"u = %f\n",freesteam_u(S));
	fprintf(stderr,"s = %f\n",freesteam_s(S));
#endif
	if(!errorflag){
		fprintf(stderr,"SUCCESS! Max rel err = %e %%\n",maxrelerr*100);
	}else{
		fprintf(stderr,"ERRORS ENCOUNTERED. Max rel err = %e %% Return code %d.\n",maxrelerr*100, errorflag);
	}
	return errorflag;
}

