#include "region4.h"
#include "region1.h"
#include "region2.h"
#include "steamcalculator_macros.h"

#include <stdio.h>

Region4 *Region4::_instance = 0;

int Region4::getRegion() {
	return 4;
}

// when creating this object, need to get instance of Region1, Region2
// so that interpolation can be done.
Region4::Region4() {
	// liquid properties will be stored in the parent object
	this->liquidstate = Region2::Instance();
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

void Region4::set_pT(SteamCalculator * c, Pressure p, Temperature T,
                          Num x) {
	// SteamCalculator will already have checked that these are saturated values:
	c->gas = new SteamCalculator();
	c->gas->setSatSteam_p(p);
	c->liq = new SteamCalculator();
	c->liq->setSatWater_p(p);
	c->x = x;
	c->p = p;
	c->T = T;
	c->isset = true;
	ENSURE(c->whichRegion() == 4);
	//fprintf(stderr,"Set properties of steam in Region 4!\n");
	// so what is x then?
	// SteamCalculator will have set that as well.
}

//--------
// Macro to interpolate between x=0 and x=1 to find actual two-phase value
// Pass the common 'c' object to the SteamState instances.

#define INTERP_REGION4(FUNC,TYPE) \
	TYPE Region4::FUNC(SteamCalculator *c){ \
		SteamCalculator *g=c->gas; \
		SteamCalculator *l=c->liq; \
		TYPE sub=l->FUNC(); \
		TYPE sup=g->FUNC(); \
		return sub*(1 - c->x)+sup*(c->x); \
	}

INTERP_REGION4(specvol,      SpecificVolume);
INTERP_REGION4(specienergy,  SpecificEnergy);
INTERP_REGION4(specentropy,  SpecificEntropy);
INTERP_REGION4(specenthalpy, SpecificEnergy);
INTERP_REGION4(speccp,       SpecHeatCap);
INTERP_REGION4(speccv,       SpecHeatCap);
INTERP_REGION4(dens,         Density);

