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

#include "../steamcalculator.h"
#include "../exception.h"
#include "../units.h"

#include <iostream>
#include <iomanip>
using namespace std;

int main(void){

	int n=200;

	Temperature Tmin = 652.0 * Kelvin;
	Temperature Tmax = 654.0 * Kelvin;
	Temperature Tstep = (Tmax - Tmin) / double(n);

	SteamCalculator S;

	char tab = '\t';

	cout.flags(ios_base::showbase);
	cout << scientific << setprecision(8);

	cout << "T = [" << endl;
	for(Temperature T=Tmin; T <= Tmax; T+=Tstep){
		cout << tab << T/Kelvin;
	}
	cout << endl << "];" << endl;

	cout << "pbound = [" << endl;

	for(Temperature T=Tmin; T <= Tmax; T+=Tstep){
		try{

			Pressure p = Boundaries::getpbound_T(T);
			cout << tab << p/MPa;

		}catch(Exception &E){
			cerr << "Exception: " << E.what() << endl;
			cout << tab << "0";
		}

		cout << endl;
	}
	cout << "];" << endl;

	cout << "plot(pbound,T);" << endl;
	cout << "xlabel('pressure / MPa');ylabel('temperature / K');" << endl;

}
