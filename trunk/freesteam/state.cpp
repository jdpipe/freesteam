#include "state.h"
#include "steamcalculator_macros.h"
#include <cstdio>

#define EVAL_STEAM(FUNC,EXPR) EXPR_FUNC(SteamState,FUNC,EXPR)
#define EVAL_STEAM_OBJ(FUNC,EXPR) EXPR_FUNC_OBJ(SteamState,FUNC,EXPR)

int SteamState::getRegion() const{
	return -1;
}

SteamState *SteamState::Instance() {
	throw new Exception("Illegal call to SteamState::Instance(). Must instantiate a subclass.");
}

void SteamState::set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x) {
	throw new Exception("Illegal call to SteamState::Instance(). Must instantiate a subclass.");
}

/**
* @return Temperature of water/steam [K]
*/
inline Temperature
SteamState::temp(const SteamCalculator &c) const{
	REQUIRE(c.T > 0.0 * Kelvin);
	return c.T;
}

/**
* @return Pressure of steam/water[MPa]
*/
inline Pressure
SteamState::pres(const SteamCalculator &c) const{
	REQUIRE(c.p >= 0.0 * MPa);
	return c.p;
}

/**
* @return Specific volume of steam [m³/kg]
*/
inline SpecificVolume
SteamState::specvol(const SteamCalculator &c) const{
	return (R * c.T / c.p) * c.pi * gampi(c);
}

/**
* @return Density of steam [kg/m³]
*/
inline Density
SteamState::dens(const SteamCalculator &c) const{
	return 1.0 / specvol(c);
}

/*
Num Steam::dens(){
	Num dens=0;
	dens=(1/specvol());
	printf("NOTE: in Steam::dens(), dens = %10.8e (region %d)\n",dens,whereami());
	return dens;
}
*/
/**
* @return Specific internal energy of water/steam [kJ/kg]
*/
SpecificEnergy
SteamState::specienergy(const SteamCalculator &c) const{
	return (R * c.T) * (c.tau * gamtau(c) - c.pi * gampi(c));
}

/**
* @return Specific entropy water/steam [kJ/kg]
*/
SpecificEntropy
SteamState::specentropy(const SteamCalculator &c) const{
	return R * (c.tau * gamtau(c) - gam(c));
}

/**
* @return Specific enthalpy of water/steam [kJ/kg]
*/
SpecificEnergy
SteamState::specenthalpy(const SteamCalculator &c) const{
	return R * c.T * (c.tau * gamtau(c));
}

/**
* @return Isobaric specific heat capacity of water/steam [kJ/kgK]
*/
SpecHeatCap
SteamState::speccp(const SteamCalculator &c) const{
	return R * (-sq(c.tau) * gamtautau(c));
}

/**
* @return Isochoric specific heat capacity of water/steam [kJ/kgK]
*/
SpecHeatCap
SteamState::speccv(const SteamCalculator &c) const{
	return R * (-sq(c.tau) * gamtautau(c) +
                           sq(gampi(c) -
                              c.tau * gampitau(c)) / gampipi(c));
}
