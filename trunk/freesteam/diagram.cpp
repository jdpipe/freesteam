#include "diagram.h"

// Intentionally empty

/*
	This file is to be used for template method overrides where more detail is needed on particular plots, etc etc.
*/

const std::string
Diagram<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0>::getMatlabPlotType(){
	cerr << endl << "DIAGRAM<u,v>::getMatlabPlotType() = semilogx";
	return "semilogx";
}

const std::string
Diagram<SpecificEnergy,Pressure,SOLVE_ENTHALPY,0>::getMatlabPlotType(){
	return "semilogx";
}
