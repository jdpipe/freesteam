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
				
		void testUV(Pressure p,Temperature T,bool verbose=false){
			try{
				SteamCalculator S;
				S.set_pT(p,T);
				SpecificEnergy u = S.specienergy();
				SpecificVolume v = S.specvol();
				
				if(verbose){
					cerr << endl << endl << "Solver2Test:TestUV: Testing with p = " << p << ", T = " << T;
					cerr << endl << "Solver2Test:TestUV: Expecting u = " << u << ", v = " << v;
				}

				Solver2<SpecificEnergy,SpecificVolume> SS;
				S = SS.solve( u, v);
				
				SteamPropertyTest<Pressure>::test( S, p, 0.01 * bar);
				SteamPropertyTest<Temperature>::test( S, T, 0.01 * Kelvin);
							
			}catch(Exception *E){
				stringstream s;
				s << "Solver2Test::testUV: " << E->what();
				CPPUNIT_FAIL(s.str());
			}
		}
		
		void testRegion1(){
			Temperature Tmin = fromcelsius(0);
			Temperature Tmax = T_REG1_REG3;
			Temperature Tstep = (Tmax - Tmin) / double(10);
			for(Temperature T = Tmin + Tstep * 0.5; T <= Tmax ; T += Tstep){
				Pressure pmin = Boundaries::getSatPres_T(T);
				Pressure pmax = P_MAX;
				Pressure pstep = (pmax - pmin) / double(10);
			
				for(Pressure p = pmin + pstep * 0.5 ; p <= pmax; p +=pstep){
					testUV(p,T);
				}
			}
		}
		
		void testRegion2(){
			// Use a triangular grid of points,
	 		/*  ^
			   p| b23_-`.
				|   - . .
			   sat,'. . .
				|/. . . .
				+--------->
				         T
			*/
			Temperature Tmin,Tmax,Tstep;
			Pressure pmin,pmax,pstep;
			
			Tmin = fromcelsius(0);
			Tmax = T_MAX;
			Tstep = (Tmax - Tmin) / double(20);
			int i = 1;
			for(Temperature T = Tmin + Tstep * 0.5; T <= Tmax ; T += Tstep){

				//cerr << endl << "Solver2Test::testRegion2: T = " << T;
				pmin = P_TRIPLE;
				if(T <= TB_LOW){
					//cerr << endl << "Solver2Test::testRegion2: T <= TB_LOW";
					pmax = Boundaries::getSatPres_T(T);
				}else{
					//cerr << endl << "Solver2Test::testRegion2: T > TB_LOW";
					pmax = Boundaries::getpbound_T(T);
					if(pmax > P_MAX){
						pmax=P_MAX;
					}
				}
				Pressure pstep = (pmax - pmin) / double(i);
			
				for(Pressure p = pmin + pstep * 0.5 ; p <= pmax; p +=pstep){
					//cerr << endl << "Solver2Test::testRegion2: p = " << p;					
					testUV(p,T);
				}
				
				i++;
			}
		}
		
		public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(Solver2Test);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test);


