#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "units.h"

#define CHECK_RESULT(method,val) if(!eq(S->method(),val,tol)){\
	stringstream s; \
	s<< #method << ": expected " << val << ", got " << S->method() << "."; \
	CPPUNIT_FAIL(s.str()); \
	}
