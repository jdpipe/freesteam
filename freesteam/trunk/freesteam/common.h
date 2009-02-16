#ifndef FREESTEAM_COMMON_H
#define FREESTEAM_COMMON_H

/* Constants used throughout IAPWS-IF97 */

#define IAPWS97_PMAX 100e6 /* Pa */
#define IAPWS97_TMIN 273.15 /* K */

#define IAPWS97_TCRIT 647.096 /* K */
#define IAPWS97_PCRIT 22.064e6 /* Pa */
#define IAPWS97_RHOCRIT 322. /* kg/m³ */

#define IAPWS97_R 461.526 /* J/kgK */

#define IAPWS97_WARN_APPROX

#ifndef __GNUC__
# define __func__ "<function>"
#endif

#include <stdio.h>

#define IAPWS97_APPROXIMATE \
	static char _warn_approx=0; \
	if(!_warn_approx){ \
		_warn_approx = 1; \
		fprintf(stderr \
			,"WARNING: %s (%s:%d): backwards or approximation function used!\n" \
			,__func__,__FILE__,__LINE__ \
		); \
	}

#define SQ(X) ((X)*(X))

/* Basic math routines, if necesary... */

double ipow(double x, int n);

#endif /* FREESTEAM_COMMON_H */
