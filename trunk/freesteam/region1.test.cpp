#include "steamcalculator.h"
#include "batchtest.h"
#include "steamtestpoint.h"

#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

class Region1TestPoint : public SteamTestPoint{
	
	public:
	
	Region1TestPoint(double t, double p, double v, double h, double u, double s, double cp, double w)
		: SteamTestPoint(t,p,v,h,u,s,cp,w){}
	
	void test(double tol) const{
		SteamCalculator S;

		//cerr.flags(ios_base::showbase);
		//cerr << "    p = " << pres << ", T = " << temp << endl;
		
		S.set_pT(pres, temp);
		
		CPPUNIT_ASSERT_EQUAL(1, S.whichRegion());

		// tol as a fraction of the expected value (eg 'v')
		CHECK_RESULT(specvol,      v);
		CHECK_RESULT(specenthalpy, h);
		CHECK_RESULT(specienergy,  u);
		CHECK_RESULT(specentropy,  s);
		CHECK_RESULT(speccp,       cp);
		
	}

};
	
	
class Region1RefDataTest : public BatchTest<Region1TestPoint > {
		
	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(Region1RefDataTest);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp(){

			setTolerance(0.0000005);

/*

SOURCE TEST DATA http://www.iapws.org/relguide/IF97.pdf#page=9

Table 5. Thermodynamic property values calculated from Eq. (7) for selected values of T and p*

T/(K)		300				300				500
p/(MPa)		3				80				3
v/(m3/kg)	0.100215168E-2	0.971180894E-3	0.120241800E-2
h/(kJ/kg)	0.115331273E3	0.184142828E3	0.975542239E3
u/(kJ/kg)	0.112324818E3	0.106448356E3	0.971934985E3
s/(kJ/kgK)	0.392294792		0.368563852		0.258041912E1
cp/(kJ/kgK)	0.417301218E1	0.401008987E1	0.465580682E1
w/(m/s)		0.150773921E4	0.163469054E4	0.124071337E4

* It is recommended to verify programmed functions using 8 byte real values for all three combinations of T and
p given in this table.

*/

			addTestPoint(Region1TestPoint(300, 3, 0.100215168E-2, 0.115331273E3, 0.112324818E3, 0.392294792, 0.417301218E1, 0.150773921E4));

			addTestPoint(Region1TestPoint(300, 80, 0.971180894E-3, 0.184142828E3, 0.106448356E3, 0.368563852, 0.401008987E1, 0.163469054E4));

			addTestPoint(Region1TestPoint(500, 3, 0.120241800E-2, 0.975542239E3, 0.971934985E3, 0.258041912E1, 0.465580682E1, 0.124071337E4));

		}

		void tearDown() {

			data.clear();

		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(Region1RefDataTest);
