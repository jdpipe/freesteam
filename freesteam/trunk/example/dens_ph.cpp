/*
freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2008  John Pye

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

#ifdef FREESTEAM_NOT_INSTALLED
# include "steamcalculator.h"
# include "solver.h"
# include "solver2.h"
#else
# include "freesteam/steamcalculator.h"
# include "freesteam/solver.h"
# include "freesteam/solver2.h"
#endif

#include <cstdlib>
#include <iostream>
#include <iomanip>
using namespace std;

int main(){
	try{
		// turn on display of units
		cout.flags(ios_base::showbase);

		SteamCalculator S1;

		// set desired values of enthalpy and pressure
		SpecificEnergy h = 2000. * kJ_kg;
		Pressure p = 5.0 * bar;

		// solve for the steam state these values
		Solver2<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY> SPH;
		S1 = SPH.solve(p,h);

		// calculate the temperature and density at this state
		Temperature T = S1.temp();
		Density rho = S1.dens();

		// output the results
		cout << "T = " << T;
		cout << " (" << tocelsius(T) << "°C)" << endl;
		cout << "rho = " << rho << endl;;
		exit(0);
	}catch(std::exception &E){
		cerr << "Error: " << E.what();
		exit(1);
	}
}

