#include "test.h"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

template<class TestPoint> 
class BatchTest : public CppUnit::TestFixture {

	typedef typename vector<TestPoint>::iterator TestPointIterator;
	
	protected:
		vector < TestPoint > data;

		double tol;

		void testAllPoints(){	
			
			//cerr << endl;
			//cerr << " Testing " << data.size() << " points." << endl;

			for (TestPointIterator i = data.begin(); i < data.end(); ++i) {
				this->testOnePoint(*i);
			}
		}

		void testOnePoint(const TestPoint testpoint) {
			
			testpoint.test(tol);

		}

		void setTolerance(double tol) {
			this->tol = tol;
		}

		void addTestPoint(const TestPoint& p) {
			data.push_back(p);
		}

	public:
		virtual void setUp(){
			throw new Exception("'setUp' not implemented");
		}
		
		virtual void tearDown(){
			throw new Exception("'tearDown' not implemented");
		}
	
};