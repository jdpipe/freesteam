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
#include "solver.h"
#include "solver2.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main(){
	try{
		SteamCalculator S1, S2, S3;

		// turn on display of units
		cerr.flags(ios_base::showbase);

		// initialise T1, p1, p2
		Temperature T1 = fromcelsius(200.0);
		Pressure p1 = 5.0 * bar;
		Pressure p2 = 10.0 * bar;

		// Part 1
		cerr << endl << "Part (1) - density at 10 bar, 200°C" << endl;
		cerr << "p1 = " << p1/bar << " bar" << endl;
		cerr << "T1 = " << T1;
		cerr << " (" << tocelsius(T1) << "°C)" << endl;
		S1.set_pT(p1, T1);
		Density rho1 = S1.dens();
		cerr << "rho1 = " << rho1 << endl;

		// Part 2
		cerr << endl << "Part (2) - isentropic compression to 10 bar" << endl;
		SpecificEntropy s1 = S1.specentropy();
		cerr << "s1 = " << s1 << endl;
		cerr << "p2 = " << p2/bar << " bar" << endl;
		Solver<Pressure,SpecificEntropy,Temperature> PS(p2, s1);
		S2 = PS.solve(0.0001 * kJ_kgK, 0.0001 * Kelvin);
		Temperature T2 = S2.temp();
		cerr << "T2 = " << T2;
		cerr << " (" << tocelsius(T2) << "°C)" << endl;

		// part (3) - Finding p,T for v as above and u increased by 200 kJ_kg
		cerr << endl << "Part (3) - Finding p,T for v as above and u increased by 200 kJ_kg" << endl;
		SpecificVolume v2 = S2.specvol();
		SpecificEnergy u2 = S2.specienergy();
		cerr << "v2 = " << v2 << endl;
		cerr << "u2 = " << u2 << endl;

		SpecificEnergy u3 = u2 + 200.0 * kJ_kg;
		cerr << "u3 = " << u3 << endl;

		Solver2<SpecificEnergy,SpecificVolume> SUV;
		S3 = SUV.solve(u3,v2);
		Temperature T3 = S3.temp();
		Pressure p3 = S3.pres();
		cerr << "p3 = " << p3/bar << " bar" << endl;
		cerr << "T3 = " << T3;
		cerr << " (" << tocelsius(T3) << "°C)" << endl;

		exit(0);

	}catch(Exception &E){
		cerr << "Error: " << E.what();
		exit(1);
	}
}
