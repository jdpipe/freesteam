/*

freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2005  John Pye

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef STEAMPROPERTY_H
#define STEAMPROPERTY_H

#include "steamcalculator.h"

#ifndef NDEBUG
#include <sstream>
#include <stdexcept>
#endif

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

		static inline const bool plotOnLogAxis(){
			return false;
		}
};


// 'name' hooks

template<>
const char *
SteamProperty<SpecificEnergy,SOLVE_IENERGY>::name();

template<>
const char *
SteamProperty<SpecificEnergy,SOLVE_ENTHALPY>::name();

template<>
const char *
SteamProperty<Density,0>::name();

template<>
const char *
SteamProperty<SpecificVolume,0>::name();

template<>
const char *
SteamProperty<Temperature,0>::name();

template<>
const char *
SteamProperty<Pressure,0>::name();

template<>
const char *
SteamProperty<SpecificEntropy,SOLVE_ENTROPY>::name();

template<>
const char *
SteamProperty<SpecHeatCap,SOLVE_CP>::name();

template<>
const char *
SteamProperty<SpecHeatCap,SOLVE_CV>::name();

template<>
const char *
SteamProperty<Num,0>::name();


// 'getProperty' hooks
/*
	These are inline functions so they need to be expressed here in the header file.
*/

template<>
inline SpecificEnergy
SteamProperty<SpecificEnergy,SOLVE_IENERGY>::get(const SteamCalculator &S){
	return S.specienergy();
}

template<>
inline SpecificEnergy
SteamProperty<SpecificEnergy,SOLVE_ENTHALPY>::get(const SteamCalculator &S){
	return S.specenthalpy();
}

template<>
inline Density
SteamProperty<Density,0>::get(const SteamCalculator &S){
	return S.dens();
}

template<>
inline SpecificVolume
SteamProperty<SpecificVolume,0>::get(const SteamCalculator &S){
	return S.specvol();
}

template<>
inline Temperature
SteamProperty<Temperature,0>::get(const SteamCalculator &S){
	return S.temp();
}

template<>
inline Pressure
SteamProperty<Pressure,0>::get(const SteamCalculator &S){
	return S.pres();
}

template<>
inline SpecificEntropy
SteamProperty<SpecificEntropy,SOLVE_ENTROPY>::get(const SteamCalculator &S){
	#ifndef NDEBUG
		try{
	#endif

	return S.specentropy();

	#ifndef NDEBUG
		}catch(std::exception &E){
			std::stringstream s;
			s << "SteamProperty<s>::get: " << E.what();
			throw std::runtime_error(s.str());
		}
	#endif
}

template<>
inline SpecHeatCap
SteamProperty<SpecHeatCap,SOLVE_CP>::get(const SteamCalculator &S){
	return S.speccp();
}

template<>
inline SpecHeatCap
SteamProperty<SpecHeatCap,SOLVE_CV>::get(const SteamCalculator &S){
	return S.speccv();
}

template<>
inline Num
SteamProperty<Num,0>::get(const SteamCalculator &S){
	return S.quality();
}

template<>
inline
const bool
SteamProperty<SpecificVolume,0>::plotOnLogAxis(){
	return true;
}

template<>
inline
const bool
SteamProperty<Pressure,0>::plotOnLogAxis(){
	return true;
}

// Plot stuff

template<>
inline
const bool
SteamProperty<SpecificVolume,0>::plotOnLogAxis();

template<>
inline
const bool
SteamProperty<Pressure,0>::plotOnLogAxis();

#endif
