#include "steamcalculator.h"
#include "steampropertytest.h"

#include "test.h"

#include <sstream>
#include <string>

using namespace std;

class B234Point : public CppUnit::TestFixture{
			
	public:

		void setUp(){}
		void tearDown(){}

	protected:
		
		void checkValues(SteamCalculator &S){
				SteamPropertyTest<SpecificEnergy,SOLVE_IENERGY>::test( S, 2418.138482 * kJ_kg,  0.1 * kJ_kg);
				SteamPropertyTest<SpecificEnergy,SOLVE_ENTHALPY>::test(S, 2563.636711 * kJ_kg,  0.1 * kJ_kg);
				SteamPropertyTest<SpecificEntropy,SOLVE_ENTROPY>::test(S, 5.21100158  * kJ_kgK, 0.1 * kJ_kgK);
				SteamPropertyTest<SpecificVolume,0>::test(             S, 0.008802382 * m3_kg,  0.000003 * m3_kg);
		}
		
		void testWithSatSteam(){
			
			try{
				//cerr << endl << "Setting sat steam at TB_LOW... ";
				SteamCalculator S;
				S.setSatSteam_T(TB_LOW);
				
				//cerr << endl << "Region is apparently " << S.whichRegion() << ". ";

				//cerr << endl << "Verifying u, h, s, v... ";
				checkValues(S);
				
			}catch(Exception *E){
				CPPUNIT_FAIL(E->what());
			}
		}
		
		void testWithB23(){
			
			try{
				//cerr << endl << "Setting sat steam at TB_LOW... ";
				SteamCalculator S;
				S.setB23_T(TB_LOW);
				
				//cerr << endl << "Region is apparently " << S.whichRegion() << ". ";

				//cerr << endl << "Verifying u, h, s, v... ";
				checkValues(S);

			}catch(Exception *E){
				CPPUNIT_FAIL(E->what());
			}
		}
	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(B234Point);
		CPPUNIT_TEST(testWithSatSteam);
		CPPUNIT_TEST(testWithB23);
		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(B234Point);

