#include "diagram.h"

// Intentionally empty

/*
	This file is to be used for template method overrides where more detail is needed on particular plots, etc etc.
*/

template <>
const std::string
Diagram<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0>::getMatlabPlotType(){
	return "semilogy";
}

template <>
const std::string
Diagram<SpecificEnergy,Pressure,SOLVE_ENTHALPY,0>::getMatlabPlotType(){
	return "semilogy";
}
