#include "units.h"

#include <iostream>
using namespace std;

/// Tests units from result of division
int main(void){

	Pressure p1 = 1 * bar;
	Pressure p2 = 1 * MPa;
	
	Pressure p3 = p1/p2;
}
