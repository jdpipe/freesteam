#include "steamcalculator.h"
#include "batchtest.h"
#include "steamtestpoint.h"

#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

class Region4TestPoint{

	public:
		Pressure p;
		Temperature T;

		/**
			Constructor uses doubles to facilitate initialisation from arrays.
			@param p Pressure / MPa
			@param T Temperature / K
		*/
		Region4TestPoint(double T, double p){
			this->T = T * Kelvin;
			this->p = p * MPa;
		}

		void test(double tol) const{

			try{

				// Test forwards equation
				Pressure p_sat = Boundaries::getSatPres_T(T);
				if(fabs(p - p_sat)/p_sat > tol){
					stringstream s;
					s.flags(ios_base::showbase);
					s << "Saturation-Pressure Basic Equation gave wrong value at T = " << T;
					s << "(calculated value was " << p_sat << ", expected " << p << ")";
					CPPUNIT_FAIL(s.str());
				}

				/*
				Temperature T_sat = Boundaries::getSatTemp_p(p);
				if(fabs(T_sat - T)/T > tol){
					stringstream s;
					s.flags(ios_base::showbase);
					s << "Saturation-Pressure Basic Equation gave wrong value at T = " << T;
					s << "(calculated value was " << T_sat << ", expected " << T << ", with p = " << p << ")";
					CPPUNIT_FAIL(s.str());
				}
				*/

			}catch(Exception *E){
				stringstream s;
				s << "Region4TestPoint::test: " << E->what();
				CPPUNIT_FAIL(s.str());
			}
		}

	public:

};


class Region4Test : public BatchTest<Region4TestPoint > {

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(Region4Test);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp(){

			setTolerance(0.0000005);

/*

SOURCE TEST DATA http://www.iapws.org/relguide/IF97.pdf#page=34

Table 35. Saturation pressures calculated from Eq. (30) for selected values of T *

T/K p s /MPa
300	0.353658941E-2
500	0.263889776E1
600	0.123443146E2

* It is recommended to verify the programmed equation
using 8 byte real values for all three values of T given
in this table.

*/

			addTestPoint(Region4TestPoint(300,	0.353658941E-2));
			addTestPoint(Region4TestPoint(500,	0.263889776E1));
			addTestPoint(Region4TestPoint(600,	0.123443146E2));

		}

		void tearDown() {

			data.clear();

		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(Region4Test);
