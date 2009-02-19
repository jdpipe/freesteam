#ifndef STEAMPROPERTYTEST_H
#define STEAMPROPERTYTEST_H

#include "steamproperty.h"
#include "steamcalculator.h"
#include "test.h"

template<class Property,int PropertyAlternative=0>
class SteamPropertyTest : public SteamProperty<Property,PropertyAlternative>{

	public:

		static void test(SteamCalculator S,Property expected_value,Property tol){

			Property calculated_value;

			try{
				calculated_value = SteamProperty<Property,PropertyAlternative>::get(S);
			}catch(Exception *E){
				stringstream s;
				s << "SteamPropertyTest::test: " << E->what();
				throw new Exception(s.str());
			}

			if(fabs(calculated_value - expected_value) > tol){
				stringstream s;
				s.flags(ios_base::showbase);
				s << "Expected " << SteamProperty<Property,PropertyAlternative>::name() << " = " << expected_value;
				s << ", got " << SteamProperty<Property,PropertyAlternative>::name() << " = " << calculated_value;
				throw new Exception(s.str());
			}
		}
};

#endif
