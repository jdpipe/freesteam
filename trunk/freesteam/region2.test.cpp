#include "steamcalculator.h"
#include "batchtest.h"
#include "steamtestpoint.h"

#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

class Region2RefDataTest:public BatchTest < SteamTestPoint > {

	private:

		SteamCalculator * S;

		void testOnePoint(SteamTestPoint p) {

			S->set_pT(p.pres, p.temp);

			CPPUNIT_ASSERT(S->whichRegion() == 2);

			CHECK_RESULT(specvol,      p.v);
			CHECK_RESULT(specenthalpy, p.h);
			CHECK_RESULT(specienergy,  p.u);
			CHECK_RESULT(specentropy,  p.s);
			CHECK_RESULT(speccp,       p.cp);

		}
		
	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(Region2RefDataTest);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp(){

			S = new SteamCalculator();

			setTolerance(0.0000005);

			addTestPoint(SteamTestPoint
			             (300, 0.0035, 0.394913866E2, 0.254991145E4,
			              0.241169160E4, 0.852238967E1, 0.191300162E1,
			              0.427920172E3));

			addTestPoint(SteamTestPoint
			             (700, 0.0035, 0.923015898E2, 0.333568375E4,
			              0.301262819E4, 0.101749996E2, 0.208141274E1,
			              0.644289068E3));

			addTestPoint(SteamTestPoint
			             (700, 30, 0.542946619E-02, 0.263149474E+4,
			              0.246861076E+4, 0.517540298E+1, 0.103505092E+2,
			              0.480386523E+3));

		}

		void tearDown() {

			delete S;

			data.clear();

		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(Region2RefDataTest);
