#include "../units.h"

int main(void){

	Pressure p = 1.0 * bar;
	Pressure p2 = 0.001 * MPa;

	Pressure p3 = p + p2;
}
