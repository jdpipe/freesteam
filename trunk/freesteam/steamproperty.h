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
		static inline Property get(const SteamCalculator &S);
		static const char *name();
};

// 'getProperty' hooks

inline SpecificEnergy
SteamProperty<SpecificEnergy,SOLVE_IENERGY>::get(const SteamCalculator &S){
	return S.specienergy();
}

inline SpecificEnergy
SteamProperty<SpecificEnergy,SOLVE_ENTHALPY>::get(const SteamCalculator &S){
	return S.specenthalpy();
}

inline Density
SteamProperty<Density,0>::get(const SteamCalculator &S){
	return S.dens();
}

inline SpecificVolume
SteamProperty<SpecificVolume,0>::get(const SteamCalculator &S){
	return S.specvol();
}

inline Temperature
SteamProperty<Temperature,0>::get(const SteamCalculator &S){
	return S.temp();
}

inline Pressure
SteamProperty<Pressure,0>::get(const SteamCalculator &S){
	return S.pres();
}

inline SpecificEntropy
SteamProperty<SpecificEntropy,SOLVE_ENTROPY>::get(const SteamCalculator &S){
	return S.specentropy();
}

inline SpecHeatCap
SteamProperty<SpecHeatCap,SOLVE_CP>::get(const SteamCalculator &S){
	return S.speccp();
}

inline SpecHeatCap
SteamProperty<SpecHeatCap,SOLVE_CV>::get(const SteamCalculator &S){
	return S.speccv();
}

inline Num
SteamProperty<Num,0>::get(const SteamCalculator &S){
	return S.quality();
}

#endif
