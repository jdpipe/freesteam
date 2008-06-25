/**
	@file
	Routines to return surface tension of water according to the IAPWS release.
*/

#ifndef SURFACETENSION_H
#define SURFACETENSION_H

#include "units.h"

SurfaceTension surfaceTension(const Temperature &T);

#endif
