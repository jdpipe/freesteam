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

#define PSOLVE_COMPARE(VAR) \
	if(!eq(S.pres(), p, maxerror)){ \
		stringstream s; \
		s << "Failed to solve " << #VAR << " = " << VAR << ", T = " << T << ": expected p = " << p/Pascal << "Pa but got " << S.pres()/Pascal << "Pa (error was " << fabs(S.pres() - p) << ", max is " << maxerror << ")"; \
		CPPUNIT_FAIL(s.str()); \
	}

class PressureSolverTest: public CppUnit::TestFixture{

	public:
		void tearDown() {
			cerr << " (Note: no values in saturated zone have been tested!)";
		}

		void setUP(){
		}

	protected:

		/**
			Test to see if pressure solver for T,h(p,T) gives back same value of p.
		*/
		void Test(Pressure p, Temperature T){
			try{
				SteamCalculator S;
				S.set_pT(p,T);
				SpecificEnergy u = S.specienergy();
				SpecificEntropy s = S.specentropy();
				SpecificVolume v = S.specvol();

				Pressure maxerror = 0.5 * Pascal;

				//Solving for p(u,T):
				Solver<Temperature,SpecificEnergy,Pressure> PS1(T,u);
				S = PS1.solve(0.00001 * kJ_kg, 0.1 * Pascal);
				PSOLVE_COMPARE(u);

				//Solving for p(s,T):
				Solver<Temperature,SpecificEntropy,Pressure>  PS2(T,s);
				S = PS2.solve(0.00001 * kJ_kgK, 0.1 * Pascal);
				PSOLVE_COMPARE(s);

				//Solving for p(v,T):
				Solver<Temperature,SpecificVolume,Pressure>  PS3(T,v);
				S = PS3.solve(0.00001 * metre3 / kilogram , 0.1 * Pascal);
				PSOLVE_COMPARE(v);

			}catch(Exception *e){
				stringstream s;
				s << "PressureSolverTest::Test: " << e->what();
				CPPUNIT_FAIL(s.str());
			}
		}

		void Test1(){ Test(10.0 * MPa,			fromcelsius(500.0)); }
		void Test2(){ Test(261.39 * bar,		fromcelsius(400.0)); }
		void Test3(){ Test(6.625945256E+04*kPa, fromcelsius(500.0)); }
		void Test4(){ Test(1.0 * bar,			fromcelsius(100.0)); }
		void Test5(){ Test(1.554939222E+03*kPa,	fromcelsius(300.0)); }
		void Test6(){ Test(3.489194196E+04*kPa,	fromcelsius(600.0)); }
		void Test7(){ Test(1.554939222E+03*kPa,	fromcelsius(201.0)); }
		void Test8(){ Test(1.554939222E+03*kPa,	fromcelsius(200.0)); }

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(PressureSolverTest);
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

CPPUNIT_TEST_SUITE_REGISTRATION(PressureSolverTest);
