#include "../solver2.h"
#include "steampropertytest.h"
#include "test.h"

#include <sstream>
#include <string>
using namespace std;

template <class FirstProp, class SecondProp, int FirstPropAlt=0, int SecondPropAlt=0>
class Solver2Test : public CppUnit::TestFixture
{

	protected:

		SteamCalculator S;

		void testWith(const SteamCalculator &S,int asserted_region, bool verbose=false){
			Pressure p = S.pres();
			Temperature T = S.temp();
			Density rho = S.dens();

			try{

				FirstProp f = SteamProperty<FirstProp,FirstPropAlt>::get(S);
				SecondProp s = SteamProperty<SecondProp,SecondPropAlt>::get(S);

				if(verbose){
					cerr << endl << endl << "Solver2Test<" << SteamProperty<FirstProp,FirstPropAlt>::name() << "," << SteamProperty<SecondProp,SecondPropAlt>::name() << ">:testWith: Testing with p = " << p << ", T = " << T << ", expecting " << SteamProperty<FirstProp,FirstPropAlt>::name() << " = " << SteamProperty<FirstProp,FirstPropAlt>::get(S) << ", " << SteamProperty<SecondProp,SecondPropAlt>::name() << " = " << SteamProperty<SecondProp,SecondPropAlt>::get(S);
				}

				Solver2<FirstProp,SecondProp,FirstPropAlt,SecondPropAlt> SS;

				int region = SS.whichRegion(f,s);
				if(region != asserted_region){
					stringstream ss;
					ss << "Expected region was " << asserted_region << " but whichRegion(" << f << ", " << s << ") gave " << region;
					throw std::runtime_error(ss.str());
				}

				SteamCalculator S2 = SS.solve(f, s);

				SteamPropertyTest<Pressure>::test( S2, p, 0.01 * bar);
				SteamPropertyTest<Temperature>::test( S2, T, 0.01 * Kelvin);
				SteamPropertyTest<Density>::test( S2, rho, rho * 0.001 * Percent);

			}catch(std::exception &E){
				stringstream ss;
				ss << "Solver2Test<" <<  SteamProperty<FirstProp,FirstPropAlt>::name() << "," << SteamProperty<SecondProp,SecondPropAlt>::name() << ">:testWith(p = " << p << ", T = " << T << ", rho = " << rho << "): " << E.what();
				CPPUNIT_FAIL(ss.str());
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
					testWith(S,1);
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
					testWith(S,2);
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
					testWith(S,4);
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

						testWith(S,3);
					}
				}
			}catch(std::exception &E){
				stringstream s;
				s << "Solver2Test::testRegion3: " << E.what();
				CPPUNIT_FAIL(s.str());
			}
		}


	public:

		CPPUNIT_TEST_SUITE(Solver2Test);
		CPPUNIT_TEST(testRegion4);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST(testRegion3);

		CPPUNIT_TEST_SUITE_END();

};

class Solver2Test_UV : public Solver2Test<SpecificEnergy,SpecificVolume>
{
	public:

		CPPUNIT_TEST_SUITE(Solver2Test_UV);
		CPPUNIT_TEST(testRegion4);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST(testRegion3);

		CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test_UV);

class Solver2Test_PH: public Solver2Test<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY>
{
	public:

		CPPUNIT_TEST_SUITE(Solver2Test_PH);
		CPPUNIT_TEST(testRegion4);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST(testRegion3);

		CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test_PH);

class Solver2Test_PT: public Solver2Test<Pressure,Temperature>
{
	public:

		CPPUNIT_TEST_SUITE(Solver2Test_PT);
		// CPPUNIT_TEST(testRegion4); // invalid, underspecified!
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST(testRegion3); // this should be OK but it's getting stuck on the saturation line

		CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test_PT);

class Solver2Test_PU: public Solver2Test<Pressure,SpecificEnergy,0,SOLVE_IENERGY>
{
	public:

		CPPUNIT_TEST_SUITE(Solver2Test_PU);
		CPPUNIT_TEST(testRegion4);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST(testRegion3);

		CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test_PU);

class Solver2Test_PS: public Solver2Test<Pressure,SpecificEntropy,0,SOLVE_ENTROPY>
{
	public:

		CPPUNIT_TEST_SUITE(Solver2Test_PS);
		CPPUNIT_TEST(testRegion4);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST(testRegion3);

		CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test_PS);

class Solver2Test_TS: public Solver2Test<Temperature,SpecificEntropy,0,SOLVE_ENTROPY>
{
	public:

		CPPUNIT_TEST_SUITE(Solver2Test_TS);
		CPPUNIT_TEST(testRegion4);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST(testRegion3);

		CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test_TS);

class Solver2Test_TH: public Solver2Test<Temperature,SpecificEntropy,0,SOLVE_ENTROPY>
{
	public:

		CPPUNIT_TEST_SUITE(Solver2Test_TH);
		CPPUNIT_TEST(testRegion4);
		CPPUNIT_TEST(testRegion1);
		CPPUNIT_TEST(testRegion2);
		CPPUNIT_TEST(testRegion3);

		CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Solver2Test_TH);
