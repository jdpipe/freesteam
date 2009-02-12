#include "steam.h"
#include "steam_ph.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int errorflag = 0;

#define CHECK_VAL(EXPR, VAL, RELTOL){ \
	double calc = (EXPR); \
	double error = calc - (VAL);\
	if(fabs(error) > fabs(VAL*RELTOL)){\
		fprintf(stderr,"ERROR (%s:%d): %s = %e, should be %e, error %10e %% exceeds tol %10e %%\n",\
			__FILE__,__LINE__,#EXPR, calc, (VAL), error/VAL*100., RELTOL*100.\
		);\
		errorflag = 1; \
	}else{\
		fprintf(stderr,"OK: %s = %f with %f %% error (test value = %f).\n", #EXPR, calc, error/VAL*100,VAL);\
	}\
}

/*------------------------------------------------------------------------------
  REGION 1: FORWARDS
*/
		
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
	CHECK_VAL(freesteam_v(S),v,1e-8);
	CHECK_VAL(freesteam_h(S),h*1e3,1e-8);
	CHECK_VAL(freesteam_u(S),u*1e3,1e-8);
	CHECK_VAL(freesteam_s(S),s*1e3,1e-8);
	CHECK_VAL(freesteam_cp(S),cp*1e3,1e-8);

}	

void testregion1(void){
	fprintf(stderr,"REGION 1 TESTS\n");
	test_region1_point(300, 3, 0.100215168E-2, 0.115331273E3, 0.112324818E3, 0.392294792, 0.417301218E1, 0.150773921E4);
	test_region1_point(300, 80, 0.971180894E-3, 0.184142828E3, 0.106448356E3, 0.368563852, 0.401008987E1, 0.163469054E4);
	test_region1_point(500, 3, 0.120241800E-2, 0.975542239E3, 0.971934985E3, 0.258041912E1, 0.465580682E1, 0.124071337E4);
}	

/*------------------------------------------------------------------------------
  REGION 2: FORWARDS
*/

void test_region2_point(double T,double p, double v,double h,double u, double s, double cp, double w){

	/* units of measurement as for region1 test */

	SteamState S = freesteam_region2_set_pT(p*1e6, T);
	CHECK_VAL(freesteam_v(S),v,1e-8);
	CHECK_VAL(freesteam_h(S),h*1e3,1e-8);
	CHECK_VAL(freesteam_u(S),u*1e3,1e-8);
	CHECK_VAL(freesteam_s(S),s*1e3,1e-8);
	CHECK_VAL(freesteam_cp(S),cp*1e3,1e-8);
}

void testregion2(void){
	fprintf(stderr,"REGION 2 TESTS\n");
	test_region2_point(300, 0.0035, 0.394913866E2, 0.254991145E4, 0.241169160E4, 0.852238967E1, 0.191300162E1, 0.427920172E3);
	test_region2_point(700, 0.0035, 0.923015898E2, 0.333568375E4, 0.301262819E4, 0.101749996E2, 0.208141274E1, 0.644289068E3);
	test_region2_point(700, 30, 0.542946619E-02, 0.263149474E+4, 0.246861076E+4, 0.517540298E+1, 0.103505092E+2, 0.480386523E+3);
}

int main(void){
	errorflag = 0;
	testregion1();
	testregion2();

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
		fprintf(stderr,"SUCCESS\n");
	}else{
		fprintf(stderr,"ERRORS ENCOUNTERED. Return code %d.\n",errorflag);
	}
	return errorflag;
}

