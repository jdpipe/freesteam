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
	}\
}
		
void test_region1_point(double T,double p, double v,double h,double u, double s, double cp, double w){

	SteamState S = freesteam_region1_set_pT(p, T);
	CHECK_VAL(freesteam_v(S),v,1e-8);
	CHECK_VAL(freesteam_h(S),h,1e-8);
	CHECK_VAL(freesteam_u(S),u,1e-8);
	CHECK_VAL(freesteam_s(S),s,1e-8);
	CHECK_VAL(freesteam_cp(S),cp,1e-8);

}	

void testregion1(void){
	test_region1_point(300, 3, 0.100215168E-2, 0.115331273E3, 0.112324818E3, 0.392294792, 0.417301218E1, 0.150773921E4);
	test_region1_point(300, 80, 0.971180894E-3, 0.184142828E3, 0.106448356E3, 0.368563852, 0.401008987E1, 0.163469054E4);
	test_region1_point(500, 3, 0.120241800E-2, 0.975542239E3, 0.971934985E3, 0.258041912E1, 0.465580682E1, 0.124071337E4);
}	

int main(void){
	errorflag = 0;
	testregion1();

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
	return errorflag;
}

