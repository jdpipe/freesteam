#include "diagram.h"

#include <iostream>

int main(void){

	Diagram<SpecificEnergy,SpecificVolume, SOLVE_IENERGY,0> D;

	cout << D.plot();

}
