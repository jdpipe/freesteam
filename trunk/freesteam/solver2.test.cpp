#include "solver2.h"
#include "steampropertytest.h"
#include "test.h"

#include <sstream>
#include <string>

using namespace std;


class Solver2Test : public CppUnit::TestFixture{
			
	public:

		void setUp(){}
		void tearDown(){}

	protected:

		SteamCalculator S;
				
		void testUV(Pressure p,Temperature T){
			try{
				SteamCalculator S;
				S.set_pT(p,T);
				SpecificEnergy u = S.specienergy();
				SpecificVolume v = S.specvol();
				
				//cerr << endl << endl << "Solver2Test:TestUV: Testing with p = " << p << ", T = " << T;
				//cerr << endl << "Solver2Test:TestUV: Expecting u = " << u << ", v = " << v;

				Solver2<SpecificEnergy,SpecificVolume> SS;
				S = SS.solve( u, v);
				
				
				SteamPropertyTest<Pressure>::test( S, p, 0.01 * bar);
				SteamPropertyTest<Temperature>::test( S, T, 0.01 * Kelvin);
							
			}catch(Exception *E){
				CPPUNIT_FAIL(E->what());
			}
		}
		
		void test1(){
			Temperature Tmin = fromcelsius(0);
			Temperature Tmax = T_REG1_REG3;
			Temperature Tstep = (Tmax - Tmin) / double(20);
			for(Temperature T = Tmin + Tstep * 0.5; T <= Tmax ; T += Tstep){
				
				Pressure pmin = Boundaries::getSatPres_T(T);
				Pressure pmax = P_MAX;
				Pressure pstep = (pmax - pmin) / double(20);
			
				for(Pressure p = pmin + pstep * 0.5 ; p <= pmax; p +=pstep){
					testUV(p,T);
				}
			}
		}
		
	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(Solver2Test);
		CPPUNIT_TEST(test1);
		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test);


