/**
	EES Hooks into freesteam library

	Based on examples_ees.h
	from Example C++ DLLs
	at http://www.fchart.com/ees/goodies.shtml
*/

#include "eesparameterlist.h"

#include "../steamcalculator.h"
#include "../solver2.h"
#include "../exception.h"

#include <windows.h>
#include <sstream>
#include <cstring>


// Don't mangle the following functions - they need to be visible in the DLL
extern "C"  {

	//---------------------------------------------------------------------------
	// Declare your functions and procedures
	//  (functions return a single double-precision value; for anything else, use procedure of FDL format)

	//------------------------------------------------------------------------
	// IMPLEMENT THE FUNCTIONS CALLABLE BY EES

	EES_FUNCTION_START(iapws_psat_T,"psat = iapws_psat_T(T)",1)

		// Calculating code here ...

		return Boundaries::getSatPres_T(fromcelsius(inputs[0])) / bar;

	EES_FUNCTION_END(iapws_psat_T)

	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Tell EES which functions and procedures are exported in the library

	/**
		Return a c-string of 'DLF' style functions to EES
	*/
	__declspec(dllexport)
	void
	DLFNames(char *Names)
	{
		strcpy(Names,"iapws_psat_T");
	}

	/**
		Return a c- string list of 'DLP' style procedures to EES
	*/
	__declspec(dllexport)
	void
	DLPNames(char *Names)
	{
	 	strcpy(Names,"\0");
	}

	/**
		Return a list of 'FDL' style procedures to EES
	*/
	__declspec(dllexport)
	void
	FDLNames(char *Names)
	{
		strcpy(Names,"\0");
	}

};

/*

// DLP procedures implementation
__declspec(dllexport)
void
iapws_pT_uv(
		char s[EES_MSG_LENGTH]
		, int mode
		, EES_PARAM_REC *rec_in
		, EES_PARAM_REC *rec_out)
{
	try{
		// Pull parameter lists in to EESParameterList class for convenience
		EESParameterList inputs(rec_in);
		EESParameterList outputs(rec_out);

		int NInputs = inputs.length();
		int NOutputs = outputs.length();

		// Return calling syntax when mode == -1
		if (mode==-1) {
			strncpy(s,"call lnlog_c(a : lnep, ldec)",);
			return;
		}

		// Check the number of inputs
		if (NInputs!=2) {
			stringstream ss;
			ss << "Expected 2 inputs, received " << NInputs;
			throw new Exception(ss.str());
		}

		// Check the number of outputs
		if (NOutputs!=2) {
			stringstream ss;
			ss << "Expected 2 outputs, received " << NOutputs;
			throw new Exception(ss.str());
			return;
		}

		// Calculating code here ...
			...

		outputs[0] = log(inputs[0]);
		outputs[1] = log10(inputs[0]);



		return;

	}catch(Exception *E){
		std::stringstream ss;
		ss << E->what();
		std::strncpy(s,ss.str().c_str(),EES_MSG_LENGTH-1);
		s[EES_MSG_LENGTH-1]='\0';
		delete E;
		return;
	}
}

*/


//----------------------------------------------------------------------
// Define the entry point for the dll
//   (probably no need to modify this)

BOOL APIENTRY DllMain(
		HANDLE hModule
		, DWORD  ul_reason_for_call
		, LPVOID lpReserved
){
    return TRUE;
}
