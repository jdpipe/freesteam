#include "steamcalculator.h"
#include "test.h"

#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

class Region1RefDataTest:public BatchTest<SteamTestPoint > {

	private:

		SteamCalculator * S;

		void testOnePoint(SteamTestPoint p) {

			S->set_pT(p.pres, p.temp);

			CPPUNIT_ASSERT(S->whichRegion() == 1);
			
			CHECK_RESULT(specvol,      p.v);
			CHECK_RESULT(specenthalpy, p.h);
			CHECK_RESULT(specienergy,  p.u);
			CHECK_RESULT(specentropy,  p.s);
			CHECK_RESULT(speccp,       p.cp);
		}
		
	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(Region1RefDataTest);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp(){

			S = new SteamCalculator();

			setTolerance(0.0000005);

/*

SOURCE TEST DATA http://www.iapws.org/relguide/IF97.pdf#page=9

Table 5. Thermodynamic property values calculated from Eq. (7) for selected values of T and p*

T/(K)		300				300				500
p/(MPa)		3				80				80
v/(m3/kg)	0.100215168E-2	0.971180894E-3	0.120241800E-2
h/(kJ/kg)	0.115331273E3	0.184142828E3	0.975542239E3
u/(kJ/kg)	0.112324818E3	0.106448356E3	0.971934985E3
s/(kJ/kgK)	0.392294792		0.368563852		0.258041912E1
cp/(kJ/kgK)	0.417301218E1	0.401008987E1	0.465580682E1
w/(m/s)		0.150773921E4	0.163469054E4	0.124071337E4

* It is recommended to verify programmed functions using 8 byte real values for all three combinations of T and
p given in this table.

*/

			addTestPoint(SteamTestPoint(300, 3, 0.100215168E-2, 0.115331273E3, 0.112324818E3, 0.392294792, 0.417301218E1, 0.150773921E4));

			addTestPoint(SteamTestPoint(300, 80, 0.971180894E-3, 0.184142828E3, 0.106448356E3, 0.368563852, 0.401008987E1, 0.163469054E4));

			addTestPoint(SteamTestPoint(500, 80, 0.120241800E-2, 0.975542239E3, 0.971934985E3, 0.258041912E1, 0.465580682E1, 0.124071337E4));

		}

		void tearDown() {

			delete S;

			data.clear();

		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(Region1RefDataTest);
