#ifndef SOLVER2_H
#define SOLVER2_H

#include "units.h"
#include "satcurve.h"
#include "exception.h"

/**
	Base class for all property solvers
*/
template<class FirstProp,class SecondProp,int FirstPropAlternative=0, int SecondPropAlternative=0>
class Solver2Base{

	protected:
	
		int whichRegion(FirstProp fp,SecondProp sp);
		
		virtual SteamCalculator solve(FirstProp fp, SecondProp sp){
			throw new Exception("Solver2Base::solve must be over-ridden");
		}
	
		FirstProp getFirstProp(SteamCalculator &S){
			return SteamProperty<FirstProp,FirstPropAlternative>::get(S);
		}
		
		SecondProp getSecondProp(SteamCalculator &S){
			return SteamProperty<SecondProp,SecondPropAlternative>::get(S);
		}
		
		virtual SteamCalculator solveRegion1(FirstProp fp, SecondProp sp){
			// If we are in region 1, then we will be iterating with pressure and temperature
			
		}
		

	}
}

/**
	Solver class for the steam tables
	
	This is intended to be a general purpose way of defining steam state in terms of any combination of properties, eg to find the pressure at which rho = 1.1 kg/m3 and u = 2500 kJ/kg, use:
	
		Solver2<Density,SpecificEnergy> SS;
		SteamCalculator S = SS.solve(1.1 * kg_m3, 2500.0 * kJ_kg);
		cerr << S->pres() << endl;
		
	Likewise, even with one of the properties being a correlation property, just use
	
		Solver2<Temperature,SpecificEnergy> SS;
		SteamCalculator S = SS.solve(450.0 * Kelvin, 2500.0 * kJ_kg);
		cerr << S->pres() << endl;

	Or even,
	
		Solver2<Temperature,Pressure> SS;
		SteamCalculator S = SS.solve(450.0 * Kelvin, 10.0 * bar);
		cerr << S->dens() << endl;
	
*/		
template<class FirstProp,class SecondProp,int FirstPropAlternative=0, int SecondPropAlternative=0>
class Solver2 
	: public Solver2Base<FirstProp,SecondProp,FirstPropAlternative,SecondPropAlternative>{
	
	...
	
};


#endif
