#include "../units.h"
#include "../surfacetension.h"
#include "../steamcalculator.h"
#include "../b13curve.h"

#include <sstream>
using namespace std;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class SurfaceTensionTest: public CppUnit::TestFixture{

protected:
	void testSigma(Temperature T, SurfaceTension sigma){
		SurfaceTension sigma_calc = surfaceTension(T);
		if(fabs(sigma_calc - sigma) > 0.01 * milli*Newton/metre){
			stringstream s;
			s << "Expected sigma = " << sigma << ", got sigma_calc = " << sigma_calc << " with T = " << T;
			CPPUNIT_FAIL(s.str());
		}
	}

public:
	void test1(){
		struct TestData{
			double temp_C;
			double sigma_mNpermetre;
		};
		struct TestData testdata[]={
			{0.01, 75.65}
			,{5, 74.94}
			,{30, 71.19}
			,{55,67.10}	
			,{80,62.67}
			,{105,57.94}
			,{130,52.93}
			,{180,42.19}
			,{205,36.53}
			,{230,30.74}
			,{255,24.87}
			,{280,18.99}
			,{305,13.22}
			,{330,7.70}
			,{355,2.74}
			,{370,0.39}
			,{0,0}
		};

		for(int i=0;testdata[i].temp_C!=0;++i){
			testSigma(
				fromcelsius(testdata[i].temp_C)
				,testdata[i].sigma_mNpermetre * milli*Newton/metre
			);
		}
	}

	void test2(){
		SteamCalculator S;
		S.setRegion4_Tx(fromcelsius(255),0.5);
		CPPUNIT_ASSERT(fabs( S.surftens() - 24.87 * milli*Newton/metre)
			 < 0.01 * milli*Newton/metre
		);
	}

	// Invoke CPPUNIT macros to add tests to a suite:
	CPPUNIT_TEST_SUITE(SurfaceTensionTest);
	CPPUNIT_TEST(test1);
	CPPUNIT_TEST(test2);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SurfaceTensionTest);
