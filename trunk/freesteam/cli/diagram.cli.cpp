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

#include "diagram.h"
#include "../exception.h"
#include <stdlib.h>

#include <iostream>

int main(void){

	try{
		// Diagram<SpecificEnergy, Pressure,SOLVE_ENTHALPY,0> D;
		// Diagram<SpecificEnergy,SpecificVolume, SOLVE_IENERGY,0> D;
		// Diagram<Temperature, Pressure> D;
		Diagram<SpecificEntropy, Temperature, SOLVE_ENTROPY> D;

		cout << D.plot();
	}catch(Exception *E){
		cerr << "main: " << E->what();
		delete E;
		exit(1);
	}
}
