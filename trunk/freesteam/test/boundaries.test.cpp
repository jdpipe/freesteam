#include "../steamcalculator.h"
#include "../batchtest.h"
#include "../steamtestpoint.h"
#include "../exception.h"

#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

class BoundariesTest : public CppUnit::TestFixture {

	private:

		void testRegion1(){

			try{

				SteamCalculator S;

				S.set_pT(3.0 * MPa,300.0 * Kelvin, 0.0);
				CPPUNIT_ASSERT_EQUAL_MESSAGE("#1: region is incorrect",1,S.whichRegion());

				S.set_pT(80.0 * MPa,300.0 * Kelvin, 0.0);
				CPPUNIT_ASSERT_EQUAL_MESSAGE("#2: region is incorrect",1,S.whichRegion());

				S.set_pT(3.0 * MPa,500.0 * Kelvin, 0.0);
				CPPUNIT_ASSERT_EQUAL_MESSAGE("#3: region is incorrect",1,S.whichRegion());

			}catch(Exception *e){
				CPPUNIT_FAIL(e->what());
			}
		}

		void testRegion2(){

			try{

			SteamCalculator S;

			S.set_pT(0.0035 * MPa,300.0 * Kelvin, 1.0);
			CPPUNIT_ASSERT_EQUAL(2,S.whichRegion());

			S.set_pT(0.0035 * MPa,700.0 * Kelvin, 1.0);
			CPPUNIT_ASSERT_EQUAL(2,S.whichRegion());

			S.set_pT(30.0 * MPa,700.0 * Kelvin, 1.0);
			CPPUNIT_ASSERT_EQUAL(2,S.whichRegion());

			}catch(Exception *e){
				CPPUNIT_FAIL(e->what());
			}
		}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(BoundariesTest);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);

		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp(){

		}

		void tearDown() {

		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(BoundariesTest);
