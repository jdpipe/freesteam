#include "zeroin.h"
#include "steamtestpoint.h"

#include <sstream>

using namespace std;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class ZeroInTest: public CppUnit::TestFixture{

	public:
	
		double quadratic(const double &x){
			this->x=x;	
			return x*x - 4*x - 4;
		}

		void tearDown() {
		}
		
		void setUP(){
		}

	protected:	
		
		double x;
		
		void Test1(){
			
			try{
				
				double expected = 2.0;
				
				ZeroIn<ZeroInTest> z;
				z.setLowerBound(-10);
				z.setUpperBound(4.566);
				z.setMethod(&ZeroInTest::quadratic);
				z.setTolerance(1e-10);
				z.visit(this);
				
				if(fabs(x - expected) > 1e8){
					stringstream s;
					s << "Failed to solve quadratic. Expected solution was " << expected << " but returned value was " << x;
					CPPUNIT_FAIL(s.str());
				}
				
			}catch(Exception *e){
				CPPUNIT_FAIL(e->what());
			}
		}


	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(ZeroInTest);
		CPPUNIT_TEST(Test1);
		CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ZeroInTest);
