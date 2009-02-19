#include "designbycontract.h"

#ifndef NDEBUG

#include "exception.h"
#include <iostream>
#include <sstream>
#include <string>

/// Design-by-contract consistency checking
/**
	All classes inheriting from this class need to define a method IsValid. This method should perform a consistency check on the state of the object. Note that this method needs to be defined only when a debug build is made.
*/
bool DesignByContract::isValid() const{
	return 0;
}

/// Abort program and show error message
/**
	This routine may need to be changed for different console/interface scenarios
*/
void DesignByContract::__throw_error_(const char *file, const int line,
                                        const char *expr, const char *type) {

	// Compiler-style error message for use in TextPad 'IDE':
	std::stringstream s;
	s << std::endl << file << ":" << line << ": " << type << " assertion failed: "<< expr;


	//std::cerr << std::endl << std::endl << type << " FAILED" << std::endl;
	//std::cerr << "Assertion: " << expr << std::endl;
	//std::cerr << "File:      " << file << std::endl;
	//std::cerr << "Line:      " << line << std::endl;
	// std::abort();

	throw new Exception(s.str());
}

void DesignByContract::__report_Message_(const char *file, const int line, const char *msg){
	std::cerr << std::endl << file << ":" << line << ": " << msg;
}

#endif
