#include "../solver.h"
#include "steamtestpoint.h"

#include <sstream>

using namespace std;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#define TSOLVE_COMPARE(VAR) \
	if(!eq(S.temp(), T, maxerror)){ \
		stringstream s; \
		s << "Failed to solve " << #VAR << " = " << VAR << ", p = " << p << ": expected T = " << T/Kelvin<< "K but got " << S.temp()/Kelvin<< "K (error was " << fabs(S.temp() - T) << ", max is " << maxerror << ")"; \
		CPPUNIT_FAIL(s.str()); \
	}

class TemperatureSolverTest: public CppUnit::TestFixture{

	public:
		void tearDown() {
		}

		void setUP(){
		}

	protected:

		/**
			Tests to see if temperature solver for T( p, ???(p,T) ) gives back same value of T.
		*/
		void Test(Pressure p, Temperature T){
			try{
				SteamCalculator S;
				S.set_pT(p,T);
				SpecificEnergy u = S.specienergy();
				SpecificEntropy s = S.specentropy();
				SpecificVolume v = S.specvol();

				Temperature maxerror = 5e-5 * Kelvin;

				//Solving for T(u,p):
				Solver<Pressure,SpecificEnergy,Temperature> PS1(p,u);
				S = PS1.solve(0.00001 * kJ_kg, 1e-6 * Kelvin);
				TSOLVE_COMPARE(u);

				//Solving for T(s,p):
				Solver<Pressure,SpecificEntropy,Temperature> PS2(p,s);
				S = PS2.solve(0.00001 * kJ_kgK, 1e-6 * Kelvin);
				TSOLVE_COMPARE(s);

				//Solving for T(v,p):
				Solver<Pressure,SpecificVolume,Temperature> PS3(p,v);
				S = PS3.solve(0.00001 * metre3 / kilogram, 1e-7 * Kelvin);
				TSOLVE_COMPARE(v);

			}catch(Exception &e){
				stringstream s;
				s << "TemperatureSolverTest::Test: " << e.what();
				CPPUNIT_FAIL(s.str());
			}
		}

		void Test1(){ Test(10.0 * MPa,			fromcelsius(500.0)); }
		void Test2(){ Test(261.39 * bar,		fromcelsius(400.0)); }
		void Test3(){ Test(6.625945256E+04*kPa, fromcelsius(500.0)); }
		void Test4(){ Test(50.0 * bar,			fromcelsius(110.0)); }
		void Test5(){ Test(1.554939222E+03*kPa,	fromcelsius(300.0)); }
		void Test6(){ Test(3.489194196E+04*kPa,	fromcelsius(600.0)); }
		void Test7(){ Test(1.554939222E+03*kPa,	fromcelsius(201.0)); }
		void Test8(){ Test(1.554939222E+03*kPa,	fromcelsius(200.0)); }

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(TemperatureSolverTest);
		CPPUNIT_TEST(Test1);
		CPPUNIT_TEST(Test2);
		CPPUNIT_TEST(Test3);
		CPPUNIT_TEST(Test4);
		CPPUNIT_TEST(Test5);
		CPPUNIT_TEST(Test6);
		CPPUNIT_TEST(Test7);
		CPPUNIT_TEST(Test8);
		CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TemperatureSolverTest);
