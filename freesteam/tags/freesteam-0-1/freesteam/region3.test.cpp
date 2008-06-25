#include "steamcalculator.h"
#include "batchtest.h"
#include "steamtestpoint.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include <cmath>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;



/**
	In region 3, points are defined in terms of temp,density, so define them that way here
	
	NOTE this approach will introduce some errors since rootfinding is used to find the target pres value, rather than defining the given rho.
*/
class Region3TestPoint : public SteamTestPoint {

	public:

		/// Easy initialiser for values of known units
		Region3TestPoint(double t, double rho, double p, double h, double u,
		                 double s, double cp, double w) : SteamTestPoint(t,p,1/rho,h,u,s,cp,w){
		}

		void test(double tol) const{
			
			SteamCalculator S;
			
			cerr.flags(ios_base::showbase);
			
			//cerr << endl << "TESTPOINT: p = " << pres << ", T = " << temp;
			
			S.set_pT(pres, temp);

			CPPUNIT_ASSERT_EQUAL(3,S.whichRegion());

			CHECK_RESULT(pres,     pres);
			CHECK_RESULT(specvol,     v);
			CHECK_RESULT(specentropy, s);
			CHECK_RESULT(speccp,      cp);
			CHECK_RESULT(specienergy, u);
			CHECK_RESULT(specenthalpy,h);
		}
};



class Region3RefDataTest : public BatchTest<Region3TestPoint>{

	private:

		SteamCalculator * S;
		double tol;



	public:

		CPPUNIT_TEST_SUITE(Region3RefDataTest);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp() {

			setTolerance(5e-4);
			
			cerr << setprecision(15);
			
			data.
			push_back(Region3TestPoint
			          (650.00000000, 500.0000, 0.255837018E2,
			           0.186343019E4, 0.181226279E4, 0.405427273E1,
			           0.138935717E2, 0.502005554E3));

			data.
			push_back(Region3TestPoint
			          (650.0000000, 200.00000000, 0.222930643E2,
			           0.237512401E4, 0.226365868E4, 0.485438792E1,
			           0.446579342E2, 0.383444594E3));
			data.
			push_back(Region3TestPoint
			          (750, 500, 0.783095639E2, 0.225868845E4,
			           0.210206932E4, 0.446971906E1, 0.634165359E1,
			           0.760696041E3));

		}

		void tearDown() {

			data.clear();

		}

};

CPPUNIT_TEST_SUITE_REGISTRATION(Region3RefDataTest);
