#include "../units.h"

#include <iostream>
using namespace std;

/// Test of addition of unitful value to double
int main(void){

	Pressure p = 1.0 * bar;
	double d = 5;

	Pressure p2 = p + d;

}
