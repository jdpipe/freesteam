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
#include "exception.h"
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
	#ifndef NDEBUG
		try{
	#endif

	return S.specentropy();

	#ifndef NDEBUG
		}catch(Exception &E){
			std::stringstream s;
			s << "SteamProperty<s>::get: " << E.what();
			throw Exception(s.str());
		}
	#endif
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

inline
const bool
SteamProperty<SpecificVolume,0>::plotOnLogAxis(){
	return true;
}

inline
const bool
SteamProperty<Pressure,0>::plotOnLogAxis(){
	return true;
}

#endif
