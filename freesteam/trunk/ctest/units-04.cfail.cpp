#include "../units.h"

#include <iostream>
using namespace std;

/// Test of units cancelling to give double
int main(void){

	Temperature T;

	cerr << endl << "PERFORMING EXPR: " << endl;

	double upsilon = T + 13.5 / (T - 23.5);
}
