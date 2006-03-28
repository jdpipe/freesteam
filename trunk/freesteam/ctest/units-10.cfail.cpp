#include "../units.h"

// test of square root
int main(void){

	Area m1 = 100.0 * metre2;
	Length l = Area::sqrt(m1);
	Length l = Length::sqrt(2.0 * metre);
}
