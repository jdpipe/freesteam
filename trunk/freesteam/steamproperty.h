#ifndef STEAMPROPERTY_H
#define STEAMPROPERTY_H

#include "steamcalculator.h"

/// Return a particular property of steam in the given state
/**
	Allow evaluation of a desired steam property using template parameters. This is basicaly for internal use, but you may find it useful in your own code, perhaps.

	@see SatCurve, B23Curve, B13Curve, Solver, Solver2
*/
template<class Property,int PropertyAlternative=0>
class SteamProperty{
	public:
		static Property get(const SteamCalculator &S);
		static const char *name();
};

#endif
