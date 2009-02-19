#include "convergencetest.h"
#include "steamcalculator.h"

// 'test(X,p,T)' hooks

bool
ConvergenceTest<SpecificEnergy,SOLVE_IENERGY>::test(const SpecificEnergy &error, const Pressure& p, const Temperature &T){
	//cerr << endl << "         ... u error required < " << errorU * R * T;
	return ( fabs(error) < errorU * R * T);
}

bool
ConvergenceTest<SpecificEnergy,SOLVE_ENTHALPY>::test(const SpecificEnergy &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorH * R * T);
}

bool
ConvergenceTest<Density,0>::test(const Density &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < p / R / T * errorRHO);
}

bool
ConvergenceTest<SpecificVolume,0>::test(const SpecificVolume &error, const Pressure& p, const Temperature &T){
	//cerr << endl << "         ... v error required < " << errorV * R * T / p;
	return ( fabs(error) < errorV * R * T / p );
}

bool
ConvergenceTest<Pressure,0>::test(const Pressure &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorP * p);
}

bool
ConvergenceTest<Temperature,0>::test(const Temperature &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorT * T);
}

bool
ConvergenceTest<SpecificEntropy,SOLVE_ENTROPY>::test(const SpecificEntropy &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorS * R);
}

bool
ConvergenceTest<SpecHeatCap,SOLVE_CP>::test(const SpecHeatCap &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorCP * R);
}

bool
ConvergenceTest<SpecHeatCap,SOLVE_CV>::test(const SpecHeatCap &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorCV * R);
}

bool
ConvergenceTest<Num,0>::test(const Num &error, const Pressure& p, const Temperature &T){
	return ( fabs(error) < errorX);
}
