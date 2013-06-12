#include "../satcurve.h"

#include <sstream>

using namespace std;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class SatCurveTest: public CppUnit::TestFixture{

	public:
		void tearDown() {
		}

		void setUP(){
		}

	protected:

		static const int nsteps=200;

		/*
		void TestTU(Temperature T, SpecificEnergy u,int flags=SAT_WATER){
			try{
				// Solver will give use u_sat(v)
				SatCurve<Temperature, SpecificEnergy,0,SOLVE_IENERGY> C;
				Temperature T_solved = C.solve(u,flags);

				if(fabs(T_solved - T) > T * 0.8 * Percent){
					stringstream s;
					s << "Expected T = " << T << " but solution T_sat(u = " << u << ") gave " << T_solved;
					s << " (error is " << fabs(T_solved - T)/T / Percent << "%)";
					CPPUNIT_FAIL(s.str());
				}

			}catch(std::exception &e){
				stringstream s;
				s << "SatCurveTest::TestTU: " << e.what();
				CPPUNIT_FAIL(s.str());
			}
		}
		*/

		void TestUT(SpecificEnergy u,Temperature T, int flags=SAT_WATER){
			try{
				// Solver will give use u_sat(v)
				SatCurve<SpecificEnergy,Temperature,SOLVE_IENERGY,0> C;
				SpecificEnergy u_solved = C.solve(T,flags);

				if(fabs(u_solved - u) > fabs(u * 0.8 * Percent)){
					stringstream s;
					s << "Expected u = " << u << " but solution u_sat(T = " << T << ") gave " << u_solved;
					s << " (error is " << fabs(u_solved - u)/u / Percent << "%)";
					CPPUNIT_FAIL(s.str());
				}

			}catch(std::exception &e){
				stringstream s;
				s << "SatCurveTest::TestUT: " << e.what();
				CPPUNIT_FAIL(s.str());
			}
		}


		void TestVU(SpecificVolume v, SpecificEnergy u,int flags=SAT_WATER){
			try{
				// Solver will give use u_sat(v)
				SatCurve<SpecificEnergy,SpecificVolume> C;
				SpecificEnergy u_solved = C.solve(v,flags);

				if(fabs(u_solved - u) > fabs(u * 0.8 * Percent)){
					stringstream s;
					s << "Expected u = " << u << " but solution u_sat(v = " << v << ") gave " << u_solved;
					s << " (error is " << fabs(u_solved - u)/u / Percent << "%)";
					CPPUNIT_FAIL(s.str());
				}

			}catch(std::exception &e){
				stringstream s;
				s << "SatCurveTest::TestVU: " << e.what();
				CPPUNIT_FAIL(s.str());
			}
		}

		void TestWater(){
			try{

				Temperature Tstep = (T_CRIT - T_TRIPLE ) / double(nsteps);
				for(Temperature T = T_TRIPLE; T < T_CRIT; T+= Tstep){
					SteamCalculator S;
					S.setSatWater_T(T);
					SpecificEnergy u = S.specienergy();
					SpecificVolume v = S.specvol();

					//cerr << endl << "SatCurveTest::TestWater: T = " << T << ": u = " << u << ", v = " << v;

					if(T < T_MIN_VOL){
						TestVU(v,u,SAT_WATER | SAT_LOW_TEMP);
					}else{
						TestVU(v,u,SAT_WATER);
					}
				}
			}catch(std::exception &E){
				stringstream s1;
				s1 << "SatCurveTest::TestWater: " << E.what();
				CPPUNIT_FAIL(s1.str());
			}
		}

		void TestSteam(){
			try{

				Temperature Tstep = (T_CRIT - T_TRIPLE ) / double(nsteps);
				for(Temperature T = T_TRIPLE; T < T_CRIT; T+= Tstep){
					SteamCalculator S;
					S.setSatSteam_T(T);
					SpecificEnergy u = S.specienergy();
					SpecificVolume v = S.specvol();

					//cerr << endl << "SatCurveTest::TestSteam: T = " << T << ": u = " << u << ", v = " << v;

					TestVU(v,u,SAT_STEAM);
				}
			}catch(std::exception &E){
				stringstream s1;
				s1 << "SatCurveTest::TestSteam: " << E.what();
				CPPUNIT_FAIL(s1.str());
			}
		}

		/*
		void Test11(){ TestVU( 0.001053 * m3_kg   ,  467.0 * kJ_kg     , SAT_WATER   ); }
		void Test12(){ TestVU( 0.001061 * m3_kg   ,  504.5 * kJ_kg     , SAT_WATER   ); }
		void Test13(){ TestVU( 0.001074 * m3_kg   ,  561.1 * kJ_kg     , SAT_WATER   ); }
		void Test14(){ TestVU( 0.001084 * m3_kg   ,  604.2 * kJ_kg     , SAT_WATER   ); }
		void Test15(){ TestVU( 1.159 * m3_kg      , 2506.1 * kJ_kg     , SAT_STEAM   ); }
		void Test16(){ TestVU( 0.885 * m3_kg      , 2529.2 * kJ_kg     , SAT_STEAM   ); }
		void Test17(){ TestVU( 0.606 * m3_kg      , 2543.0 * kJ_kg     , SAT_STEAM   ); }
		void Test18(){ TestVU( 0.462 * m3_kg      , 2552.7 * kJ_kg     , SAT_STEAM   ); }
		void Test40(){ TestVU( 0.001127231 * m3_kg , 761.3878384 * kJ_kg, SAT_WATER    ); }
		*/
		void Test21(){ TestUT( 467.0 * kJ_kg      , fromcelsius(111.4) , SAT_WATER   ); }
		void Test22(){ TestUT( 504.5 * kJ_kg      , fromcelsius(120.6) , SAT_WATER   ); }
		void Test23(){ TestUT( 561.1 * kJ_kg      , fromcelsius(133.5) , SAT_WATER   ); }
		void Test24(){ TestUT( 604.2 * kJ_kg      , fromcelsius(143.6) , SAT_WATER   ); }
		void Test25(){ TestUT(2506.1 * kJ_kg      , fromcelsius(111.4) , SAT_STEAM   ); }
		void Test26(){ TestUT(2529.2 * kJ_kg      , fromcelsius(120.6) , SAT_STEAM   ); }
		void Test27(){ TestUT(2543.0 * kJ_kg      , fromcelsius(133.5) , SAT_STEAM   ); }
		void Test28(){ TestUT(2552.7 * kJ_kg      , fromcelsius(143.6) , SAT_STEAM   ); }
		/*
		void Test31(){ TestTU( fromcelsius(111.4) ,  467.0 * kJ_kg     , SAT_WATER   ); }
		void Test32(){ TestTU( fromcelsius(120.6) ,  504.5 * kJ_kg     , SAT_WATER   ); }
		void Test33(){ TestTU( fromcelsius(133.5) ,  561.1 * kJ_kg     , SAT_WATER   ); }
		void Test34(){ TestTU( fromcelsius(143.6) ,  604.2 * kJ_kg     , SAT_WATER   ); }
		void Test35(){ TestTU( fromcelsius(111.4) , 2506.1 * kJ_kg     , SAT_STEAM   ); }
		void Test36(){ TestTU( fromcelsius(120.6) , 2529.2 * kJ_kg     , SAT_STEAM   ); }
		void Test37(){ TestTU( fromcelsius(133.5) , 2543.0 * kJ_kg     , SAT_STEAM   ); }
		void Test38(){ TestTU( fromcelsius(143.6) , 2552.7 * kJ_kg     , SAT_STEAM   ); }
		*/


	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(SatCurveTest);

		//CPPUNIT_TEST(Test11);
		//CPPUNIT_TEST(Test12);
		//CPPUNIT_TEST(Test13);
		//CPPUNIT_TEST(Test14);
		//CPPUNIT_TEST(Test15);
		//CPPUNIT_TEST(Test16);
		//CPPUNIT_TEST(Test17);
		//CPPUNIT_TEST(Test18);
		//CPPUNIT_TEST(Test40);

		CPPUNIT_TEST(Test21);
		CPPUNIT_TEST(Test22);
		CPPUNIT_TEST(Test23);
		CPPUNIT_TEST(Test24);
		CPPUNIT_TEST(Test25);
		CPPUNIT_TEST(Test26);
		CPPUNIT_TEST(Test27);
		CPPUNIT_TEST(Test28);

		CPPUNIT_TEST(TestWater);
		//CPPUNIT_TEST(TestSteam);

		CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SatCurveTest);