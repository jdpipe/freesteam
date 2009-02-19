#include "test.h"
#include "steamcalculator.h"

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

class EmptyTest : public CppUnit::TestFixture {
	
	private:
		SteamCalculator S;
		
	protected:
	
		void testSetRead() {
			
			Pressure p = 1 * bar;
			Temperature T = fromcelsius(25);
			
			S.set_pT(p,T);
			
			CPPUNIT_ASSERT(p==S.pres());
			CPPUNIT_ASSERT(T==S.temp());
			
		}

	public:
		void setUp(){}
		
		void tearDown(){}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(EmptyTest);
		CPPUNIT_TEST(testSetRead);
		CPPUNIT_TEST_SUITE_END();
	
};

CPPUNIT_TEST_SUITE_REGISTRATION(EmptyTest);
