#include "steamcalculator.h"
#include "steamproperty.h"

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

// 'name' hooks

const char *
SteamProperty<SpecificEnergy,SOLVE_IENERGY>::name(){
	return "u";
}

const char *
SteamProperty<SpecificEnergy,SOLVE_ENTHALPY>::name(){
	return "h";
}

const char *
SteamProperty<Density,0>::name(){
	return "rho";
}

const char *
SteamProperty<SpecificVolume,0>::name(){
	return "v";
}

const char *
SteamProperty<Temperature,0>::name(){
	return "T";
}

const char *
SteamProperty<Pressure,0>::name(){
	return "p";
}

const char *
SteamProperty<SpecificEntropy,SOLVE_ENTROPY>::name(){
	return "s";
}

const char *
SteamProperty<SpecHeatCap,SOLVE_CP>::name(){
	return "cp";
}

const char *
SteamProperty<SpecHeatCap,SOLVE_CV>::name(){
	return "cv";
}

const char *
SteamProperty<Num,0>::name(){
	return "x";
}

