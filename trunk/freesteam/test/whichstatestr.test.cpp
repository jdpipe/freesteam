#include "../steamcalculator.h"
#include "test.h"

#include <sstream>
#include <string>
#include <cstring>
using namespace std;

class WhichStateStrTest : public CppUnit::TestFixture{

	private:
		SteamCalculator S;

	protected:

		void checkState(SteamCalculator &S,const string expected){
			if(expected != S.whichStateStr()){
				stringstream s;
				s << "For p = " << S.pres() << ", T = " << S.temp() << ", x = " << S.quality();
				s << ", expected state was " << expected;
				s << " but actual value was " << S.whichStateStr();

				CPPUNIT_FAIL(s.str());
			}
		}

		void testSaturated(){
			try{

			S.setSatSteam_T(fromcelsius(100));
			checkState(S,"SAT STEAM");

			S.setSatWater_T(fromcelsius(100));
			checkState(S,"SAT LIQUID");

			S.setSatSteam_T(fromcelsius(370));
			checkState(S,"SAT STEAM");

			S.setSatWater_T(fromcelsius(370));
			checkState(S,"SAT LIQUID");

			S.setRegion4_Tx(fromcelsius(100),0.4);
			checkState(S,"SATURATED");

			S.setRegion4_Tx(T_TRIPLE,0.6);
			checkState(S,"SATURATED");

			S.setSatSteam_T(T_REG1_REG3);
			checkState(S,"SAT STEAM");

			S.setSatWater_T(T_REG1_REG3);
			checkState(S,"SAT LIQUID");

			S.setSatSteam_T(T_CRIT);
			checkState(S,"SATURATED");

			S.setSatWater_T(T_CRIT);
			checkState(S,"SATURATED");

			}catch(Exception *E){
				stringstream s;
				s << "WhichStateStrTest:testSaturated: " << E->what();
				CPPUNIT_FAIL(s.str());
			}
		}

		void testSupercritical(){
			try{

			S.set_pT(25.0 * MPa,fromcelsius(400));
			checkState(S,"SUPERCRITICAL");

			}catch(Exception *E){
				stringstream s;
				s << "WhichStateStrTest:testSupercritical: " << E->what();
				CPPUNIT_FAIL(s.str());
			}
		}

		void testSubcooled(){
			try{
			S.set_pT(1.0 * bar,T_TRIPLE,0.0);
			checkState(S,"SUBCOOLED");

			S.set_pT(1.0 * bar, fromcelsius(20), 0.0);
			checkState(S,"SUBCOOLED");

			S.set_pT(10.0 * MPa, fromcelsius(300), 0.0);
			checkState(S,"SUBCOOLED");

			S.set_pT(P_MAX, fromcelsius(100),0.0);
			checkState(S,"SUBCOOLED");
			}catch(Exception *E){
				stringstream s;
				s << "WhichStateStrTest:testSubcooled: " << E->what();
				CPPUNIT_FAIL(s.str());
			}
		}

		void testSuperheated(){
			try{
			S.set_pT(P_MAX, T_MAX, 1.0);
			checkState(S,"SUPERHEATED");

			S.set_pT(P_MIN, T_MAX, 1.0);
			checkState(S,"SUPERHEATED");

			S.set_pT(6.0 * MPa,fromcelsius(280),1.0);
			checkState(S,"SUPERHEATED");
			}catch(Exception *E){
				stringstream s;
				s << "WhichStateStrTest:testSuperheated: " << E->what();
				CPPUNIT_FAIL(s.str());
			}
		}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(WhichStateStrTest);
		CPPUNIT_TEST(testSaturated);
		CPPUNIT_TEST(testSubcooled);
		CPPUNIT_TEST(testSuperheated);
		CPPUNIT_TEST(testSupercritical);
		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(WhichStateStrTest);

