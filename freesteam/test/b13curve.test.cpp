#include "../units.h"
#include "../steamproperty.h"
#include "../b13curve.h"

#include <sstream>

using namespace std;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

/*
	@todo
		Rewrite B13CurveTest to use internally-generated test data
*/
class B13CurveTest: public CppUnit::TestFixture{

	public:
		void tearDown() {
		}

		void setUP(){
		}

	protected:


		/**
			Tests B13 curve solver for

				      T_b13(p)
				      u_b13(s)
				      u_p13(v)
				      h_b13(v)
		*/
		void testB13(
			Temperature T, Pressure p
			,SpecificEnergy u, SpecificEnergy h, SpecificEntropy s
			, SpecificVolume v, SpecHeatCap cp, SpecHeatCap cv
		){

			try{

				B13Curve<Temperature,Pressure> BTp;
				Temperature T_solved = BTp.solve(p);

				if(fabs(T_solved - T)/T > 0.001 * Percent){
					stringstream s;
					s << "Expected T = " << T << ", got T_b13 = " << T_solved << " with p = " << p;
					CPPUNIT_FAIL(s.str());
				}

				B13Curve<SpecificEnergy,SpecificEntropy,SOLVE_IENERGY,SOLVE_ENTROPY> Bus;
				SpecificEnergy u_solved = Bus.solve(s);

				if(fabs(u_solved - u)/u > 0.001 * Percent){
					stringstream s;
					s << "Expected u = " << u << ", got u_b13 = " << u_solved << " with s = " << s;
					CPPUNIT_FAIL(s.str());
				}

				/*
				B13Curve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY> Buv;
				u_solved = Buv.solve(v);

				if(fabs(u_solved - u)/u > 0.001 * Percent){
					stringstream s;
					s << "Expected u = " << u << ", got u_b13 = " << u_solved << " with v = " << v;
					CPPUNIT_FAIL(s.str());
				}

				B13Curve<SpecificEnergy,SpecificVolume,SOLVE_ENTHALPY> Bhv;
				SpecificEnergy h_solved = Bhv.solve(v);

				if(fabs(h_solved - h)/h > 0.001 * Percent){
					stringstream s;
					s << "Expected h = " << h << ", got h_b13 = " << h_solved << " with v = " << v;
					CPPUNIT_FAIL(s.str());
				}
				*/
			}catch(Exception &E){
				stringstream s;
				s << "B13CurveTest::testB13: " << E.what();
				CPPUNIT_FAIL(s.str());
			}

		}

	public:


		void test1(){
			testB13(
				6.23150000e+02 * Kelvin
				, 1.66292000e+07 * Pascal
				, 1.64102963e+06 * J_kg
				, 1.66991763e+06 * J_kg
				, 3.77649282e+03 * J_kgK
				, 1.73718499e-03 * m3_kg
				, 1.00078132e+04 * J_kgK
				, 3.14699407e+03 * J_kgK

			);
		}


		void test2(){testB13(6.23150000e+02 * Kelvin, 1.94082267e+07 * Pascal, 1.61686001e+06 * J_kg, 1.64937040e+06 * J_kg, 3.73592165e+03 * J_kgK, 1.67508266e-03 * m3_kg, 8.32384585e+03 * J_kgK, 3.06089450e+03 * J_kgK);}


		void test3(){testB13(6.23150000e+02 * Kelvin, 2.21872533e+07 * Pascal, 1.59883759e+06 * J_kg, 1.63505485e+06 * J_kg, 3.70557843e+03 * J_kgK, 1.63234533e-03 * m3_kg, 7.49997707e+03 * J_kgK, 3.02585362e+03 * J_kgK);}
		void test4(){testB13(6.23150000e+02 * Kelvin, 2.49662800e+07 * Pascal, 1.58405962e+06 * J_kg, 1.62398453e+06 * J_kg, 3.68061038e+03 * J_kgK, 1.59915345e-03 * m3_kg, 6.98516921e+03 * J_kgK, 3.00102989e+03 * J_kgK);}
		void test5(){testB13(6.23150000e+02 * Kelvin, 2.77453067e+07 * Pascal, 1.57138134e+06 * J_kg, 1.61499482e+06 * J_kg, 3.65911500e+03 * J_kgK, 1.57192291e-03 * m3_kg, 6.62143159e+03 * J_kgK, 2.97636516e+03 * J_kgK);}
		void test6(){testB13(6.23150000e+02 * Kelvin, 3.05243333e+07 * Pascal, 1.56019442e+06 * J_kg, 1.60747037e+06 * J_kg, 3.64008277e+03 * J_kgK, 1.54879543e-03 * m3_kg, 6.34673837e+03 * J_kgK, 2.95658418e+03 * J_kgK);}
		void test7(){testB13(6.23150000e+02 * Kelvin, 3.33033600e+07 * Pascal, 1.55012851e+06 * J_kg, 1.60103705e+06 * J_kg, 3.62289773e+03 * J_kgK, 1.52863045e-03 * m3_kg, 6.12877845e+03 * J_kgK, 2.94383560e+03 * J_kgK);}
		void test8(){testB13(6.23150000e+02 * Kelvin, 3.60823867e+07 * Pascal, 1.54094676e+06 * J_kg, 1.59545676e+06 * J_kg, 3.60716632e+03 * J_kgK, 1.51070934e-03 * m3_kg, 5.94929363e+03 * J_kgK, 2.93604501e+03 * J_kgK);}
		void test9(){testB13(6.23150000e+02 * Kelvin, 3.88614133e+07 * Pascal, 1.53248896e+06 * J_kg, 1.59057006e+06 * J_kg, 3.59262375e+03 * J_kgK, 1.49456995e-03 * m3_kg, 5.79785095e+03 * J_kgK, 2.93030491e+03 * J_kgK);}
		void test10(){testB13(6.23150000e+02 * Kelvin, 4.16404400e+07 * Pascal, 1.52463846e+06 * J_kg, 1.58626196e+06 * J_kg, 3.57907829e+03 * J_kgK, 1.47989533e-03 * m3_kg, 5.66811862e+03 * J_kgK, 2.92492835e+03 * J_kgK);}
		void test11(){testB13(6.23150000e+02 * Kelvin, 4.44194667e+07 * Pascal, 1.51730485e+06 * J_kg, 1.58244374e+06 * J_kg, 3.56638160e+03 * J_kgK, 1.46644904e-03 * m3_kg, 5.55576352e+03 * J_kgK, 2.91954456e+03 * J_kgK);}
		void test12(){testB13(6.23150000e+02 * Kelvin, 4.71984933e+07 * Pascal, 1.51041574e+06 * J_kg, 1.57904441e+06 * J_kg, 3.55441470e+03 * J_kgK, 1.45404366e-03 * m3_kg, 5.45748413e+03 * J_kgK, 2.91442827e+03 * J_kgK);}
		void test13(){testB13(6.23150000e+02 * Kelvin, 4.99775200e+07 * Pascal, 1.50391255e+06 * J_kg, 1.57600647e+06 * J_kg, 3.54308105e+03 * J_kgK, 1.44252710e-03 * m3_kg, 5.37064906e+03 * J_kgK, 2.90991250e+03 * J_kgK);}
		void test14(){testB13(6.23150000e+02 * Kelvin, 5.27565467e+07 * Pascal, 1.49774781e+06 * J_kg, 1.57328337e+06 * J_kg, 3.53230222e+03 * J_kgK, 1.43177588e-03 * m3_kg, 5.29316517e+03 * J_kgK, 2.90613161e+03 * J_kgK);}
		void test15(){testB13(6.23150000e+02 * Kelvin, 5.55355733e+07 * Pascal, 1.49188291e+06 * J_kg, 1.57083729e+06 * J_kg, 3.52201439e+03 * J_kgK, 1.42169021e-03 * m3_kg, 5.22339548e+03 * J_kgK, 2.90301761e+03 * J_kgK);}
		void test16(){testB13(6.23150000e+02 * Kelvin, 5.83146000e+07 * Pascal, 1.48628608e+06 * J_kg, 1.56863734e+06 * J_kg, 3.51216518e+03 * J_kgK, 1.41218947e-03 * m3_kg, 5.16007465e+03 * J_kgK, 2.90039852e+03 * J_kgK);}
		void test17(){testB13(6.23150000e+02 * Kelvin, 6.10936267e+07 * Pascal, 1.48093089e+06 * J_kg, 1.56665795e+06 * J_kg, 3.50271110e+03 * J_kgK, 1.40320797e-03 * m3_kg, 5.10222330e+03 * J_kgK, 2.89809290e+03 * J_kgK);}
		void test18(){testB13(6.23150000e+02 * Kelvin, 6.38726533e+07 * Pascal, 1.47579497e+06 * J_kg, 1.56487761e+06 * J_kg, 3.49361544e+03 * J_kgK, 1.39469140e-03 * m3_kg, 5.04907295e+03 * J_kgK, 2.89596161e+03 * J_kgK);}
		void test19(){testB13(6.23150000e+02 * Kelvin, 6.66516800e+07 * Pascal, 1.47085913e+06 * J_kg, 1.56327796e+06 * J_kg, 3.48484678e+03 * J_kgK, 1.38659412e-03 * m3_kg, 5.00000800e+03 * J_kgK, 2.89392026e+03 * J_kgK);}
		void test20(){testB13(6.23150000e+02 * Kelvin, 6.94307067e+07 * Pascal, 1.46610673e+06 * J_kg, 1.56184315e+06 * J_kg, 3.47637790e+03 * J_kgK, 1.37887714e-03 * m3_kg, 4.95452453e+03 * J_kgK, 2.89193016e+03 * J_kgK);}
		void test21(){testB13(6.23150000e+02 * Kelvin, 7.22097333e+07 * Pascal, 1.46152322e+06 * J_kg, 1.56055935e+06 * J_kg, 3.46818499e+03 * J_kgK, 1.37150674e-03 * m3_kg, 4.91220241e+03 * J_kgK, 2.88998301e+03 * J_kgK);}
		void test22(){testB13(6.23150000e+02 * Kelvin, 7.49887600e+07 * Pascal, 1.45709576e+06 * J_kg, 1.55941444e+06 * J_kg, 3.46024709e+03 * J_kgK, 1.36445342e-03 * m3_kg, 4.87268666e+03 * J_kgK, 2.88808816e+03 * J_kgK);}
		void test23(){testB13(6.23150000e+02 * Kelvin, 7.77677867e+07 * Pascal, 1.45281302e+06 * J_kg, 1.55839766e+06 * J_kg, 3.45254563e+03 * J_kgK, 1.35769123e-03 * m3_kg, 4.83567456e+03 * J_kgK, 2.88626478e+03 * J_kgK);}
		void test24(){testB13(6.23150000e+02 * Kelvin, 8.05468133e+07 * Pascal, 1.44866490e+06 * J_kg, 1.55749952e+06 * J_kg, 3.44506410e+03 * J_kgK, 1.35119717e-03 * m3_kg, 4.80090629e+03 * J_kgK, 2.88453859e+03 * J_kgK);}
		void test25(){testB13(6.23150000e+02 * Kelvin, 8.33258400e+07 * Pascal, 1.44464239e+06 * J_kg, 1.55671154e+06 * J_kg, 3.43778774e+03 * J_kgK, 1.34495075e-03 * m3_kg, 4.76815782e+03 * J_kgK, 2.88294119e+03 * J_kgK);}
		void test26(){testB13(6.23150000e+02 * Kelvin, 8.61048667e+07 * Pascal, 1.44073744e+06 * J_kg, 1.55602614e+06 * J_kg, 3.43070335e+03 * J_kgK, 1.33893363e-03 * m3_kg, 4.73723524e+03 * J_kgK, 2.88151051e+03 * J_kgK);}
		void test27(){testB13(6.23150000e+02 * Kelvin, 8.88838933e+07 * Pascal, 1.43694282e+06 * J_kg, 1.55543654e+06 * J_kg, 3.42379903e+03 * J_kgK, 1.33312931e-03 * m3_kg, 4.70797012e+03 * J_kgK, 2.88029124e+03 * J_kgK);}
		void test28(){testB13(6.23150000e+02 * Kelvin, 9.16629200e+07 * Pascal, 1.43325200e+06 * J_kg, 1.55493662e+06 * J_kg, 3.41706408e+03 * J_kgK, 1.32752281e-03 * m3_kg, 4.68021573e+03 * J_kgK, 2.87933441e+03 * J_kgK);}
		void test29(){testB13(6.23150000e+02 * Kelvin, 9.44419467e+07 * Pascal, 1.42965913e+06 * J_kg, 1.55452088e+06 * J_kg, 3.41048880e+03 * J_kgK, 1.32210056e-03 * m3_kg, 4.65384384e+03 * J_kgK, 2.87869609e+03 * J_kgK);}
		void test30(){testB13(6.23150000e+02 * Kelvin, 9.72209733e+07 * Pascal, 1.42615889e+06 * J_kg, 1.55418434e+06 * J_kg, 3.40406442e+03 * J_kgK, 1.31685010e-03 * m3_kg, 4.62874216e+03 * J_kgK, 2.87843487e+03 * J_kgK);}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:
		CPPUNIT_TEST_SUITE(B13CurveTest);

		CPPUNIT_TEST(test1);
		CPPUNIT_TEST(test2);
		CPPUNIT_TEST(test3);
		CPPUNIT_TEST(test4);
		CPPUNIT_TEST(test5);
		CPPUNIT_TEST(test6);
		CPPUNIT_TEST(test7);
		CPPUNIT_TEST(test8);
		CPPUNIT_TEST(test9);
		CPPUNIT_TEST(test10);
		CPPUNIT_TEST(test11);
		CPPUNIT_TEST(test12);
		CPPUNIT_TEST(test13);
		CPPUNIT_TEST(test14);
		CPPUNIT_TEST(test15);
		CPPUNIT_TEST(test16);
		CPPUNIT_TEST(test17);
		CPPUNIT_TEST(test18);
		CPPUNIT_TEST(test19);
		CPPUNIT_TEST(test20);
		CPPUNIT_TEST(test21);
		CPPUNIT_TEST(test22);
		CPPUNIT_TEST(test23);
		CPPUNIT_TEST(test24);
		CPPUNIT_TEST(test25);
		CPPUNIT_TEST(test26);
		CPPUNIT_TEST(test27);
		CPPUNIT_TEST(test28);
		CPPUNIT_TEST(test29);
		CPPUNIT_TEST(test30);

		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(B13CurveTest);
