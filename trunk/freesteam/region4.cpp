#include "region4.h"
#include "region1.h"
#include "region2.h"
#include "steamcalculator_macros.h"

#include <stdio.h>

Region4 *Region4::_instance = 0;

int Region4::getRegion() const{
	return 4;
}

// when creating this object, need to get instance of Region1, Region2
// so that interpolation can be done.
Region4::Region4() {
	// liquid properties will be stored in the parent's 'liq' object
	// gas properties will be stored in the parent's 'gas' object.
}

SteamState *Region4::Instance() {
	//fprintf(stderr,"Region4 instance...\n");
	if (_instance == 0) {
		//fprintf(stderr,"Creating new object...\n");
		_instance = new Region4();
		//fprintf(stderr,"New object created...\n");
	}
	return _instance;
}

Temperature
Region4::temp(const SteamCalculator &c) const{
	return c.T;
}

void Region4::set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T,
                          Num x) {
	// SteamCalculator will already have checked that these are saturated values:

	//REQUIRE(Boundaries::isSat_pT(p,T));
	REQUIRE(x >=0);
	REQUIRE(x <=1);

	c.gas = new SteamCalculator();
	c.gas->setSatSteam_T(T);
	c.liq = new SteamCalculator();
	c.liq->setSatWater_T(T);
	c.x = x;
	c.p = c.gas->pres();
	c.T = T;
	c.isset = true;

	ENSURE(c.whichRegion() == 4);
}


Num
Region4::pitau_iaps85(const SteamCalculator &c) const{
	throw new Exception("Invalid call to Region4::pitau_iaps85");
}

Num
Region4::delpi_iaps85(const SteamCalculator &c) const{
	throw new Exception("Invalid call to Region4::delpi_iaps85");
}

//--------
// Macro to interpolate between x=0 and x=1 to find actual two-phase value
// Pass the common 'c' object to the SteamState instances.

#define INTERP_REGION4(FUNC,TYPE) \
	TYPE Region4::FUNC(const SteamCalculator &c) const{ \
		ASSERT(c.x >= 0); \
		ASSERT(c.x <= 1); \
		SteamCalculator *g = c.gas; \
		SteamCalculator *l = c.liq; \
		TYPE sub = l->FUNC(); \
		TYPE sup = g->FUNC(); \
		return sub*(1 - c.x)+sup*(c.x); \
	}

INTERP_REGION4(specvol,      SpecificVolume);
INTERP_REGION4(specienergy,  SpecificEnergy);
INTERP_REGION4(specentropy,  SpecificEntropy);
INTERP_REGION4(specenthalpy, SpecificEnergy);
INTERP_REGION4(speccp,       SpecHeatCap);
INTERP_REGION4(speccv,       SpecHeatCap);
INTERP_REGION4(dens,         Density);

Pressure
Region4::pres(const SteamCalculator &c) const{
	return c.gas->pres();
}
