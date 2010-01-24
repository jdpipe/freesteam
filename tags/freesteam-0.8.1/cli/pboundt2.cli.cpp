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

#include "../b23curve.h"
#include "../steamcalculator.h"
#include <stdexcept>
#include "../units.h"

#include <iostream>
#include <iomanip>
using namespace std;

int main(void){

	int n=30;

	Temperature Tmin = TB_LOW;
	Temperature Tmax = TB_HIGH;
	Temperature Tstep = (Tmax - Tmin) / double(n);

	SteamCalculator S;

	char tab = '\t';

	cout.flags(ios_base::showbase);
	cout << scientific << setprecision(8);

	cout        << "T / K"       << tab << "p / MPa"     << tab << "u / kJ/kg";
	cout << tab << "h / kJ/kg"   << tab << "s / kJ/kgK"  << tab << "v / m3/kg";
	cout << tab << "cp / kJ/kgK" << tab << "cv / kJ/kgK" << endl;

	for(Temperature T=Tmin; T <= Tmax + Tstep * 0.01; T+=Tstep){
		try{

			cout << T << tab;

			Pressure p = Boundaries::getpbound_T(T);
			SteamCalculator S;
			S.set_pT(p,T);
			SpecificEnergy u = S.specienergy();
			SpecificEnergy h = S.specenthalpy();
			SpecificEntropy s = S.specentropy();
			SpecificVolume v = S.specvol();
			SpecHeatCap cp = S.speccp();
			SpecHeatCap cv = S.speccv();

			cout <<                               p           << tab << u  ;
			cout << tab << h            << tab << s           << tab << v ;
			cout << tab << cp           << tab << cv          << endl;

		}catch(std::exception &E){
			cerr << "Exception: " << E.what() << endl;
		}
	}

}

