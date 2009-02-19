#include "diagram.h"

#include <iostream>

int main(void){

	// Diagram<SpecificEnergy, Pressure,SOLVE_ENTHALPY,0> D;
	// Diagram<SpecificEnergy,SpecificVolume, SOLVE_IENERGY,0> D;
	// Diagram<Temperature, Pressure> D;
	Diagram<SpecificEntropy, Temperature, SOLVE_ENTROPY> D;

	cout << D.plot();

}
