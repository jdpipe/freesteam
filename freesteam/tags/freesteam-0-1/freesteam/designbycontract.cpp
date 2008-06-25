#include "designbycontract.h"

#ifndef NDEBUG
#include <iostream>
#include <string>

/// Design-by-contract consistency checking
/**
	All classes inheriting from this class need to define a method IsValid. This method should perform a consistency check on the state of the object. Note that this method needs to be defined only when a debug build is made.
*/
bool DesignByContract::isValid() {
	return 0;
}

/// Abort program and show error message
/**
	This routine may need to be changed for different console/interface scenarios
*/
void DesignByContract::__abort_Program_(const char *file, const int line,
                                        const char *expr, const char *type) {
	
	// Compiler-style error message for use in TextPad 'IDE':
	
	std::cerr << std::endl << std::endl << file << ":" << line << ": " << type << " assertion failed: "<< expr << std::endl;


	//std::cerr << std::endl << std::endl << type << " FAILED" << std::endl;
	//std::cerr << "Assertion: " << expr << std::endl;
	//std::cerr << "File:      " << file << std::endl;
	//std::cerr << "Line:      " << line << std::endl;
	std::abort();
}

void DesignByContract::__report_Message_(const char *file, const int line, const char *msg){
	std::cerr << std::endl << file << ":" << line << ": " << msg;
}

#endif
