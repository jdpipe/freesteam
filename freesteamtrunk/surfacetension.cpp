#include "surfacetension.h"
#include "common.h"

/**
	Return the surface tension for water at the given temperature.
	The correlation is the IAPWS Release on Surface Tension of Ordinary Water Substance, September 1994.
	@since 0.7
*/
SurfaceTension surfaceTension(const Temperature &T){
	double tau = 1 - T / T_CRIT;
	const ForcePerLength B = 235.8 * milli*Newton/metre;
	const double b = -0.625;
	const double mu = 1.256;

	return B * pow(tau,mu) * (1 + b * tau);
}
