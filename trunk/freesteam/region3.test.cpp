#include "steamcalculator.h"
#include "test.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include <cmath>
#include <vector>
#include <sstream>

using namespace std;


class Region3RefDataTest : public BatchTest<SteamTestPointR3>{

	private:

		SteamCalculator * S;
		double tol;

		void testOnePoint(SteamTestPointR3 p) {

			S->set_pT(p.pres, p.temp);

			CPPUNIT_ASSERT(S->whichRegion() == 3);

			CHECK_RESULT(pres, p.pres);
			CHECK_RESULT(specenthalpy, p.h);
			CHECK_RESULT(specienergy, p.u);
			CHECK_RESULT(specentropy, p.s);
			CHECK_RESULT(speccp, p.cp);

		}

	public:

		CPPUNIT_TEST_SUITE(Region3RefDataTest);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp() {

			S = new SteamCalculator();

			tol = 0.0000005;

			data.
			push_back(SteamTestPointR3
			          (650.00000000, 500.0000, 0.255837018E2,
			           0.186343019E4, 0.181226279E4, 0.405427273E1,
			           0.138935717E2, 0.502005554E3));

			data.
			push_back(SteamTestPointR3
			          (650.0000000, 200.00000000, 0.222930643E2,
			           0.237512401E4, 0.226365868E4, 0.485438792E1,
			           0.446579342E2, 0.383444594E3));
			data.
			push_back(SteamTestPointR3
			          (750, 500, 0.783095639E2, 0.225868845E4,
			           0.210206932E4, 0.446971906E1, 0.634165359E1,
			           0.760696041E3));

		}

		void tearDown() {

			delete S;

			data.clear();

		}

};

CPPUNIT_TEST_SUITE_REGISTRATION(Region3RefDataTest);
