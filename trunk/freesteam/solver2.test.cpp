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

		void testUV(const SteamCalculator &S,int asserted_region, bool verbose=false){
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

				int region = SS.whichRegion(u,v);
				if(region != asserted_region){
					stringstream s;
					s << "testUV: expected region was " << asserted_region << " but whichRegion(u = " << u << ", v = " << v << ") gives " << region;
					CPPUNIT_FAIL(s.str());
				}

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
					testUV(S,1);
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
					testUV(S,2);
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
			for(Temperature T = Tmin + Tstep * 0.5; T <= Tmax - Tstep * 0.5; T += Tstep){

				xmin = 0.0;
				xmax = 1.0;

				xstep = (xmax - xmin) / double(50 - i / 2);

				for(Num x = xmin + xstep * 0.5; x <= xmax - xstep * 0.5; x +=xstep){
					SteamCalculator S;
					S.setRegion4_Tx(T,x);
					testUV(S,4);
				}

				i++;
			}
		}

		/**
			@todo
				Fix this so to use some kind of rho_pmax(T) rootfinder...
		*/
		void testRegion3(){

			try{
				Temperature Tmin,Tmax, Tstep;
				Density rhomin,rhomax, rhostep;
				SteamCalculator S;

				Tmin = T_REG1_REG3;
				//Tmin = 735.65 * Kelvin;

				Tmax = TB_HIGH;

				Tstep = (Tmax - Tmin) / double(50);

				for(Temperature T=Tmin + Tstep * 0.5; T<=Tmax - Tstep*0.5; T += Tstep){

					S.setB23_T(T);
					rhomin = S.dens() + 0.0001 * kg_m3;

					// where is the maximum density???

					Density rhomax = 380.0 * kg_m3 + ( T - 855.0 * Kelvin) / (T_REG1_REG3 - 855.0 * Kelvin) * (750.0 * kg_m3 -  380.0 * kg_m3);

					//cerr << endl << "Solver2Test::testRegion3: With T = " << T << " (" << tocelsius(T) << "°C), rhomax = " << rhomax;

					rhostep = (rhomax - rhomin) / double(50);
					for(Density rho=rhomin + rhostep * 0.5; rho<=rhomax - rhostep*0.5; rho+=rhostep){

						if(T < T_CRIT){
							// here we need to calculate two different branches, one on either side of the saturated region 4 portion.

							S.setSatSteam_T(T);
							Density rho_g = S.dens();

							S.setSatWater_T(T);
							Density rho_f = S.dens();

							if(rho >= rho_g && rho <= rho_f){
								//cerr << endl << "Solver2Test:testRegion3: skipping with rho = " << rho;
								continue;
							}
						}

						S.setRegion3_rhoT(rho,T);
						//cerr << endl << "Solver2Test::testRegion3: rho = " << rho << ", T = " << T << " (" << tocelsius(T) << ";°C)";

						testUV(S,3);
					}
				}
			}catch(Exception *E){
				stringstream s;
				s << "Solver2Test::testRegion3: " << E->what();
				CPPUNIT_FAIL(s.str());
			}
		}


		public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(Solver2Test);
		CPPUNIT_TEST(testRegion4);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST(testRegion3);

		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test);

