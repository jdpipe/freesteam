#include "test.h"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main( int argc, char* argv[] ) { 
	CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest(); 
	CppUnit::TextUi::TestRunner runner; 
	runner.addTest( suite ); 
	runner.setOutputter( CppUnit::CompilerOutputter::defaultOutputter( &runner.result(), std::cerr ) ); 
	bool res = runner.run(); 
	return (res? 0 : 1); 
}
