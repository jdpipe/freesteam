#include "batchtest.h"
#include "../common.h"

#include <cmath>
#include <vector>
#include <sstream>
#include <map>
using namespace std;

class DoublePair{

	private:

		double first;
		double second;

	public:

		DoublePair(double f,double s) : first(f), second(s){}

		void test(double tol) const{

			double y=sq(first);
			//cerr << "    Testing " << second << " = sq( " << first << ")..." << endl;
			CPPUNIT_ASSERT(fabs(y-second)<tol);

		}
};

class BatchTestTest : public BatchTest <DoublePair> {

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(BatchTestTest);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp(){

			setTolerance(0.0000005);

			addTestPoint(DoublePair(0,0));
			addTestPoint(DoublePair(1,1));
			addTestPoint(DoublePair(2,4));
			addTestPoint(DoublePair(16,256));
		}

		void tearDown() {

			data.clear();

		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(BatchTestTest);
