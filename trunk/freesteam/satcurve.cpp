
#include "satcurve.h"

// 'getProperty' hooks

SpecificEnergy
SteamProperty<SpecificEnergy,SOLVE_IENERGY>::get(SteamCalculator &S){
	return S.specienergy();
}

SpecificEnergy
SteamProperty<SpecificEnergy,SOLVE_ENTHALPY>::get(SteamCalculator &S){
	return S.specenthalpy();
}

Density
SteamProperty<Density,0>::get(SteamCalculator &S){
	return S.dens();
}

SpecificVolume
SteamProperty<SpecificVolume,0>::get(SteamCalculator &S){
	return S.specvol();
}

Temperature
SteamProperty<Temperature,0>::get(SteamCalculator &S){
	return S.temp();
}

Pressure
SteamProperty<Pressure,0>::get(SteamCalculator &S){
	return S.pres();
}

SpecificEntropy
SteamProperty<SpecificEntropy,SOLVE_ENTROPY>::get(SteamCalculator &S){
	return S.specentropy();
}

SpecHeatCap
SteamProperty<SpecHeatCap,SOLVE_CP>::get(SteamCalculator &S){
	return S.speccp();
}

SpecHeatCap
SteamProperty<SpecHeatCap,SOLVE_CV>::get(SteamCalculator &S){
	return S.speccv();
}

Num
SteamProperty<Num,0>::get(SteamCalculator &S){
	return S.quality();
}
