#include "../boundaries.h"
#include <stdexcept>

#include <sstream>

using namespace std;

#include "test.h"

class RhoSatTest: public CppUnit::TestFixture{

	protected:


		void TestLiqPoint(const Temperature &T, const Density &rho_f){

			try{

				Density rho_f_calc = Boundaries::getSatDensWater_T(T);

				if(fabs(rho_f_calc - rho_f) > 0.001 * kg_m3){
					stringstream s;
					s << "Expected rho_f = " << rho_f << " but Boundaries::getRhoSatWater_T(T = " << T << ") gave " << rho_f_calc;
					s << " (error is " << fabs(rho_f_calc - rho_f)/rho_f / Percent << "%)";
					CPPUNIT_FAIL(s.str());
				}
			}catch(std::exception &e){
				stringstream s;
				s << "RhoSatTest::TestLiqPoint: " << e.what();
				CPPUNIT_FAIL(s.str());
			}
		}

		void TestGasPoint(const Temperature &T, const Density &rho_g){
			try{


				Density rho_g_calc = Boundaries::getSatDensSteam_T(T);

				if(fabs(rho_g_calc - rho_g) > 6e-5 * Percent * rho_g){
					stringstream s;
					s << "Expected rho_g = " << rho_g << " but Boundaries::getRhoSatSteam_T(T = " << T << ") gave " << rho_g_calc;
					s << " (error is " << fabs(rho_g_calc - rho_g)/rho_g / Percent << "%)";
					CPPUNIT_FAIL(s.str());
				}

			}catch(std::exception &e){
				stringstream s;
				s << "RhoSatTest::TestPoint: " << e.what();
				CPPUNIT_FAIL(s.str());
			}
		}

		void Test1(){ TestLiqPoint(273.16 * Kelvin,	999.789 * kg_m3); }
		void Test2(){ TestGasPoint(273.16 * Kelvin,	0.00485426 * kg_m3); }
		void Test3(){ TestLiqPoint(373.1243 * Kelvin,	958.365 * kg_m3); }
		void Test4(){ TestGasPoint(373.1243 * Kelvin,	0.597586 * kg_m3); }
		void Test5(){ TestLiqPoint(647.096 * Kelvin,	322.0 * kg_m3);	}
		void Test6(){ TestGasPoint(647.096 * Kelvin,	322.0 * kg_m3); }

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(RhoSatTest);

		CPPUNIT_TEST(Test1);
		CPPUNIT_TEST(Test2);
		CPPUNIT_TEST(Test3);
		CPPUNIT_TEST(Test4);
		CPPUNIT_TEST(Test5);
		CPPUNIT_TEST(Test6);

		CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RhoSatTest);
