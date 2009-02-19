#include "test.h"

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

class EmptyTest : public CppUnit::TestFixture {
	
	protected:

		void testTestFail() {
			//CPPUNIT_FAIL("'testOnePoint' not implemented.");
		}

		void testTestPass() {
			CPPUNIT_ASSERT(true);
		}

	public:
		void setUp(){}
		
		void tearDown(){}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(EmptyTest);
		CPPUNIT_TEST(testTestFail);
		CPPUNIT_TEST(testTestPass);
		CPPUNIT_TEST_SUITE_END();
	
};

CPPUNIT_TEST_SUITE_REGISTRATION(EmptyTest);
