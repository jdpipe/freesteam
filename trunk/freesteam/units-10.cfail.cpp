#include "units.h"

#include <iostream>
using namespace std;

/// Test of addition on different units
int main(void){

	Pressure p = 1.0 * bar;
	Distance d = 1.2 * metre;
	
	Pressure p2 = p + d;
	
}
