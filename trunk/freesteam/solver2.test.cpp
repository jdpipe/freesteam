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

		void testUV(const SteamCalculator &S,bool verbose=false){
			try{
				Pressure p = S.pres();
				Temperature T = S.temp();
				Density rho = S.dens();

				SpecificEnergy u = S.specienergy();
				SpecificVolume v = S.specvol();

				if(verbose){
					cerr << endl << endl << "Solver2Test:TestUV: Testing with p = " << p << ", T = " << T;
					cerr << endl << "Solver2Test:TestUV: Expecting u = " << u << ", v = " << v;
				}

				Solver2<SpecificEnergy,SpecificVolume> SS;
				SteamCalculator S2 = SS.solve( u, v);

				SteamPropertyTest<Pressure>::test( S2, p, 0.01 * bar);
				SteamPropertyTest<Temperature>::test( S2, T, 0.01 * Kelvin);
				SteamPropertyTest<Density>::test( S2, rho, rho * 0.001 * Percent);

			}catch(Exception *E){
				stringstream s;
				s << "Solver2Test::testUV: " << E->what();
				CPPUNIT_FAIL(s.str());
			}
		}

		void testRegion1(){
			Temperature Tmin = fromcelsius(0.0);
			Temperature Tmax = T_REG1_REG3;
			Temperature Tstep = (Tmax - Tmin) / double(40);
			for(Temperature T = Tmin + Tstep * 0.5; T <= Tmax ; T += Tstep){
				Pressure pmin = Boundaries::getSatPres_T(T);
				Pressure pmax = P_MAX;
				Pressure pstep = (pmax - pmin) / double(40);

				for(Pressure p = pmin + pstep * 0.5 ; p <= pmax; p +=pstep){
					SteamCalculator S;
					S.setRegion1_pT(p,T);
					testUV(S);
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
			Tstep = (Tmax - Tmin) / double(40);
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
					SteamCalculator S;
					S.setRegion2_pT(p,T);
					testUV(S);
				}

				i++;
			}
		}

		void testRegion4(){

			Temperature Tmin,Tmax,Tstep;
			Num xmin,xmax,xstep;

			Tmin = T_TRIPLE;
			Tmax = T_REG1_REG3;
			Tstep = (Tmax - Tmin) / double(50);
			int i = 1;
			for(Temperature T = Tmin; T <= Tmax ; T += Tstep){

				xmin = 0.0;
				xmax = 1.0;

				xstep = (xmax - xmin) / double(50);

				for(Num x = xmin; x <= xmax; x +=xstep){
					SteamCalculator S;
					S.setRegion4_Tx(T,x);
					testUV(S);
				}

				i++;
			}
		}

		public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(Solver2Test);
		CPPUNIT_TEST(testRegion4);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test);


