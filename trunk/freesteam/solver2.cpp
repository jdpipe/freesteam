
#include "steamproperty.h"
#include "solver2.h"
#include "units.h"
#include "steamcalculator.h"
#include "b23curve.h"
#include "b13curve.h"

// Which region?

// whichRegion given p, T
	
int
Solver2<Pressure,Temperature,0,0>::whichRegion(const Pressure &p, const Temperature &T){
	SteamCalculator S;
	S.set_pT(p,T);
	cerr << endl << "Solver2: whichRegion(p,T) = " << S.whichRegion();
	return S.whichRegion();
}

/**
	whichRegion given u, v
*/
int
Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0>::whichRegion(const SpecificEnergy &u, const SpecificVolume &v){
	
	//cerr << endl << "Solver2: whichRegion(u,v)...";
	
	SatCurve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> SC; // u_sat(v)
	SteamCalculator S;
	S.setSatSteam_T(T_CRIT);
	SpecificVolume v_crit=S.specvol();
	SpecificEnergy u_crit=S.specienergy();
	
	S.set_pT(100.0 * MPa, T_REG1_REG3);
	SpecificVolume v_b13 = S.specvol();

	SpecificEnergy u_b13, u_b23, u_sat;
	
	if(v > v_crit){
		//cerr << endl << "Solver2: v > v_crit";
		
		if(u < u_crit){
			cerr << endl << "Solver2: u < u_crit: REGION = 4";
			return 4;
		}
		
		// check saturated vapour line
		u_sat = SC.solve(v,SAT_STEAM);
		
		if(u < u_sat){
			cerr << endl << "Solver2: u < u_sat: REGION = 4";
			return 4;
		}		
		
		// check again B23 curve
		B23Curve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> *B23C;
		B23C = new B23Curve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0>();
		
		u_b23 = B23C->solve(v);

		if(u < u_b23){
			cerr << endl << "Solver2: u < u_b23: REGION = 3";
			return 3;
		}

		cerr << endl << "Solver2: u >= u_b23: REGION = 2";
		return 2;

	}else{
		//cerr << endl << "Solver2: v < v_crit";
		
		if(v < v_b13){
			return 1;
		}
		
		if(u < u_crit){
			//cerr << endl <<"Solver2: v > v_b13 and u < u_crit: solving for saturated water at v = " << v;
			
			// check saturated liquid line
			u_sat = SC.solve(v,SAT_WATER);

			if(u < u_sat){

				cerr << endl << "Solver2: u < u_sat: REGION = 4";
				return 4;
			}
		}
		
		// check B13 curve
		B13Curve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> B13C;
		SpecificEnergy u_b13 = B13C.solve(v);
		
		if(u <= u_b13){
			//cerr << endl << "Solver2: u < u_bound: REGION = 1";
			return 1;
		}
		
		cerr << endl << "Solver2: u > u_b13 = " << u_b13 <<" ( u - u_b13 = " << u - u_b13 << "): REGION = 3";
		return 3;
	}
	
}

//----------------------------------------
// Solve to find point

/*

/// solve for p,T
SteamCalculator
Solver2<Pressure,Temperature,0,0>::solve(Pressure p,Temperature T){
	SteamCalculator S;
	S.set_pT(p,T);
	return S;
}

*/
/// Generic two-point solver

/**
	@todo
		Case where one variable is pressure or temperature, for each region
*/

// All the stuff in 'if statements' should be templateable or implemented with a stategy pattern, or something, eg
/*
		Solver2::solve(xxx,yyy){
			try{
				switch(whichRegion(xxx,yyy)){
					case 1:
						return solveRegion1(xxx,yyy);
					case 2:
						return solveRegion2(xxx,yyy);
					case 3:
						return solveRegion3(xxx,yyy);
					case 4:
						return solveRegion4(xxx,yyy);
				}
			}catch(Exception *E){
				stringstream s;
				s << "Solver2::solve: " << E->what();
				throw new Exception(s.str());
			}
		}
		
		Then, each solveRegionN(xxx,yyy) can done with partial template specialisations etc as required.
		
		REGION     |    FirstQuantity   |   SecondQuantity
		-----------|--------------------|----------------------------
		4				Pressure			*					single solver
		4				* 					Temperature			single solver
		1				Pressure			*					single solver
		1				*					Temperature			single solver
		2				Pressure			*					single solver
		2				*					Temperature			single solver
		3				Density				*					single solver
		3				SpecificVolume		*					single solver
		3				*					Temperature			single solver
		
		All the rest should be possible a common two-variable solver once brackets have been determined, eg on 
		
		Region 1:
			T < xxx
			p > p_sat(T)
		Region 2:
			T > T_MIN
			T < T_MAX
			depending on T, p < p_sat(T) or p < p_bound(T)
		Region 3:
			Rho in certain range
			T in certain range
			Maybe check validity after increments added
		
			
		if(whi
		Region4<SpecificEnergy,SpecificVolume>::solve
	}
*/
