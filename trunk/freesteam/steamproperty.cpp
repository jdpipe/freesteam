#include "steamcalculator.h"
#include "steamproperty.h"

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

