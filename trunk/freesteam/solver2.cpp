
#include "solver2.h"

// Which region?

int
Solver2<Pressure,Temperature>::whichRegion(Pressure p, Temperature T){
	SteamCalculator S;
	S.set_pT(p,T);
	return S.whichRegion(p,T);
}

/**
	find which region we're in given (u,v)
*/
int
Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0>(SpecificEnergy u, SpecificVolume v){

	SatCurve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> SC; // u_sat(v)
	SteamCalculator S;
	S.setSatSteam_T(T_CRIT);
	SpecificVolume v_crit=S.specvol();
	
	if(v > v_crit){
		// check saturated vapour line
		u_sat = SC.solve(v,SAT_STEAM);
		
		if(u < u_sat){
			return 4;
		}		
		
		// check again B23 curve
		B23Curve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> BC;
		SpecificEnergy u_bound = BC.solve(v);

		if(u < u_bound){
			return 3;
		}

		return 2;

	}else{
		// check saturated liquid line
		u_sat = SC.solve(v,SAT_WATER);
		
		if(u < u_sat){
			return 4;
		}
		
		// check B13 curve
		B13Curve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> BC;
		SpecificEnergy u_bound = BC.solve(v);
		
		if(u < ubound){
			return 1;
		}
		
		return 3;
	}
	
	if(u < u_sat){
		return true;
	}	
	return false;
}

/// Basic, default behaviour for pressure-temperature
SteamCalculator
Solver2<Pressure,Temperature,0,0>::solve(Pressure p,Temperature T){
	SteamCalculator S;
	S.set_pT(p,T);
	return S;
}

/// When one variable is pressure,
template<
SteamCalculator
Solver2<Pressure,
