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

#include "steamcalculator.h"
#include "steamproperty.h"


// 'name' hooks

template<>
const char *
SteamProperty<SpecificEnergy,SOLVE_IENERGY>::name(){
	return "u";
}

template<>
const char *
SteamProperty<SpecificEnergy,SOLVE_ENTHALPY>::name(){
	return "h";
}

template<>
const char *
SteamProperty<Density,0>::name(){
	return "rho";
}

template<>
const char *
SteamProperty<SpecificVolume,0>::name(){
	return "v";
}

template<>
const char *
SteamProperty<Temperature,0>::name(){
	return "T";
}

template<>
const char *
SteamProperty<Pressure,0>::name(){
	return "p";
}

template<>
const char *
SteamProperty<SpecificEntropy,SOLVE_ENTROPY>::name(){
	return "s";
}

template<>
const char *
SteamProperty<SpecHeatCap,SOLVE_CP>::name(){
	return "cp";
}

template<>
const char *
SteamProperty<SpecHeatCap,SOLVE_CV>::name(){
	return "cv";
}

template<>
const char *
SteamProperty<Num,0>::name(){
	return "x";
}

