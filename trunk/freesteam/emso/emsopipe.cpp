/*

freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2005  John Pye

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "emsoextobj.h"
#include <exception>
#include <stdexcept>
#include "../common.h"

const int MAX_COLEBROOK_ITER=20;

#ifndef HAVE_EMSO
	#error EMSO NOT CONFIGURED ON YOUR SYSTEM, CHECK CPPFLAGS IN THE FILE PACKAGE_ROOT/make-xxxxx.mak FOR YOUR SYSTEM.
#endif

#ifdef EMSO_DEBUG
	#warning EMSO DEBUG MODE IS SET - START EMSO FROM A CONSOLE TO SEE DEBUG OUTPUT
#endif

#include <emso/external_object.h>

#include <sstream>
#include <cstring>
#include <map>
using namespace std;

/*
	Using the method from the 'stat' program in Oreilly C++ Programming, ch 27
	(see downloadable examples from Oreilly.com)
*/
#define METHOD_LIST \
	M(f,ReEd)

#define PARAM_LIST \
	P(Ed)

/**
	EMSO pipe pressure drop model

	TODO: integrate the 'params' and 'methods' stuff into a smarter parent class
*/
class EMSOpipe : public ExternalObjectStrategy {

public:
	virtual void create(const real_t *y0){
		// nothing to do here
	}

	virtual void destroy() = 0;

	virtual int_t getNumOfInputs(const int_t &method){
		return 2;
	}

	virtual int_t getNumOfOutputs(const int_t &method){
		return 1;
	}

	virtual const char *getInputUnits(const int_t &method, const int &index){
		return NULL;
	}

	virtual const char *getOutputUnits(const int_t &method, const int &index){
		return NULL;
	}

	virtual void calc(const int_t *methodID, const char *methodName, const real_t *inputValues, real_t *outputValues){
		try{

			#ifdef EMSO_DEBUG
				//cerr << "starting emsopipe::calc call" << endl;
				++numOfCalcCalls;
			#endif

			int_t method;
			method = *methodID;
			if(!method)
				method = convertMethod(string(methodName));

			if(method == 0){
				stringstream ss;
				ss << "Method '" << methodName << " not found";
				throw std::runtime_error(ss.str());
			}

			#ifdef EMSO_DEBUG
				cerr << "EMSOpipe[" << instanceNumber << "," << numOfCalcCalls << "]: " << methodName << "(";
				try{
			#endif
					switch(method){
						case f | given_ReEd:
							#ifdef EMSO_DEBUG
								cerr << "Re = " << inputValues[0] << "," << "eps/D = " << inputValues[1] << ")";
							#endif
							outputValues[0] = getFrictionFactor(inputValues[0],inputValues[1]);
							#ifdef EMSO_DEBUG
								cerr << " = " << outputValues[0] << endl;
							#endif
							break;

						default:
							throw std::runtime_error("EMSOpipe::calc: Method not implemented.");
					}

			#ifdef EMSO_DEBUG
				}catch(std::exception &E){
					stringstream ss;
					ss << methodName << ": error during evaluation: " << E.what() << endl;
					throw std::runtime_error(ss.str());
				}

				//cerr << "end emsopipe::calc call" << endl;
			#endif

			*retval = emso_ok;

		}catch(std::exception &E){
			stringstream ss;
			ss << "EMSOpipe::calc: " << E.what();

			report_error(msg, ss.str());
			*retval = emso_error;
		}
	}

private:

	double epsOnD;

	double getFrictionFactor(const double &Re, const double &epsOnD){

		if(Re <= 3000){
			return 64/Re;
		}

		if(!epsOnD){

			// Smooth pipe with Re > 3000, use Petukov

			if(Re > 5E6){
				throw std::runtime_error("EMSOpipe::frictionFactor: Reynolds number out of range");
			}

			return pow(0.790 * log(Re) - 1.64,-2.0);

		}else{

			// Iteration on Colebrook equation for rough pipes...

			double oneOnRootFnew = 0;
			double oneOnRootFold = 1;

			int iter=0;
			double tol=1e-5;

			while(1){
				oneOnRootFold = oneOnRootFnew;
				oneOnRootFnew = -0.86 * log(epsOnD/3.7 + 2.51/Re*oneOnRootFold);
				iter++;

				if(fabs(oneOnRootFnew-oneOnRootFold)<=tol){
					break;
				}

				if(iter>=MAX_COLEBROOK_ITER){
					stringstream s;
					s << "EMSOpipe::frictionFactor: Unable to converge Colebrook equation for pipe roughness with " << MAX_COLEBROOK_ITER << " iterations: ";
					s << " error now = " << fabs(oneOnRootFnew-oneOnRootFold) << " >  required " << tol;
					throw std::runtime_error(s.str());
				}
			}

			return 1.0 / sq(oneOnRootFnew);
		}
	}

	#ifdef EMSO_DEBUG
		bool debug;
	#endif

	/// Specific property options
	/**
		These must never give nonzero when ANDed with output_arguments or each other
	*/
	enum input_arguments {
		given_ReEd = 1

		, InputMask = 0x00000fff
	};

	/// Returned property options
	/**
		These must never give nonzero when ANDed with input_arguments or each other
	*/
	enum output_arguments{

		f		= 0x00010000

		,OutputMask = 0xffff0000
	};

/*
	See the comment at the top of the file for an explanation of this crazy stuff here
*/
#define M(OUT,IN) OUT ## _ ## IN = OUT | given_ ## IN
#define X ,
/// The methods
	typedef enum {
		METHOD_LIST
	} Method;
#undef M
#undef X
	typedef map<const string, Method> MethodMap;

	static MethodMap methodNames;

// Assign each method to the map:
#define M(OUT,IN) methodNames[#OUT "_" #IN] = OUT ## _ ## IN
#define X ;
	virtual void initialiseMethodNames(){
		if(!methodNames.size()){
			METHOD_LIST;
		}
	}
#undef M
#undef X
/*
	End of crazy stuff
*/

/*
	See the comment at the top of the file for an explanation of this crazy stuff here
*/
#define P(NAME) NAME
#define X ,
/// The methods
	typedef enum {
		PARAM_LIST
	} Param;
#undef P
#undef X
	typedef map<const string, Param> ParamMap;

	static ParamMap paramNames;

// Assign each method to the map:
#define P(NAME) paramNames[#NAME] = NAME
#define X ;
	void initialiseParamNames(){
		if(!paramNames.size()){
			PARAM_LIST;
		}
	}
#undef P
#undef X
/*
	End of crazy stuff
*/

};


EMSOpipe::MethodMap EMSOpipe::methodNames=EMSOpipe::MethodMap();
EMSOpipe::ParamMap EMSOpipe::paramNames=EMSOpipe::ParamMap();


EXPORT_EXTERNAL_OBJECT(EMSOpipe)
