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
*//*
	Example of using freesteam to calculate the temperature of
	steam after isentropic decompression, following email enquiry from 
	Maxim Belov.
*/

#ifdef FREESTEAM_NOT_INSTALLED
# include "steamcalculator.h"
# include "solver2.h"
#else
# include "freesteam/steamcalculator.h"
# include "freesteam/solver2.h"
#endif

#include <cstdlib>
#include <iostream>
#include <iomanip>
using namespace std;

int main(){
	try{
		// turn on display of units
		cerr.flags(ios_base::showbase);

		SteamCalculator S1, S2;

		Solver2<Pressure,SpecificEntropy,0,SOLVE_ENTROPY> PS;

		/* first example from Maxim */

		cerr << "From 14 bar, 250 C ---> 6 bar" << endl;
		S1.set_pT(14.*bar, fromcelsius(250.));

		cerr << "T1 = " << S1.temp() << " = " << tocelsius(S1.temp()) << " °C" << endl;;
		cerr << "p1 = " << S1.pres() << endl;
		
		S2 = PS.solve(6.*bar, S1.specentropy());

		cerr << "T2 = " << S2.temp() << " = " << tocelsius(S2.temp()) << " °C" << endl;;
		cerr << "p2 = " << S2.pres() << endl;

		cerr << "From 10 bar, 191 C ---> 4 bar" << endl;
		S1.set_pT(10.*bar, fromcelsius(191.));

		cerr << "T1 = " << S1.temp() << " = " << tocelsius(S1.temp()) << " °C" << endl;;
		cerr << "p1 = " << S1.pres() << endl;
		
		S2 = PS.solve(4.*bar, S1.specentropy());

		cerr << "T2 = " << S2.temp() << " = " << tocelsius(S2.temp()) << " °C" << endl;;
		cerr << "p2 = " << S2.pres() << endl;


		exit(0);

	}catch(std::exception &E){
		cerr << "Error: " << E.what();
		exit(1);
	}
}
