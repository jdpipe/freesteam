#include "steamcalculator.h"
#include "boundaries.h"

#include "test.h"

#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

class B23RefDataTest : public CppUnit::TestFixture{
		
	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(B23RefDataTest);
		CPPUNIT_TEST(testForwards);
		CPPUNIT_TEST(testBackwards);
		CPPUNIT_TEST_SUITE_END();
	
	public:

		void setUp(){
			tol=0.0005;
			p = 0.165291643E2 * MPa;
			T = 0.623150000E3 * Kelvin;
		}
		void tearDown(){}
	
	private:
		double tol;
		Pressure p;
		Temperature T;
	
	protected:
		
		void testForwards(){
			Temperature T_bound = Boundaries::getTbound_p(p);
			//cerr.flags(ios_base::showbase);
			//cerr << "p = " << p / MPa << " MPa" << endl;
			//cerr << "T = " << T << endl;
			//cerr << "T_bound = " << T_bound << endl;
			//cerr << "fabs(T-T_bound) = " << fabs(T-T_bound) << endl;
			
			if(!eq(T_bound,T,5.0E-7 * Kelvin)){
				CPPUNIT_FAIL("T_bound(p) equation failed");
			}
		}
			
		void testBackwards(){
			Pressure p_bound = Boundaries::getpbound_T(T);
			//cerr.flags(ios_base::showbase);
			//cerr << "T = " << T << endl;
			//cerr << "p = " << p / MPa << "MPa" << endl;
			//cerr << "p_bound = " << p_bound / MPa << "MPa" << endl;
			//cerr << "p error = " << (p - p_bound) / Pascal << "Pa" << endl;
			//cerr << "fabs(p-p_bound) = " << fabs(p-p_bound) << endl;
			
			if(!eq(p_bound,p,0.05 * Pascal)){
				CPPUNIT_FAIL("p_bound(T) equation failed");
			}
		}

};

CPPUNIT_TEST_SUITE_REGISTRATION(B23RefDataTest);
