#ifndef STEAMPROPERTY_H
#define STEAMPROPERTY_H

#include "steamcalculator.h"

/// Return a particular property of steam in the given state
/**
	Allow evaluation of a desired steam property using template parameters.
	
	@see SatCurve, B23Curve, B13Curve, Solver, Solver2
*/
template<class Property,int PropertyAlternative=0>
class SteamProperty{
	public:
		static Property get(SteamCalculator &S);
		static const char *name();
};

#endif
