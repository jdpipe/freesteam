#include "steamcalculator.h"

#include "test.h"

#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

class CriticalPointTest : public CppUnit::TestFixture{

	public:

		void setUp(){
			S.setSatSteam_T(T_CRIT);
		}
		void tearDown(){}

	private:

	protected:

		SteamCalculator S;

		void testSetRhoT(){
			S.setRegion3_rhoT(RHO_CRIT,T_CRIT);
		}

		void testSetAsSteamT(){
			S.setSatSteam_T(T_CRIT);
			cerr << " (region = " << S.whichRegion() << ")";
		}

		/*void testSetAsSteamP(){
			S.setSatSteam_p(P_CRIT);
			cerr << " (region = " << S.whichRegion() << ")";
		}
		*/
		void testSetAsWaterT(){
			S.setSatWater_T(T_CRIT);
			cerr << " (region = " << S.whichRegion() << ")";
		}

		/*void testSetAsWaterP(){
			S.setSatWater_p(P_CRIT);
			cerr << " (region = " << S.whichRegion() << ")";
		}
		*/
		void testV(){
			SpecificVolume v = 0.00310559 * m3_kg;
			if(fabs(S.specvol() - v) > 0.00001 * m3_kg){
				stringstream s;
				s.flags(ios_base::showbase);
				s << "At C.P, got v = " << S.specvol() << ", expected " << v;
				CPPUNIT_FAIL(s.str());
			}
		}

		void testU(){
			SpecificEnergy u = 2015.73 * kJ_kg;
			if(fabs(S.specienergy() - u) > 4.0 * kJ_kg){
				stringstream s;
				s.flags(ios_base::showbase);
				s << "At C.P, got u = " << S.specienergy() << ", expected " << u;
				CPPUNIT_FAIL(s.str());
			}
		}

		void testH(){
			SpecificEnergy h = 2084.26 * kJ_kg;
			if(fabs(S.specenthalpy() - h) > 4.0 * kJ_kg){
				stringstream s;
				s.flags(ios_base::showbase);
				s << "At C.P, got h = " << S.specenthalpy() << ", expected " << h;
				CPPUNIT_FAIL(s.str());
			}
		}

		void testS(){
			SpecificEntropy s = 4.407 * kJ_kgK;
			if(fabs(S.specentropy() - s) > 5.0 * J_kgK){
				stringstream ss;
				ss.flags(ios_base::showbase);
				ss << "At C.P, got s = " << S.specentropy() << ", expected " << s;
				CPPUNIT_FAIL(ss.str());
			}
		}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(CriticalPointTest);
		CPPUNIT_TEST(testSetAsSteamT);
		CPPUNIT_TEST(testV);
		CPPUNIT_TEST(testU);
		CPPUNIT_TEST(testH);
		CPPUNIT_TEST(testS);
		/*
		CPPUNIT_TEST(testSetAsSteamP);
		CPPUNIT_TEST(testV);
		CPPUNIT_TEST(testU);
		CPPUNIT_TEST(testH);
		CPPUNIT_TEST(testS);
		*/
		CPPUNIT_TEST(testSetAsWaterT);
		CPPUNIT_TEST(testV);
		CPPUNIT_TEST(testU);
		CPPUNIT_TEST(testH);
		CPPUNIT_TEST(testS);

		CPPUNIT_TEST(testSetRhoT);
		CPPUNIT_TEST(testV);
		CPPUNIT_TEST(testU);
		CPPUNIT_TEST(testH);
		CPPUNIT_TEST(testS);

		/*
		CPPUNIT_TEST(testSetAsWaterP);
		CPPUNIT_TEST(testV);
		CPPUNIT_TEST(testU);
		CPPUNIT_TEST(testH);
		CPPUNIT_TEST(testS);
		*/
		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(CriticalPointTest);

