
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

	try{

		SatCurve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> SC; // u_sat(v)
		SteamCalculator S;

		SpecificVolume v_crit, v_b23_Tmax, v_b13, v_sat;
		SpecificEnergy u_b13, u_b23, u_b34, u_sat, u_crit;

		S.setSatSteam_T(T_CRIT);
		v_crit=S.specvol();
		u_crit=S.specienergy();


		if(v > v_crit){
			//cerr << endl << "Solver2<u,v>::whichRegion: v > v_crit";

			if(u < u_crit){
				//cerr << endl << "Solver2<u,v>::whichRegion: u < u_crit: REGION = 4";
				return 4;
			}

			S.setB23_T(TB_LOW);
			SpecificVolume v_b234 = S.specvol();

			if(v >= v_b234){
				//cerr << endl << "Solver2<u,v>::whichRegion: v > v_b234, check sat vap line";

				// check saturated vapour line
				u_sat = SC.solve(v,SAT_STEAM);

				if(u < u_sat){
					//cerr << endl << "Solver2<u,v>::whichRegion: u < u_sat: REGION = 4";
					return 4;
				}

				//cerr << endl << "Solver2<u,v>::whichRegion: u > u_sat: REGION = 2";
				return 2;
			}

			//cerr << endl << "Solver2<u,v>::whichRegion: v < v_b234, check B23 curve";

			B23Curve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> B23C;

			u_b23 = B23C.solve(v);

			if(u > u_b23){
				//cerr << endl << "Solver2<u,v>::whichRegion: u > u_b23: REGION = 2";
				return 2;
			}

			u_b34 = SC.solve(v,SAT_STEAM);

			if(u > u_b34){
				cerr << endl << "Solver2<u,v>::whichRegion: u > u_b34: REGION = 3";
				return 3;
			}

			//cerr << endl << "Solver2<u,v>::whichRegion: v < v_b234, u <= u_b34: REGION = 4";
			return 4;

		}else{
			//cerr << endl << "Solver2<u,v>::whichRegion: v < v_crit";

			if(u > u_crit){
				//cerr << endl << "Solver2<u,v>::whichRegion: u > u_crit";
				S.setB23_T(TB_HIGH);
				v_b23_Tmax = S.specvol();

				if(v > v_b23_Tmax){

					//cerr << endl << "Solver2<u,v>::whichRegion: v > v_b23_Tmax";
					B23Curve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> B23C;
					u_b23 = B23C.solve(v);

					if(u >= u_b23){
						//cerr << endl << "Solver2<u,v>::whichRegion: u >= u_b23: REGION = 2";
						return 2;
					}
				}

				cerr << endl << "Solver2<u,v>::whichRegion: u > u_crit, v < v_b23_Tmax or u < u_b23: REGION = 3";
				return 3;
			}
			// u < u_crit, v <= v_crit

			//cerr << endl <<"Solver2<u,v>::whichRegion: u <= u_crit";

			SatCurve<SpecificVolume,SpecificEnergy,0,SOLVE_IENERGY> SCu;
			v_sat = SCu.solve(u);

			S.set_pT(P_MAX,T_REG1_REG3,0.0);
			SpecificEnergy u_b13_pmax = S.specienergy();
			SpecificVolume v_b13_pmax = S.specvol();

			S.setSatWater_T(T_REG1_REG3);
			SpecificEnergy u_b134 = S.specienergy();


			//cerr << endl <<"Solver2<u,v>::whichRegion: found v_sat(u) = " << v_sat;

			if(v >= v_sat){
				//cerr << endl <<"Solver2<u,v>::whichRegion: v >= v_sat...";
				if(u > u_b134){
					//cerr << endl <<"Solver2<u,v>::whichRegion: u > b134: REGION 4";
					return 4;
				}
				if(v > v_sat){
					//cerr << endl <<"Solver2<u,v>::whichRegion: v > v_sat: REGION 4";
					return 4;
				}
			}

			//cerr << endl <<"Solver2<u,v>::whichRegion: v < v_sat (by " << (v_sat - v);

			if(u <= u_b13_pmax){
				//cerr << endl <<"Solver2<u,v>::whichRegion: v <= v_b13_pmax or v > v_b13: REGION = 1";
				return 1;
			}

			//cerr << endl << "Solver2<u,v>:whichRegion: u > u_b13_pmax ( = " << u_b13_pmax << ")";

			if(v < v_b13_pmax){
				throw new Exception("Solver2<u,v>::whichRegion: Invalid u,v: u > u_b13_pmax but v < v_b13_pmax");
			}

			//cerr << endl <<"Solver2<u,v>::whichRegion: checking u <= u_b134:";

			if(u > u_b134){
				cerr << endl <<"Solver2<u,v>::whichRegion: u > u_b134 ( = " << u_b134 << "): REGION = 3";
				return 3;
			}

			//cerr << "OK";

			//cerr << endl <<"Solver2<u,v>::whichRegion: v > v_b13_pmax, u <= u_b234: solve for v_b13";

			B13Curve<SpecificVolume, SpecificEnergy,0,SOLVE_IENERGY> B13C;
			v_b13 = B13C.solve(u);

			if(v <= v_b13){
				cerr << endl <<"Solver2<u,v>::whichRegion: v <= v_b13: REGION = 3";
				return 3;
			}

			//cerr << endl << "Solver2<u,v>::whichRegion: v > v_b13: REGION = 1";
			return 1;
		}

	}catch(Exception *E){
		stringstream s;
		s << "Solver2<u,v>::whichRegion(u = " << u / kJ_kg << "kJ_kg, v = " << v << "): " << E->what();
		throw new Exception(s.str());
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
