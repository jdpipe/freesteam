#include "../steamcalculator.h"
#include "steampropertytest.h"
#include "test.h"

#include <sstream>
#include <string>

using namespace std;


class LowPressureTest : public CppUnit::TestFixture{

	public:

		void setUp(){}
		void tearDown(){}

	protected:

		SteamCalculator S;

		void testUV(Pressure p,Temperature T,SpecificEnergy u,SpecificVolume v){
			try{

				S.set_pT(p,T);

				SteamPropertyTest<SpecificVolume>::test( S, v, v * 0.01 * Percent);
				SteamPropertyTest<SpecificEnergy,SOLVE_IENERGY>::test( S, u,  90.0 * J_kg);

			}catch(Exception *E){
				CPPUNIT_FAIL(E->what());
			}
		}

		void test1(){
			testUV(1.0 * bar, fromcelsius(80), 334.9513061 * kJ_kg, 0.001029029 * m3_kg);
		}
		void test2(){
			testUV(1.0 * bar, fromcelsius(20), 83.90587432 * kJ_kg, 0.001001797* m3_kg);
		}
		void test3(){
			testUV(1.0 * bar, fromcelsius(30), 125.7208662* kJ_kg, 0.00100437* m3_kg);
		}
		void test4(){
			testUV(1.0 * bar, fromcelsius(50), 209.3161383* kJ_kg, 0.00101211* m3_kg);
		}
		void test5(){
			testUV(1.0 * bar, fromcelsius(70), 293.0191828* kJ_kg, 0.001022742* m3_kg);
		}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(LowPressureTest);
		CPPUNIT_TEST(test1);
		CPPUNIT_TEST(test2);
		CPPUNIT_TEST(test3);
		CPPUNIT_TEST(test4);
		CPPUNIT_TEST(test5);
		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(LowPressureTest);



