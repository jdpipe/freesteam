#include "units.h"
#include "steamproperty.h"
#include "b23curve.h"

#include <sstream>

using namespace std;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class B23CurveTest: public CppUnit::TestFixture{

	public:
		void tearDown() {
		}
		
		void setUP(){
		}
		
	protected:
		

		/**
			Tests B23 curve solver for
			
				      T_b23(h)
				
				and   u_b23(v)
		*/				
		void testB23Curve(
			Temperature T, Pressure p
			,SpecificEnergy u, SpecificEnergy h, SpecificEntropy s
			, SpecificVolume v, SpecHeatCap cp, SpecHeatCap cv
		){
			
			try{
				
				// test u_bound(v)
				
				B23Curve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> Buv;
				SpecificEnergy u_solved = Buv.solve(v);
	
				if(fabs(u_solved - u)/u > 0.001 * Percent){
					stringstream s;
					s << "Expected u = " << u << ", got u_b23 = " << u_solved << " with v = " << v;
					CPPUNIT_FAIL(s.str());
				}


				/*
					This doesn't work:
					
				// test v_bound(u)
				
				B23Curve<SpecificVolume,SpecificEnergy,0,SOLVE_IENERGY> Bvu;
				SpecificVolume v_solved = Bvu.solve(u);
	
				if(fabs(v_solved - v)/v > 0.001 * Percent){
					stringstream s;
					s << "Expected v = " << v << ", got v_b23 = " << v_solved << " with u = " << u;
					CPPUNIT_FAIL(s.str());
				}
				
				*/

				// test h_bound(T)
				
				B23Curve<SpecificEnergy,Temperature,SOLVE_ENTHALPY,0> BhT;
				SpecificEnergy h_solved = BhT.solve(T);
	
				if(fabs(h_solved - h)/h > 0.001 * Percent){
					stringstream s;
					s << "Expected h = " << h << ", got h_b23 = " << h_solved << " with T = " << T;
					CPPUNIT_FAIL(s.str());
				}
			
			}catch(Exception *E){
				stringstream s;
				s << "Exception caught: " << E->what();
				CPPUNIT_FAIL(s.str());
			}
	
		}	
	
	public:
	
		void test1(){
			//cerr << " (SKIPPED - problems at critical point)";
			CPPUNIT_FAIL("Test data for point 1 is wrong");
			
			//testB23Curve(6.47096000e+02 * Kelvin, 1.00000000e+00 * Pascal, 2.92790124e+06 * Joule / kilogram,3.22655286e+06 * Joule / kilogram,1.37791807e+04 * Joule / kilogram / Kelvin,2.98651624e+05 * m3_kg, 2.04640381e+03 * Joule / kilogram / Kelvin,1.58487777e+03 * Joule / kilogram / Kelvin);
		}
		void test2(){testB23Curve(6.54297800e+02 * Kelvin, 2.07313989e+07 * Pascal, 2.46666878e+06 * Joule / kilogram,2.62485882e+06 * Joule / kilogram,5.25304451e+03 * Joule / kilogram / Kelvin,7.63045658e-03 * m3_kg, 1.15766246e+04 * Joule / kilogram / Kelvin,3.22411825e+03 * Joule / kilogram / Kelvin);}
		void test3(){testB23Curve(6.61499600e+02 * Kelvin, 2.19845291e+07 * Pascal, 2.46760711e+06 * Joule / kilogram,2.62636552e+06 * Joule / kilogram,5.24119802e+03 * Joule / kilogram / Kelvin,7.22136928e-03 * m3_kg, 1.13995383e+04 * Joule / kilogram / Kelvin,3.18135743e+03 * Joule / kilogram / Kelvin);}
		void test4(){testB23Curve(6.68701400e+02 * Kelvin, 2.33433928e+07 * Pascal, 2.46661497e+06 * Joule / kilogram,2.62556885e+06 * Joule / kilogram,5.22567598e+03 * Joule / kilogram / Kelvin,6.80937344e-03 * m3_kg, 1.12982264e+04 * Joule / kilogram / Kelvin,3.14443793e+03 * Joule / kilogram / Kelvin);}
		void test5(){testB23Curve(6.75903200e+02 * Kelvin, 2.48079902e+07 * Pascal, 2.46422069e+06 * Joule / kilogram,2.62315690e+06 * Joule / kilogram,5.20770251e+03 * Joule / kilogram / Kelvin,6.40665372e-03 * m3_kg, 1.12277180e+04 * Joule / kilogram / Kelvin,3.11030172e+03 * Joule / kilogram / Kelvin);}
		void test6(){testB23Curve(6.83105000e+02 * Kelvin, 2.63783210e+07 * Pascal, 2.46089899e+06 * Joule / kilogram,2.61974906e+06 * Joule / kilogram,5.18833599e+03 * Joule / kilogram / Kelvin,6.02199289e-03 * m3_kg, 1.11548183e+04 * Joule / kilogram / Kelvin,3.07719958e+03 * Joule / kilogram / Kelvin);}
		void test7(){testB23Curve(6.90306800e+02 * Kelvin, 2.80543855e+07 * Pascal, 2.45708708e+06 * Joule / kilogram,2.61591404e+06 * Joule / kilogram,5.16850334e+03 * Joule / kilogram / Kelvin,5.66139500e-03 * m3_kg, 1.10544995e+04 * Joule / kilogram / Kelvin,3.04421872e+03 * Joule / kilogram / Kelvin);}
		void test8(){testB23Curve(6.97508600e+02 * Kelvin, 2.98361835e+07 * Pascal, 2.45318458e+06 * Joule / kilogram,2.61216817e+06 * Joule / kilogram,5.14900509e+03 * Joule / kilogram / Kelvin,5.32854995e-03 * m3_kg, 1.09091804e+04 * Joule / kilogram / Kelvin,3.01100602e+03 * Joule / kilogram / Kelvin);}
		void test9(){testB23Curve(7.04710400e+02 * Kelvin, 3.17237150e+07 * Pascal, 2.44954438e+06 * Joule / kilogram,2.60896307e+06 * Joule / kilogram,5.13050579e+03 * Joule / kilogram / Kelvin,5.02522144e-03 * m3_kg, 1.07091219e+04 * Joule / kilogram / Kelvin,2.97757599e+03 * Joule / kilogram / Kelvin);}
		void test10(){testB23Curve(7.11912200e+02 * Kelvin, 3.37169801e+07 * Pascal, 2.44646023e+06 * Joule / kilogram,2.60667002e+06 * Joule / kilogram,5.11352075e+03 * Joule / kilogram / Kelvin,4.75160553e-03 * m3_kg, 1.04528068e+04 * Joule / kilogram / Kelvin,2.94416966e+03 * Joule / kilogram / Kelvin);}
		void test11(){testB23Curve(7.19114000e+02 * Kelvin, 3.58159788e+07 * Pascal, 2.44415626e+06 * Joule / kilogram,2.60556752e+06 * Joule / kilogram,5.09840820e+03 * Joule / kilogram / Kelvin,4.50668316e-03 * m3_kg, 1.01465193e+04 * Joule / kilogram / Kelvin,2.91115934e+03 * Joule / kilogram / Kelvin);}
		void test12(){testB23Curve(7.26315800e+02 * Kelvin, 3.80207110e+07 * Pascal, 2.44278246e+06 * Joule / kilogram,2.60583693e+06 * Joule / kilogram,5.08537287e+03 * Joule / kilogram / Kelvin,4.28856970e-03 * m3_kg, 9.80271996e+03 * Joule / kilogram / Kelvin,2.87899161e+03 * Joule / kilogram / Kelvin);}
		void test13(){testB23Curve(7.33517600e+02 * Kelvin, 4.03311768e+07 * Pascal, 2.44241812e+06 * Joule / kilogram,2.60756814e+06 * Joule / kilogram,5.07448297e+03 * Joule / kilogram / Kelvin,4.09484771e-03 * m3_kg, 9.43729020e+03 * Joule / kilogram / Kelvin,2.84813922e+03 * Joule / kilogram / Kelvin);}
		void test14(){testB23Curve(7.40719400e+02 * Kelvin, 4.27473762e+07 * Pascal, 2.44308231e+06 * Joule / kilogram,2.61077428e+06 * Joule / kilogram,5.06569786e+03 * Joule / kilogram / Kelvin,3.92285990e-03 * m3_kg, 9.06621631e+03 * Joule / kilogram / Kelvin,2.81901834e+03 * Joule / kilogram / Kelvin);}
		void test15(){testB23Curve(7.47921200e+02 * Kelvin, 4.52693091e+07 * Pascal, 2.44474878e+06 * Joule / kilogram,2.61541141e+06 * Joule / kilogram,5.05890065e+03 * Joule / kilogram / Kelvin,3.76994117e-03 * m3_kg, 8.70260602e+03 * Joule / kilogram / Kelvin,2.79184740e+03 * Joule / kilogram / Kelvin);}
		void test16(){testB23Curve(7.55123000e+02 * Kelvin, 4.78969756e+07 * Pascal, 2.44736090e+06 * Joule / kilogram,2.62139832e+06 * Joule / kilogram,5.05392866e+03 * Joule / kilogram / Kelvin,3.63357839e-03 * m3_kg, 8.35490815e+03 * Joule / kilogram / Kelvin,2.76648410e+03 * Joule / kilogram / Kelvin);}
		void test17(){testB23Curve(7.62324800e+02 * Kelvin, 5.06303756e+07 * Pascal, 2.45084404e+06 * Joule / kilogram,2.62863269e+06 * Joule / kilogram,5.05059722e+03 * Joule / kilogram / Kelvin,3.51150171e-03 * m3_kg, 8.02679899e+03 * Joule / kilogram / Kelvin,2.74234390e+03 * Joule / kilogram / Kelvin);}
		void test18(){testB23Curve(7.69526600e+02 * Kelvin, 5.34695092e+07 * Pascal, 2.45511404e+06 * Joule / kilogram,2.63700247e+06 * Joule / kilogram,5.04871563e+03 * Joule / kilogram / Kelvin,3.40172228e-03 * m3_kg, 7.71855023e+03 * Joule / kilogram / Kelvin,2.71851653e+03 * Joule / kilogram / Kelvin);}
		void test19(){testB23Curve(7.76728400e+02 * Kelvin, 5.64143764e+07 * Pascal, 2.46008327e+06 * Joule / kilogram,2.64639377e+06 * Joule / kilogram,5.04809777e+03 * Joule / kilogram / Kelvin,3.30253588e-03 * m3_kg, 7.42910110e+03 * Joule / kilogram / Kelvin,2.69411838e+03 * Joule / kilogram / Kelvin);}
		void test20(){testB23Curve(7.83930200e+02 * Kelvin, 5.94649771e+07 * Pascal, 2.46566620e+06 * Joule / kilogram,2.65669790e+06 * Joule / kilogram,5.04857075e+03 * Joule / kilogram / Kelvin,3.21250773e-03 * m3_kg, 7.15782134e+03 * Joule / kilogram / Kelvin,2.66877381e+03 * Joule / kilogram / Kelvin);}
		void test21(){testB23Curve(7.91132000e+02 * Kelvin, 6.26213114e+07 * Pascal, 2.47178596e+06 * Joule / kilogram,2.66781861e+06 * Joule / kilogram,5.04998355e+03 * Joule / kilogram / Kelvin,3.13044627e-03 * m3_kg, 6.90520662e+03 * Joule / kilogram / Kelvin,2.64299080e+03 * Joule / kilogram / Kelvin);}
		void test22(){testB23Curve(7.98333800e+02 * Kelvin, 6.58833792e+07 * Pascal, 2.47838089e+06 * Joule / kilogram,2.67967882e+06 * Joule / kilogram,5.05221438e+03 * Joule / kilogram / Kelvin,3.05536738e-03 * m3_kg, 6.67237039e+03 * Joule / kilogram / Kelvin,2.61818136e+03 * Joule / kilogram / Kelvin);}
		void test23(){testB23Curve(8.05535600e+02 * Kelvin, 6.92511806e+07 * Pascal, 2.48540877e+06 * Joule / kilogram,2.69222391e+06 * Joule / kilogram,5.05517359e+03 * Joule / kilogram / Kelvin,2.98644930e-03 * m3_kg, 6.45985545e+03 * Joule / kilogram / Kelvin,2.59621859e+03 * Joule / kilogram / Kelvin);}
		void test24(){testB23Curve(8.12737400e+02 * Kelvin, 7.27247156e+07 * Pascal, 2.49284571e+06 * Joule / kilogram,2.70541883e+06 * Joule / kilogram,5.05879877e+03 * Joule / kilogram / Kelvin,2.92298316e-03 * m3_kg, 6.26662916e+03 * Joule / kilogram / Kelvin,2.57866434e+03 * Joule / kilogram / Kelvin);}
		void test25(){testB23Curve(8.19939200e+02 * Kelvin, 7.63039841e+07 * Pascal, 2.50067886e+06 * Joule / kilogram,2.71923858e+06 * Joule / kilogram,5.06304221e+03 * Joule / kilogram / Kelvin,2.86432911e-03 * m3_kg, 6.08996791e+03 * Joule / kilogram / Kelvin,2.56600329e+03 * Joule / kilogram / Kelvin);}
		void test26(){testB23Curve(8.27141000e+02 * Kelvin, 7.99889862e+07 * Pascal, 2.50889508e+06 * Joule / kilogram,2.73365536e+06 * Joule / kilogram,5.06785483e+03 * Joule / kilogram / Kelvin,2.80989027e-03 * m3_kg, 5.92637966e+03 * Joule / kilogram / Kelvin,2.55725873e+03 * Joule / kilogram / Kelvin);}
		void test27(){testB23Curve(8.34342800e+02 * Kelvin, 8.37797218e+07 * Pascal, 2.51747018e+06 * Joule / kilogram,2.74862789e+06 * Joule / kilogram,5.07317352e+03 * Joule / kilogram / Kelvin,2.75911285e-03 * m3_kg, 5.77307809e+03 * Joule / kilogram / Kelvin,2.55022077e+03 * Joule / kilogram / Kelvin);}
		void test28(){testB23Curve(8.41544600e+02 * Kelvin, 8.76761910e+07 * Pascal, 2.52636403e+06 * Joule / kilogram,2.76409900e+06 * Joule / kilogram,5.07891878e+03 * Joule / kilogram / Kelvin,2.71151121e-03 * m3_kg, 5.62916714e+03 * Joule / kilogram / Kelvin,2.54225919e+03 * Joule / kilogram / Kelvin);}
		void test29(){testB23Curve(8.48746400e+02 * Kelvin, 9.16783938e+07 * Pascal, 2.53552496e+06 * Joule / kilogram,2.78000441e+06 * Joule / kilogram,5.08500548e+03 * Joule / kilogram / Kelvin,2.66670734e-03 * m3_kg, 5.49581253e+03 * Joule / kilogram / Kelvin,2.53142340e+03 * Joule / kilogram / Kelvin);}
		void test30(){testB23Curve(8.55948200e+02 * Kelvin, 9.57863301e+07 * Pascal, 2.54490330e+06 * Joule / kilogram,2.79629139e+06 * Joule / kilogram,5.09136457e+03 * Joule / kilogram / Kelvin,2.62446733e-03 * m3_kg, 5.37519493e+03 * Joule / kilogram / Kelvin,2.51737300e+03 * Joule / kilogram / Kelvin);}
		void test31(){testB23Curve(8.63150000e+02 * Kelvin, 1.00000000e+08 * Pascal, 2.55447021e+06 * Joule / kilogram,2.81294206e+06 * Joule / kilogram,5.09796903e+03 * Joule / kilogram / Kelvin,2.58471850e-03 * m3_kg, 5.26866769e+03 * Joule / kilogram / Kelvin,2.50176403e+03 * Joule / kilogram / Kelvin);}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:
		CPPUNIT_TEST_SUITE(B23CurveTest);

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
		CPPUNIT_TEST(test31);


		CPPUNIT_TEST_SUITE_END();
		
};

CPPUNIT_TEST_SUITE_REGISTRATION(B23CurveTest);
