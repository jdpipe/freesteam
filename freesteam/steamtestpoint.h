#ifndef STEAMTESTPOINT_H
#define STEAMTESTPOINT_H

#include "units.h"

class SteamTestPoint {
	
	public:

		// Easy initialiser for values of known units
		SteamTestPoint(double t, double p, double v, double h, double u,
		               double s, double cp, double w);

		Temperature temp;    ///< K
		Pressure pres;       ///< MPa
		SpecificVolume v;    ///< m³/kg
		SpecificEnergy h;    ///< enthalpy / kJ/kg
		SpecificEnergy u;    ///< internal energy / kJ/kg
		SpecificEntropy s;   ///< kJ/kg.K
		SpecHeatCap cp;      ///< specific heat capacity at constant pressure
		Velocity w;          ///< speed of sound

};

#endif
