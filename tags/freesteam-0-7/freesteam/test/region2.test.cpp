#include "../steamcalculator.h"
#include "batchtest.h"
#include "steamtestpoint.h"

#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

class Region2TestPoint : public SteamTestPoint{

	public:

	Region2TestPoint(double t, double p, double v, double h, double u, double s, double cp, double w)
		: SteamTestPoint(t,p,v,h,u,s,cp,w){}

	void test(double tol) const{
		SteamCalculator S;

		//cerr.flags(ios_base::showbase);
		//cerr << "    p = " << pres << ", T = " << temp << endl;

		S.set_pT(pres, temp, 1.0);

		CPPUNIT_ASSERT_EQUAL(2, S.whichRegion());

		// tol as a fraction of the expected value (eg 'v')
		CHECK_RESULT(specvol,      v);
		CHECK_RESULT(specenthalpy, h);
		CHECK_RESULT(specienergy,  u);
		CHECK_RESULT(specentropy,  s);
		CHECK_RESULT(speccp,       cp);
	}

};



class Region2RefDataTest:public BatchTest < Region2TestPoint > {

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(Region2RefDataTest);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp(){

			setTolerance(0.0000005);

			addTestPoint(Region2TestPoint(300, 0.0035, 0.394913866E2, 0.254991145E4, 0.241169160E4, 0.852238967E1, 0.191300162E1, 0.427920172E3));

			addTestPoint(Region2TestPoint(700, 0.0035, 0.923015898E2, 0.333568375E4, 0.301262819E4, 0.101749996E2, 0.208141274E1, 0.644289068E3));

			addTestPoint(Region2TestPoint(700, 30, 0.542946619E-02, 0.263149474E+4, 0.246861076E+4, 0.517540298E+1, 0.103505092E+2, 0.480386523E+3));

		}

		void tearDown() {

			data.clear();

		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(Region2RefDataTest);
