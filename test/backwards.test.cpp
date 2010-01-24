#include "../backwards.h"
#include "../backwards_impl.h"

#include <cmath>
#include <sstream>

using namespace std;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

/*
	Test for the backwards equations T(p,h) etc
*/
class BackwardsTest: public CppUnit::TestFixture{

	public:
		void tearDown(){
			// nothing
		}

		void setUP(){
			// nothing
		}

	protected:
		void test_B2bc(){
			//cerr << endl;
			//cerr << "B2BC_PH(p,h) = " << REGION2_B2BC_PH(100e6, 3516004.323) << endl;
			CPPUNIT_ASSERT(fabs(REGION2_B2BC_PH(100e6, 3516004.323))<3e-8);
		}

		void test_B3ab(){
			//cerr << endl;
			//cerr << "B3AB_PH(p,h) = " << REGION3_B3AB_PH(25e6, 2095936.454) << endl;
			CPPUNIT_ASSERT(fabs(REGION3_B3AB_PH(25e6, 2095936.454))<5e-8);
		}

		void test_Tph_3a(){
			CPPUNIT_ASSERT(fabs(freesteam_region3_T_ph(20e6,1700e3)-629.3083892)<1e-7);
			CPPUNIT_ASSERT(fabs(freesteam_region3_T_ph(50e6,1700e3)-690.5718338)<1e-7);
			CPPUNIT_ASSERT(fabs(freesteam_region3_T_ph(100e6,1700e3)-733.6163014)<1e-7);
		}

	public:
		// Invoke CPPUNIT macros to add tests to a suite:
		CPPUNIT_TEST_SUITE(BackwardsTest);
				
		CPPUNIT_TEST(test_B2bc);
		CPPUNIT_TEST(test_B3ab);
		CPPUNIT_TEST(test_Tph_3a);

		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(BackwardsTest);
