#include "units.h"
#include "test.h"

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

class UnitsTest : public CppUnit::TestFixture {
	
	protected:

		void testUnits();

	public:
		void setUp(){}
		
		void tearDown(){}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(UnitsTest);
		CPPUNIT_TEST(testUnits);
		CPPUNIT_TEST_SUITE_END();
	
};

CPPUNIT_TEST_SUITE_REGISTRATION(UnitsTest);

void
UnitsTest::testUnits(){

		cerr.flags(ios_base::showbase);
	
		Velocity v = 60.0 * kilometre / hour;
		
		Length x = 250.0 * kilometre;
		
		Time t = x / v;
		
		//cerr << "The time taken to travel " << x << " at speed " << v << " is " << t << endl;
		
		Velocity vv = 10.0 * kilometre / hour;
		
		//cerr << "What about if the speed is " << vv << " faster?" << endl;
		
		v+=vv;
		
		//cerr << "ie " << v << endl;
		
		Time t2 = x / v;
		
		//cerr << "Then it takes " << t << " to travel  " << x << endl;
		
		//cerr << "That's an improvements of " << t-t2 << endl;
		
		//cerr << "Or " << (t-t2)/hour << " h" << endl;

		//cerr << "Or " << (t-t2)/minute<< " min" << endl;
		
		//cerr << " You can't do the following:" << endl;
		//Time t3 = v * v;

}
