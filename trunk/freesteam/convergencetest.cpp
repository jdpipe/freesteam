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

#include "convergencetest.h"
#include "steamcalculator.h"

// 'test(X,p,T)' hooks

template<>
bool
ConvergenceTest<SpecificEnergy,SOLVE_IENERGY>::test(const SpecificEnergy &error, const Pressure& p, const Temperature &T){
	//cerr << endl << "         ... u error required < " << errorU * R * T;
	return ( fabs(error) < errorU * R * T);
}

template<>
bool
ConvergenceTest<SpecificEnergy,SOLVE_ENTHALPY>::test(const SpecificEnergy &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorH * R * T);
}

template<>
bool
ConvergenceTest<Density,0>::test(const Density &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < p / R / T * errorRHO);
}

template<>
bool
ConvergenceTest<SpecificVolume,0>::test(const SpecificVolume &error, const Pressure& p, const Temperature &T){
	//cerr << endl << "         ... v error required < " << errorV * R * T / p;
	return ( fabs(error) < errorV * R * T / p );
}

template<>
bool
ConvergenceTest<Pressure,0>::test(const Pressure &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorP * p);
}

template<>
bool
ConvergenceTest<Temperature,0>::test(const Temperature &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorT * T);
}

template<>
bool
ConvergenceTest<SpecificEntropy,SOLVE_ENTROPY>::test(const SpecificEntropy &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorS * R);
}

template<>
bool
ConvergenceTest<SpecHeatCap,SOLVE_CP>::test(const SpecHeatCap &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorCP * R);
}

template<>
bool
ConvergenceTest<SpecHeatCap,SOLVE_CV>::test(const SpecHeatCap &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorCV * R);
}

template<>
bool
ConvergenceTest<Num,0>::test(const Num &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorX);
}
