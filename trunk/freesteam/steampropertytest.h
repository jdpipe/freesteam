#ifndef STEAMPROPERTYTEST_H
#define STEAMPROPERTYTEST_H

#include "steamproperty.h"
#include "steamcalculator.h"
#include "test.h"

template<class Property,int PropertyAlternative=0>
class SteamPropertyTest : public SteamProperty<Property,PropertyAlternative>{

	public:
	
		static void test(SteamCalculator S,Property expected_value,Property tol){
			try{	
				Property calculated_value = get(S);
				if(fabs(calculated_value - expected_value) > tol){
					stringstream s;
					s.flags(ios_base::showbase);
					s << "Expected " << name() << " = " << expected_value;
					s << ", got " << name() << " = " << calculated_value;
					throw new Exception(s.str());
				}
			}catch(Exception *E){
				CPPUNIT_FAIL(E->what());
			}
		}
};

#endif
