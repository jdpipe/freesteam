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

#include <string>
#include <sstream>

DiagramBase *
DiagramBase::newDiagram(const std::string &type){

	if(type=="Ts"){
		return new Diagram<SpecificEntropy, Temperature, SOLVE_ENTROPY>;

	}else if(type=="Tp" || type=="pT"){
		return new Diagram<Temperature, Pressure>;

	}else if(type=="ps"){
		return new Diagram<SpecificEntropy, Pressure, SOLVE_ENTROPY, 0>;

	}else if(type=="ph"){
		return new Diagram<SpecificEnergy, Pressure, SOLVE_ENTHALPY, 0>;

	}else if(type=="vu" || type=="uv"){
		return new Diagram<SpecificEnergy,SpecificVolume, SOLVE_IENERGY,0>;

	}else if(type=="Th"){
		return new Diagram<SpecificEnergy, Temperature, SOLVE_ENTHALPY, 0>;

	}else if(type=="Tv"){
		return new Diagram<SpecificVolume, Temperature>;
	}else{
		std::stringstream ss;
		ss << "Invalid diagram requested: " << type;
		throw new Exception(ss.str());
	}

}

