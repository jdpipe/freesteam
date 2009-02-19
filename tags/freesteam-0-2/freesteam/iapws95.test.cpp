
// Unit tests for Water95 class.  Returns 0 if successful.

#include "iapws95.h"
using namespace IAPWS95;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class IAPWS95Test : public CppUnit::TestFixture{

	/**
		Get percent different between parameter values
	*/
	static Num PctDiff(const Num & a, const Num & b)
	{
		if (a==0 && b==0){
			return 0.0;
		}

		return fabs(100.0*(a-b)/b);
	}


	void TestIdealGasPart()
	{
		// Test that we match Table 6 values.  Note the phi0tautau
		// expected value is negative, as it has to be, although the
		// table gives it as positive.
		Num eps = 0.0001;
		Num T = 500;       // K
		Num rho = 838.025; // kg/m^3
		Water95 w;
		CPPUNIT_ASSERT(PctDiff(phi0(          rho/w.rhoc, w.Tc/T),  0.204797734e+1) < eps);
		CPPUNIT_ASSERT(PctDiff(phi0delta(     rho/w.rhoc, w.Tc/T),  0.384236747e+0) < eps);
		CPPUNIT_ASSERT(PctDiff(phi0deltadelta(rho/w.rhoc, w.Tc/T), -0.147637878e+0) < eps);
		CPPUNIT_ASSERT(PctDiff(phi0tau(       rho/w.rhoc, w.Tc/T),  0.904611106e+1) < eps);
		CPPUNIT_ASSERT(PctDiff(phi0tautau(    rho/w.rhoc, w.Tc/T), -0.193249185e+1) < eps);
		CPPUNIT_ASSERT(PctDiff(phi0deltatau(  rho/w.rhoc, w.Tc/T),  0.000000000e+0) < eps);
	}

	void TestRealGasPart()
	{
		// Test that we match Table 6 values
		Num eps = 0.0001;
		Num T = 500;       // K
		Num rho = 838.025; // kg/m^3
		Water95 w;
		CPPUNIT_ASSERT(PctDiff(phir(          rho/w.rhoc, w.Tc/T), -0.342693206e+1) < eps);
		CPPUNIT_ASSERT(PctDiff(phirdelta(     rho/w.rhoc, w.Tc/T), -0.364366650e+0) < eps);
		CPPUNIT_ASSERT(PctDiff(phirdeltadelta(rho/w.rhoc, w.Tc/T),  0.856063701e+0) < eps);
		CPPUNIT_ASSERT(PctDiff(phirtau(       rho/w.rhoc, w.Tc/T), -0.581403435e+1) < eps);
		CPPUNIT_ASSERT(PctDiff(phirtautau(    rho/w.rhoc, w.Tc/T), -0.223440737e+1) < eps);
		CPPUNIT_ASSERT(PctDiff(phirdeltatau(  rho/w.rhoc, w.Tc/T), -0.112176915e+1) < eps);
	}

	void TestSinglePhaseRegion()
	{
		// Values from Table 7
		Num values[11][6] = {
			//Temp   Density       Pressure     Isochoric Sp. Ht  Spd of Sound       Entropy
			// K     kg/m^3           MPa          kJ/(kg*K)          m/s           kJ/(kg*K)
			// T       rho             p              cv               w                s
			{300.0, 0.9965560e+3, 0.99242e-1    , 0.413018111e+1, 0.150151914e+4, 0.393062642e+0},
			{300.0, 0.1005308e+4, 0.200022514e+2, 0.406798347e+1, 0.153492501e+4, 0.387405401e+0},
			{300.0, 0.1188202e+4, 0.700004704e+3, 0.346135580e+1, 0.244357992e+4, 0.132609616e+0},
			{500.0, 0.4350000e+0, 0.999679423e-1, 0.150817541e+1, 0.548314253e+3, 0.794488271e+1},
			{500.0, 0.4532000e+1, 0.999938125e+0, 0.166991025e+1, 0.535739001e+3, 0.682502725e+1},
			{500.0, 0.8380250e+3, 0.100003858e+2, 0.322106219e+1, 0.127128441e+4, 0.256690918e+1},
			{500.0, 0.1084564e+4, 0.700000405e+3, 0.307437693e+1, 0.241200877e+4, 0.203237509e+1},
			{647.0, 0.3580000e+3, 0.220384756e+2, 0.618315728e+1, 0.252145078e+3, 0.432092307e+1},
			{900.0, 0.2410000e+0, 0.100062559e+0, 0.175890657e+1, 0.724027147e+3, 0.916653194e+1},
			{900.0, 0.5261500e+2, 0.200000690e+2, 0.193510526e+1, 0.698445674e+3, 0.659070225e+1},
			{900.0, 0.8707690e+3, 0.700000006e+3, 0.266422350e+1, 0.201933608e+4, 0.417223802e+1}
		};
		for (int i = 0; i < 11; i++)
		{
			Water95 w;
			Num T, rho, p_e, cv_e, w_e, s_e, eps;
			T    = values[i][0];
			rho  = values[i][1];
			p_e  = values[i][2];
			cv_e = values[i][3];
			w_e  = values[i][4];
			s_e  = values[i][5];
			
			if (T == 647.0)
				eps = 0.2; // Precision at the critical point is much less
			else
				eps = 0.0002;
			
			CPPUNIT_ASSERT(PctDiff(w.p(T, rho), p_e) < eps);
			CPPUNIT_ASSERT(PctDiff(w.cv(T, rho), cv_e) < eps);
			CPPUNIT_ASSERT(PctDiff(w.w(T, rho), w_e) < eps);
			CPPUNIT_ASSERT(PctDiff(w.s(T, rho), s_e) < eps);
		}

		// Tests for u and h came from NIST webpage at
		// http://webbook.nist.gov/chemistry/fluid/
		Num values1[6][5] = {
			// T    rho         u       h        cp
			{350, 973.70,     321.75, 321.79,  4.1946},  // Liquid
			{400, 937.49,     532.69, 532.95,  4.2555},  // Liquid
			{440, 900.65,     704.50, 705.31,  4.3571},  // Liquid
			{350,   0.26029, 2477.6, 2637.7,   2.0033},  // Vapor
			{400,   1.3694,  2536.2, 2715.7,   2.2183},  // Vapor
			{500,   4.5320,  2670.6, 2891.2,   2.2795}   // Vapor
		};
		for (int i = 0; i < 6; i++)
		{
			Water95 w;
			Num T, rho, u_e, h_e, cp_e;
			Num eps = 0.0025;
			T    = values1[i][0];
			rho  = values1[i][1];
			u_e  = values1[i][2];
			h_e  = values1[i][3];
			cp_e = values1[i][4];
			CPPUNIT_ASSERT(PctDiff(w.u(T, rho), u_e) < eps);
			CPPUNIT_ASSERT(PctDiff(w.h(T, rho), h_e) < eps);
			CPPUNIT_ASSERT(PctDiff(w.cp(T, rho), cp_e) < eps);
		}
	}

	public:
		void setUp(){
		}
		
		void tearDown(){
		}
	
	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(IAPWS95Test);
		CPPUNIT_TEST(TestIdealGasPart);
		CPPUNIT_TEST(TestRealGasPart);
		CPPUNIT_TEST(TestSinglePhaseRegion);
		CPPUNIT_TEST_SUITE_END();
	
};

CPPUNIT_TEST_SUITE_REGISTRATION(IAPWS95Test);
