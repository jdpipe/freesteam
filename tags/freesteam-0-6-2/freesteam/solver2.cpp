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

#include "solver2.h"
#include "steamproperty.h"
#include "units.h"
#include "steamcalculator.h"
#include "b23curve.h"
#include "b13curve.h"

using namespace std;

// Which region?

/**
	whichRegion given p, T
*/
template<>
int
Solver2<Pressure,Temperature,0,0>::whichRegion(const Pressure &p, const Temperature &T){
	SteamCalculator S;
	S.set_pT(p,T);
	//cerr << endl << "Solver2: whichRegion(p,T) = " << S.whichRegion();
	int region = S.whichRegion();

	if(region==4){
		throw runtime_error("Solver2<p,T>::whichRegion: invalid region for this solver.");
	}
	return S.whichRegion();

}

/**
	Use the straight-up region 1 correlation if attempting to use Solver2<p,T> for region 1
*/
template<>
SteamCalculator
Solver2<Pressure,Temperature,0,0>::makeRegion1Guess(const Pressure& p, const Temperature &T){
	SteamCalculator S;
	S.setRegion1_pT(p,T);
	return S;
}

/**
	Use the straight-up region 2 correlation if attempting to use Solver2<p,T> for region 2
*/
template<>
SteamCalculator
Solver2<Pressure,Temperature,0,0>::makeRegion2Guess(const Pressure& p, const Temperature &T){
	SteamCalculator S;
	S.setRegion2_pT(p,T);
	return S;
}

// Solve Region3 for pressure, temperature.
/**
	 That's easy - just use the correlation directly.
*/
template<>
SteamCalculator
Solver2<Pressure,Temperature,0,0>::solveRegion3(const Pressure &p, const Temperature &T, const SteamCalculator &firstguess){
	SteamCalculator S;
	try{
		S.set_pT(p,T);
		return S;
	}catch(exception &E){
		stringstream s;
		s << "Solver2<p,T>::solverRegion3: " << E.what();
		throw runtime_error(s.str());
	}
}

/**
	whichRegion given T, h
*/
template<>
int 
Solver2<Temperature, SpecificEnergy, 0, SOLVE_ENTHALPY>::whichRegion(const Temperature &T, const SpecificEnergy &h){

	SteamCalculator S;
	try{

		if(T < T_CRIT){
			SatCurve<SpecificEnergy,Temperature,SOLVE_ENTHALPY> SC;
			SpecificEnergy h_f = SC.solve(T,SAT_WATER);
			SpecificEnergy h_g = SC.solve(T,SAT_STEAM);

			if(T < T_REG1_REG3){
				if(h <= h_f){
					return 1;
				}

				if(h >= h_g){
					return 2;
				}
			}

			if(h >= h_f && h <=h_g){
				return 4;
			}
		}

		if(T > TB_HIGH){
			return 2;
		}

		B23Curve<SpecificEnergy,Temperature,SOLVE_ENTHALPY> B23;
		SpecificEnergy h_b23 = B23.solve(T);

		if(h < h_b23){
			return 3;
		}

		return 2;

	}catch(exception &E){
		stringstream ss;
		ss << "Solver2<T,h>::whichRegion(T = " << T << ", h = " << h/kJ_kg << " kJ/kgK): " << E.what();
		throw runtime_error(ss.str());
	}
}


/**
	which region given T,s
*/
template<>
int
Solver2<Temperature,SpecificEntropy,0,SOLVE_ENTROPY>::whichRegion(const Temperature &T, const SpecificEntropy &s){

	SteamCalculator S;

	try{

		S.set_pT(P_MAX,T);
		SpecificEntropy s_min = S.specentropy();
		if(s < s_min){
			stringstream ss;
			ss << "Specific entropy out of range, too low: s = " << s << " (min at T = " << T << " is " << s_min << ")";
			throw runtime_error(ss.str());
		}

		if(T <= T_CRIT){
			SatCurve<SpecificEntropy,Temperature> SC;
			SpecificEntropy s_f = SC.solve(T,SAT_WATER);

			SpecificEntropy s_g = SC.solve(T,SAT_STEAM);

			if(T < T_REG1_REG3){
				if(s <= s_f){
				return 1;
				}

				if(s >= s_g){
					return 2;
				}
			}

			if(s > s_f && s < s_g){
				return 4;
			}
		}

		if(T > TB_HIGH){
			return 2;
		}

		B23Curve<SpecificEntropy,Temperature> B23;
		SpecificEntropy s_b23 = B23.solve(T);

		if(s < s_b23){
			return 3;
		}

		return 2;

	}catch(exception &E){
		stringstream ss;
		ss << "Solver2<T,s>::whichRegion: " << E.what();
		throw runtime_error(ss.str());
	}
}

/**
	whichRegion given p, s
*/
template<>
int
Solver2<Pressure,SpecificEntropy,0,SOLVE_ENTROPY>::whichRegion(const Pressure &p, const SpecificEntropy &s){

	try{

		SteamCalculator S;

		S.setRegion3_rhoT(RHO_CRIT,T_CRIT);
		Pressure p_crit = S.pres();

		S.setSatWater_T(T_REG1_REG3);
		Pressure p_b1234 = S.pres();

		S.set_pT(p,T_MAX);
		SpecificEntropy s_max = S.specentropy();

		if(s > s_max){
			stringstream ss;
			ss << "s=" << s << " exceeds maximum (s_max=" << s_max << " at p=" << p << ").";
			throw runtime_error(ss.str());
		}

		REQUIRE(p > P_MIN);
		REQUIRE(p < P_MAX);
		REQUIRE(s > 0.0 * kJ_kgK);

		if(p <= p_crit){

			SatCurve<SpecificEntropy,Pressure,SOLVE_ENTROPY> SC; // u_sat(p);
			SpecificEntropy s_f = SC.solve(p,SAT_WATER);
			SpecificEntropy s_g = SC.solve(p,SAT_STEAM);

			if(p <= p_b1234){
				if(s <= s_f){
					return 1;
				}

				if(s >= s_g){
					return 2;
				}
			}

			if(s < s_g and s > s_f){
				return 4;
			}
		}

		B13Curve<SpecificEntropy,Pressure,SOLVE_IENERGY> B13;
		SpecificEntropy s_b13 = B13.solve(p);
		if(s <= s_b13){
			return 1;
		}

		B23Curve<SpecificEntropy,Pressure,SOLVE_IENERGY> B23;
		SpecificEntropy s_b23 = B23.solve(p);
		if(s >= s_b23){
			return 2;
		}

		return 3;

	}catch(exception &E){
		stringstream ss;
		ss << "Solver2<p,s>::whichRegion(p = " << p/MPa << " MPa, s = " << s/kJ_kgK << " kJ/kgK): " << E.what();
		throw runtime_error(ss.str());
	}
}

/**
	whichRegion given p, u
*/
template<>
int
Solver2<Pressure,SpecificEnergy,0,SOLVE_IENERGY>::whichRegion(const Pressure &p, const SpecificEnergy &u){

	try{

		SteamCalculator S;

		S.setSatWater_T(T_REG1_REG3);
		Pressure p_b1234 = S.pres();
		S.setRegion3_rhoT(RHO_CRIT,T_CRIT);
		Pressure p_crit = S.pres();

		REQUIRE(p > P_MIN);
		REQUIRE(p < P_MAX);

		if(p <= p_crit){

			SatCurve<SpecificEnergy,Pressure,SOLVE_IENERGY> SC; // u_sat(p);
			SpecificEnergy u_f = SC.solve(p,SAT_WATER);
			SpecificEnergy u_g = SC.solve(p,SAT_STEAM);

			if(p <= p_b1234){
				if(u <= u_f){
					return 1;
				}

				if(u >= u_g){
					return 2;
				}
			}

			if(u < u_g and u > u_f){
				return 4;
			}
		}

		B13Curve<SpecificEnergy,Pressure,SOLVE_IENERGY> B13;
		SpecificEnergy u_b13 = B13.solve(p);
		if(u <= u_b13){
			return 1;
		}

		B23Curve<SpecificEnergy,Pressure,SOLVE_IENERGY> B23;
		SpecificEnergy u_b23 = B23.solve(p);
		if(u >= u_b23){
			return 2;
		}

		return 3;

	}catch(exception &E){
		stringstream s;
		s << "Solver2<p,u>::whichRegion(p = " << p/MPa << " MPa, u = " << u/kJ_kg << " kJ/kg): " << E.what();
		throw runtime_error(s.str());
	}
}

/**
	whichRegion given p, h
*/
template<>
int
Solver2<Pressure, SpecificEnergy, 0, SOLVE_ENTHALPY>::whichRegion(const Pressure &p, const SpecificEnergy &h){

	try{
		SteamCalculator S;

		S.set_pT(p,T_MAX);
		SpecificEnergy h_max = S.specenthalpy();

		if(h > h_max){
			stringstream ss;
			ss << "h=" << h << " exceeds maximum (h_max = " << h_max << " at p = " << p << ").";
			throw runtime_error(ss.str());
		}

		S.set_pT(p,T_TRIPLE);
		SpecificEnergy h_min = S.specenthalpy();
		if(h < h_min){
			stringstream ss;
			ss << "h=" << h << " is less than minumum (h_min = " << h_min << " at p = " << p << ").";
			throw runtime_error(ss.str());
		}


		S.setRegion3_rhoT(RHO_CRIT,T_CRIT);
		Pressure p_crit = S.pres();

		S.setSatWater_T(T_REG1_REG3);
		Pressure p_b1234 = S.pres();

		if(p <= p_crit){
			SatCurve<SpecificEnergy,Pressure,SOLVE_ENTHALPY> SC;
			SpecificEnergy h_f = SC.solve(p,SAT_WATER);
			SpecificEnergy h_g = SC.solve(p,SAT_STEAM);

			if(p <= p_b1234){
				if(h <= h_f){
					return 1;
				}

				if(h >= h_g){
					return 2;
				}
			}

			if(h >= h_f && h <= h_g){
				return 4;
			}
		}

		B13Curve<SpecificEnergy,Pressure,SOLVE_ENTHALPY> B13;
		SpecificEnergy h_b13 = B13.solve(p);
		if(h <= h_b13){
			return 1;
		}

		B23Curve<SpecificEnergy,Pressure,SOLVE_ENTHALPY> B23;
		SpecificEnergy h_b23 = B23.solve(p);
		if(h >= h_b23){
			return 2;
		}

		return 3;

	}catch(exception &E){
		stringstream s;
		s << "Solver2<p,h>::whichRegion(p = " << p/MPa << " MPa, h = " << h/kJ_kg << " kJ/kg): " << E.what();
		throw runtime_error(s.str());
	}

}

/**
	whichRegion given u, v
*/
template<>
int
Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0>::whichRegion(const SpecificEnergy &u, const SpecificVolume &v){

	//cerr << endl << "Solver2<u,v>: whichRegion(u = " << u << ", v = " << v << ")...";

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
				//cerr << endl << "Solver2<u,v>::whichRegion: u > u_b34: REGION = 3";
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

				//cerr << endl << "Solver2<u,v>::whichRegion: u > u_crit, v < v_b23_Tmax or u < u_b23: REGION = 3";
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
				throw runtime_error("Solver2<u,v>::whichRegion: Invalid u,v: u > u_b13_pmax but v < v_b13_pmax");
			}

			//cerr << endl <<"Solver2<u,v>::whichRegion: checking u <= u_b134:";

			if(u > u_b134){
				//cerr << endl <<"Solver2<u,v>::whichRegion: u > u_b134 ( = " << u_b134 << "): REGION = 3";
				return 3;
			}

			//cerr << "OK";

			//cerr << endl <<"Solver2<u,v>::whichRegion: v > v_b13_pmax, u <= u_b234: solve for v_b13";

			B13Curve<SpecificVolume, SpecificEnergy,0,SOLVE_IENERGY> B13C;
			v_b13 = B13C.solve(u);

			if(v <= v_b13){
				//cerr << endl <<"Solver2<u,v>::whichRegion: v <= v_b13: REGION = 3";
				return 3;
			}

			//cerr << endl << "Solver2<u,v>::whichRegion: v > v_b13: REGION = 1";
			return 1;
		}

	}catch(exception &E){
		stringstream s;
		s << "Solver2<u,v>::whichRegion(u = " << u / kJ_kg << " kJ/kg, v = " 
			<< v/m3_kg << " m^3/kg): " << E.what();
		throw runtime_error(s.str());
	}
}

/*
	@todo
		Make sure that one-way solvers are used wherever possible!

@code
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
			}catch(exception &E){
				stringstream s;
				s << "Solver2::solve: " << E.what();
				throw runtime_error(s.str());
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
@endcode
*/
