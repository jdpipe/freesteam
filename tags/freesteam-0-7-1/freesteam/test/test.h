#ifndef TEST_H
#define TEST_H

#include "../units.h"

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

#ifndef HAVE_CPPUNIT
	#error CPPUNIT IS NOT AVAIABLE: CHECK YOUR ../make-xxxxxx.mak FILE AND SET CPPFLAGS WITH -DHAVE_CPPUNIT
#else
	#include <cppunit/extensions/TestFactoryRegistry.h>
	#include <cppunit/ui/text/TestRunner.h>
	#include <cppunit/CompilerOutputter.h>
	#include <cppunit/TestCase.h>
	#include <cppunit/extensions/HelperMacros.h>

	#define CHECK_RESULT(method,val) if(!eq(S.method(),val,val * tol)){\
		stringstream s; \
		s.flags(ios_base::showbase); \
		s<< #method << ": expected " << val << ", got " << S.method() << "."; \
		CPPUNIT_FAIL(s.str()); \
		}

#endif


#endif
