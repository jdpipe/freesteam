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

#include "../steamcalculator.h"
#include "../solver2.h"
#include "../units.h"

#ifndef HAVE_EMSO
	#error EMSO NOT CONFIGURED ON YOUR SYSTEM, CHECK CPPFLAGS IN THE FILE PACKAGE_ROOT/make-xxxxx.mak FOR YOUR SYSTEM.
#endif

#ifdef EMSO_DEBUG
	#warning EMSO DEBUG MODE IS SET - START EMSO FROM A CONSOLE TO SEE DEBUG OUTPUT
#endif

#ifdef SOLVER2_DEBUG
	#ifdef EMSO_DEBUG
		#define SOLVER2_DEBUG_PARAM debug
	#else
		#define SOLVER2_DEBUG_PARAM false
	#endif
#else
	#define SOLVER2_DEBUG_PARAM
#endif

#include "emsoextobj.h"

#include <sstream>
#include <cstring>
#include <map>
using namespace std;

/*
	Using the method from the 'stat' program in Oreilly C++ Programming, ch 27
	(see downloadable examples from Oreilly.com)
*/
#define METHOD_LIST \
	M(p,waterT) X M(x,waterT) X M(v,waterT) X M(u,waterT) X M(s,waterT) X M(h,waterT) \
	X M(cp,waterT) X M(cv,waterT) X M(k,waterT) X M(mu,waterT) \
	X M(rho,waterT) \
	X M(region,waterT) \
	X M(psvhx,waterT) \
	X M(psh,waterT) \
	X M(ps,waterT) \
	X M(p,steamT) X M(x,steamT) X M(v,steamT) X M(u,steamT) X M(s,steamT) X M(h,steamT) \
	X M(cp,steamT) \
	X M(cv,steamT) \
	X M(k,steamT) \
	X M(mu,steamT) \
	X M(rho,steamT) X M(region,steamT) \
	X M(psvhx,steamT) \
	X M(psh,steamT) \
	X M(ps,steamT) \
	X M(x,pT) X M(v,pT) X M(u,pT) X M(s,pT) X M(h,pT) \
	X M(cp,pT) X M(cv,pT) X M(k,pT) X M(mu,pT) \
	X M(rho,pT) \
	X M(region,pT) \
	X M(hs,pT) \
	X M(T,ph) X M(x,ph) X M(v,ph) X M(u,ph) X M(s,ph) \
	X M(cp,ph) X M(cv,ph) X M(k,ph) X M(mu,ph) \
	X M(rho,ph) X M(region,ph) \
	X M(Ts,ph) \
	X M(T,ps) X M(x,ps) X M(v,ps) X M(u,ps) X M(h,ps) \
	X M(cp,ps) X M(cv,ps) X M(k,ps) \
	X M(mu,ps) \
	X M(rho,ps) X M(region,ps) \
	X M(Th,ps) \
	X M(T,pu) X M(x,pu) X M(v,pu) X M(s,pu) X M(h,pu) \
	X M(cp,pu) X M(cv,pu) X M(k,pu) X M(mu,pu) \
	X M(rho,pu) X M(region,pu) \
	X M(Th,pu) \
	X M(Ts,pu) \
	X M(hs,pu) \
	X M(T,uv) X M(p,uv) X M(x,uv) X M(s,uv) X M(h,uv) \
	X M(cp,uv) X M(cv,uv) X M(k,uv) X M(mu,uv) \
	X M(rho,uv) X M(region,uv) \
	X M(Th,uv) \
	X M(Ts,uv) \
	X M(hs,uv) \
	X M(psh,uv) \
	X M(ps,uv) \
	X M(pshT,uv) \
	X M(psT,uv) \
	X M(p,Ts) X M(x,Ts) X M(v,Ts) X M(u,Ts) X M(h,Ts) \
	X M(cp,Ts) X M(cv,Ts) X M(k,Ts) X M(mu,Ts) \
	X M(rho,Ts) X M(region,Ts) \
	X M(ph,Ts) \
	X M(p,Th) X M(x,Th) X M(v,Th) X M(u,Th) X M(s,Th) \
	X M(cp,Th) X M(cv,Th) X M(k,Th) X M(mu,Th) \
	X M(rho,Th) X M(region,Th) \
	X M(ps,Th) \
	X M(Tsvx,ph) \
	X M(Tsvx,pu) \
	X M(Tuvx,ps) \
	X M(Thvx,ps) \
	X M(Tuvx,ph)

/**
	EMSO freesteam integration

	This class allows IAPWS-IF97 steam properties to be calculated from EMSO :-D
*/
class EMSOfreesteam : public ExternalObjectStrategy {

public:

	/**
		Constructor
	*/
	EMSOfreesteam(){
		#ifdef EMSO_DEBUG
			debug=false;

			//numOfCalcCalls = 0;
			//numOfInstances++;
			//instanceNumber = instanceSerialNumber++;

			// cerr << "Created EMSOfreesteam object " << instanceNumber << " (now " << numOfInstances << " instances)..." << endl;
			cerr << "Created EMSOfreesteam object..." << endl;
		#endif
	};

	/**
		Destructor
	*/
	virtual ~EMSOfreesteam(){
		#ifdef EMSO_DEBUG
			//numOfInstances--;

			//cerr << "Deleting EMSOfreesteam object " << instanceNumber << " (leaves " << numOfInstances << " instances)..." << endl;
			cerr << "Deleting EMSOfreesteam object..." << endl;
		#endif
	};

	/**
		Create object.
		Anything risking should go here and throw an exception if necessary
	*/
	virtual void create(){
		// nothing special here. don't need to use y0 for anything really.
	}

	/**
		Destroys an instance of this object in EMSO
	*/
	virtual void destroy(){
		// nothing special here either
	}


	virtual int_t getNumOfInputs(const int_t &method){

		// The number of input parameters:
		switch(method & InputMask){

			case given_steamT:
			case given_waterT:
				return 1;

			case given_pT:
			case given_ph:
			case given_pu:
			case given_uv:
			case given_ps:
			case given_Ts:
			case given_Th:
				return 2;

			default:
				throw std::runtime_error("EMSOfreesteam::getNumOfInputs: Invalid input parameter combo");
		}
	}

	virtual int_t getNumOfOutputs(const int_t &method){
		// Number of output parameters:
		switch(method & OutputMask){
			case T:
			case p:
			case h:
			case s:
			case u:
			case v:
			case x:
			case cp:
			case cv:
			case k:
			case rho:
			case mu:
			case region:
				return 1;

			case Th:
			case Ts:
			case hs:
			case ph:
			case ps:
				return 2;

			case psh:
			case psT:
				return 3;

			case Tsvx:
			case Tuvx:
			case Thvx:
			case psvx:
			case pshT:
				return 4;

			case psvhx:
				return 5;


			default:
				throw std::runtime_error("EMSOfreesteam::getNumOfOutputs: Invalid output parameter combo");
		}
	}

	virtual void getInputUnits(const int_t &method, char *inputUnits[]){
		// the method inputs
		switch(method & InputMask){
			case given_pT:
				strcpy(inputUnits[0], "MPa");
				strcpy(inputUnits[1], "K");
				break;

			case given_ph:
			case given_pu:
				strcpy(inputUnits[0], "MPa");
				strcpy(inputUnits[1], "kJ/kg");
				break;

			case given_ps:
				strcpy(inputUnits[0], "MPa");
				strcpy(inputUnits[1], "kJ/kg/K");
				break;

			case given_uv:
				strcpy(inputUnits[0], "kJ/kg");
				strcpy(inputUnits[1], "m^3/kg");
				break;

			case given_Ts:
				strcpy(inputUnits[0], "K");
				strcpy(inputUnits[1], "kJ/kg/K");
				break;

			case given_Th:
				strcpy(inputUnits[0], "K");
				strcpy(inputUnits[1], "kJ/kg");
				break;

			case given_waterT:
			case given_steamT:
				strcpy(inputUnits[0], "K");
				break;

			default:
				throw std::runtime_error("Invalid input param combination (when giving EMSO units for method inputs)");
		};
	}

	virtual void getOutputUnits(const int_t &method, char *outputUnits[]){
		// the outputs
		switch(method & OutputMask){

			// One output:
			case T:
				strcpy(outputUnits[0], "K");
				break;

			case p:
				strcpy(outputUnits[0], "MPa");
				break;

			case h:
			case u:
				strcpy(outputUnits[0], "kJ/kg");
				break;

			case s:
			case cp:
			case cv:
				strcpy(outputUnits[0], "kJ/(kg*K)");
				break;

			case x:
				break;

			case k:
				strcpy(outputUnits[0], "W/(m*K)");
				break;
			case mu:
				strcpy(outputUnits[0], "Pa*s");
				break;
			case v:
				strcpy(outputUnits[0], "m^3/kg");
				break;

			case rho:
				strcpy(outputUnits[0], "kg/m^3");
				break;

			// Two outputs
			case Th:
				strcpy(outputUnits[0], "K");
				strcpy(outputUnits[1], "kJ/kg");
				break;

			case Ts:
				strcpy(outputUnits[0], "K");
				strcpy(outputUnits[1], "kJ/kg/K");
				break;

			case hs:
				strcpy(outputUnits[0], "kJ/kg");
				strcpy(outputUnits[1], "kJ/kg/K");
				break;

			case ph:
				strcpy(outputUnits[0], "MPa");
				strcpy(outputUnits[1], "kJ/kg");
				break;

			case ps:
				strcpy(outputUnits[0], "MPa");
				strcpy(outputUnits[1], "kJ/kg/K");
				break;

			// Three outputs
			case psh:
				strcpy(outputUnits[0], "MPa");
				strcpy(outputUnits[1], "kJ/kg/K");
				strcpy(outputUnits[2], "kJ/kg");
				break;

			case psT:
				strcpy(outputUnits[0], "MPa");
				strcpy(outputUnits[1], "kJ/kg/K");
				strcpy(outputUnits[2], "K");
				break;

			// Four outputs
			case Tsvx:
				strcpy(outputUnits[0], "K");
				strcpy(outputUnits[1], "kJ/kg/K");
				strcpy(outputUnits[2], "m^3/kg");
				break;

			case Tuvx:
				strcpy(outputUnits[0], "K");
				strcpy(outputUnits[1], "kJ/kg");
				strcpy(outputUnits[2], "m^3/kg");
				break;

			case Thvx:
				strcpy(outputUnits[0], "K");
				strcpy(outputUnits[1], "kJ/kg");
				strcpy(outputUnits[2], "m^3/kg");
				break;

			case psvx:
				strcpy(outputUnits[0], "MPa");
				strcpy(outputUnits[1], "kJ/kg/K");
				strcpy(outputUnits[2], "m^3/kg");
				break;

			case pshT:
				strcpy(outputUnits[0], "MPa");
				strcpy(outputUnits[1], "kJ/kg/K");
				strcpy(outputUnits[2], "kJ/kg");
				strcpy(outputUnits[3], "K");
				break;


			// Five outputs:
			case psvhx:
				strcpy(outputUnits[0], "MPa");
				strcpy(outputUnits[1], "kJ/kg/K");
				strcpy(outputUnits[2], "m^3/kg");
				strcpy(outputUnits[3], "kJ/kg");
				break;

			case region:
				break;

			default:
				throw std::runtime_error("Invalid output param combination (when giving EMSO units for method outputs)");
		}
	}

	/// Perform a calculation
	/**
		@param methodID ID of method being called (@see checkMethod)
		@param numOfInputs Not used here
		@param totalInputLength Not used here
		@param inputValues values of the parameters used for this function, see enum method_name
		@param numOfOutputs Not used here
		@param outputLengths Not used here
		@param totalOutputLength Not used here
		@param outputDerivatives Not used... yet
		@param msg A place to put an error message
		@return EMSO status code (see emso_types.h)
	*/
	virtual void calc(const int_t *methodID, const char *methodName, const real_t *inputValues, real_t *outputValues){

		SteamCalculator S;
		Solver2<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY> SS_PH(SOLVER2_DEBUG_PARAM);
		Solver2<Pressure,SpecificEnergy,0,SOLVE_IENERGY> SS_PU(SOLVER2_DEBUG_PARAM);
		Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> SS_UV(SOLVER2_DEBUG_PARAM);
		Solver2<Pressure,SpecificEntropy,0,SOLVE_ENTROPY> SS_PS(SOLVER2_DEBUG_PARAM);
		Solver2<Temperature,SpecificEntropy,0,SOLVE_ENTROPY> SS_TS(SOLVER2_DEBUG_PARAM);
		Solver2<Temperature,SpecificEnergy,0,SOLVE_ENTHALPY> SS_TH(SOLVER2_DEBUG_PARAM);

		int_t method, input, output;
		method = *methodID;
		if(!method)
			method = convertMethod(string(methodName));

		if(method == 0){
			stringstream s;
			s << "Method '" << methodName << " not found";
			throw std::runtime_error(s.str());
		}
		input = method & InputMask;
		output = method & OutputMask;

		#ifdef EMSO_DEBUG
			//cerr << "EMSOfreesteam[" << instanceNumber << "," << numOfCalcCalls << "]: " << methodName << "(";
			cerr << "EMSOfreesteam: " << methodName << "(";
			try{
		#endif

		switch(input){
			case given_waterT:
				S.setSatWater_T(inputValues[0] * Kelvin);
				#ifdef EMSO_DEBUG
					cerr << "sat water T=" << inputValues[0] * Kelvin;
				#endif
				break;

			case given_steamT:
				S.setSatSteam_T(inputValues[0] * Kelvin);
				#ifdef EMSO_DEBUG
					cerr << "sat steam T=" << inputValues[0] * Kelvin;
				#endif
				break;

			case given_pT:
				#ifdef EMSO_DEBUG
					cerr << "p=" << inputValues[0] * MPa << ", T=" << inputValues[1] * Kelvin;
				#endif
				S.set_pT(inputValues[0] * MPa, inputValues[1] * Kelvin);
				break;

			case given_ph:
				#ifdef EMSO_DEBUG
					cerr << "p=" << inputValues[0] * MPa << ", h=" << inputValues[1] * kJ_kg;
				#endif
				S = SS_PH.solve(inputValues[0] * MPa, inputValues[1] * kJ_kg, lastState);
				break;

			case given_ps:
				#ifdef EMSO_DEBUG
					cerr << "p=" << inputValues[0] * MPa << ", s=" << inputValues[1] * kJ_kgK;
				#endif
				S = SS_PS.solve(inputValues[0] * MPa, inputValues[1] * kJ_kgK, lastState);
				break;

			case given_pu:
				#ifdef EMSO_DEBUG
					cerr << "p=" << inputValues[0] * MPa << ", u=" << inputValues[1] * kJ_kg;
				#endif
				///cerr << "About to solve for p,u...";
				S = SS_PU.solve(inputValues[0] * MPa, inputValues[1] * kJ_kg, lastState);
				///cerr << "DONE" << endl;
				break;

			case given_uv:
				#ifdef EMSO_DEBUG
					cerr << "u=" << inputValues[0] * kJ_kg << ", u=" << inputValues[1] * m3_kg;
				#endif
				S = SS_UV.solve(inputValues[0] * kJ_kg, inputValues[1] * m3_kg, lastState);
				break;

			case given_Ts:
				#ifdef EMSO_DEBUG
					cerr << "T=" << inputValues[0] * Kelvin << ", s=" << inputValues[1] * kJ_kgK;
				#endif
				S = SS_TS.solve(inputValues[0] * Kelvin, inputValues[1] * kJ_kgK, lastState);
				break;

			case given_Th:
				#ifdef EMSO_DEBUG
					cerr << "T=" << inputValues[0] * Kelvin << ", h=" << inputValues[1] * kJ_kg;
				#endif
				S = SS_TH.solve(inputValues[0] * Kelvin, inputValues[1] * kJ_kg, lastState);
				break;

			default:
				throw std::runtime_error("EMSOfreesteam::calc: Un-handled input option (may be not yet implemented)");
		}

		#ifdef EMSO_DEBUG
			}catch(std::exception &E){
				cerr << "): error while setting state: " << E.what() << endl;
				throw;
			}
			cerr << "): (region " << S.whichRegion() << ")";

			try{
		#endif

		switch(output){
			// One output
			case T:
				outputValues[0] = S.temp() / Kelvin;
				break;
			case p:
				outputValues[0] = S.pres() / MPa;
				break;
			case v:
				outputValues[0] = S.specvol() / m3_kg;
				#ifdef EMSO_DEBUG
					cerr << " => v=" << S.specvol() << endl;
				#endif
				break;
			case rho:
				outputValues[0] = S.dens() / kg_m3;
				#ifdef EMSO_DEBUG
					cerr << " => rho=" << S.dens() << endl;
				#endif
				break;
			case u:
				outputValues[0] = S.specienergy()/kJ_kg;
				break;
			case s:
				outputValues[0] = S.specentropy() / kJ_kgK;
				break;
			case h:
				outputValues[0] = S.specenthalpy() / kJ_kg;
				break;
			case cp:
				outputValues[0] = S.speccp() / kJ_kgK;
				break;
			case cv:
				outputValues[0] = S.speccv() / kJ_kgK;
				break;
			case k:
				outputValues[0] = S.conductivity() / W_mK;
				break;
			case mu:
				outputValues[0] = S.dynvisc() / Pascal / second;
				#ifdef EMSO_DEBUG
					cerr << " => mu=" << S.dynvisc() << endl;
				#endif
				break;
			case x:
				outputValues[0] = S.quality();
				break;

			// Two outputs
			case Th:
				outputValues[0] = S.temp() / Kelvin;
				outputValues[1] = S.specenthalpy() / kJ_kg;
				#ifdef EMSO_DEBUG
					cerr << " => T=" << S.temp() << ", h=" << S.specenthalpy() << endl;
				#endif
				break;

			case Ts:
				outputValues[0] = S.temp() / Kelvin;
				outputValues[1] = S.specentropy() / kJ_kgK;
				#ifdef EMSO_DEBUG
					cerr << " => T=" << S.temp() << ", s=" << S.specentropy() << endl;
				#endif
				break;

			case hs:
				outputValues[0] = S.specenthalpy() / kJ_kg;
				outputValues[1] = S.specentropy() / kJ_kgK;
				#ifdef EMSO_DEBUG
					cerr << " => h=" << S.specenthalpy() << ", s=" << S.specentropy() << endl;
				#endif
				break;

			case ph:
				outputValues[0] = S.pres() / MPa;
				outputValues[1] = S.specenthalpy() / kJ_kg;
				#ifdef EMSO_DEBUG
					cerr << " => p=" << S.pres() << ", h=" << S.specenthalpy() << endl;
				#endif
				break;

			case ps:
				outputValues[0] = S.pres() / MPa;
				outputValues[1] = S.specentropy() / kJ_kgK;
				#ifdef EMSO_DEBUG
					cerr << " => p=" << S.pres() << ", s=" << S.specentropy() << endl;
				#endif
				break;

			// Three outputs
			case psh:
				outputValues[0] = S.pres() / MPa;
				outputValues[1] = S.specentropy() / kJ_kgK;
				outputValues[2] = S.specenthalpy() / kJ_kg;
				#ifdef EMSO_DEBUG
					cerr << " => p=" << S.pres() << ", s=" << S.specentropy() << ", h=" << S.specenthalpy() << endl;
				#endif
				break;

			case psT:
				outputValues[0] = S.pres() / MPa;
				outputValues[1] = S.specentropy() / kJ_kgK;
				outputValues[2] = S.temp() / Kelvin;

				#ifdef EMSO_DEBUG
					cerr << " => p=" << S.pres() << ", s=" << S.specentropy() << ", h=" << S.specenthalpy() << ", T=" << S.temp() << endl;
				#endif
				break;


			// Four outputs
			case Tsvx:
				outputValues[0] = S.temp() / Kelvin;
				outputValues[1] = S.specentropy() / kJ_kgK;
				outputValues[2] = S.specvol() / m3_kg;
				outputValues[3] = S.quality();
				#ifdef EMSO_DEBUG
					cerr << " => T=" << S.temp() << ", s=" << S.specentropy() << ", v=" << S.specvol() << ", x=" << S.quality() << endl;
				#endif
				break;

			case Tuvx:
				outputValues[0] = S.temp() / Kelvin;
				outputValues[1] = S.specienergy() / kJ_kg;
				outputValues[2] = S.specvol() / m3_kg;
				outputValues[3] = S.quality();
				#ifdef EMSO_DEBUG
					cerr << " => T=" << S.temp() << ", u=" << S.specienergy() << ", v=" << S.specvol() << ", x=" << S.quality() << endl;
				#endif
				break;

			case Thvx:
				outputValues[0] = S.temp() / Kelvin;
				outputValues[1] = S.specenthalpy() / kJ_kg;
				outputValues[2] = S.specvol() / m3_kg;
				outputValues[3] = S.quality();
				#ifdef EMSO_DEBUG
					cerr << " => T=" << S.temp() << ", h=" << S.specenthalpy() << ", v=" << S.specvol() << ", x=" << S.quality() << endl;
				#endif
				break;

			case pshT:
				outputValues[0] = S.pres() / MPa;
				outputValues[1] = S.specentropy() / kJ_kgK;
				outputValues[2] = S.specenthalpy() / kJ_kg;
				outputValues[3] = S.temp() / Kelvin;

				#ifdef EMSO_DEBUG
					cerr << " => p=" << S.pres() << ", s=" << S.specentropy() << ", h=" << S.specenthalpy() << ", T=" << S.temp() << endl;
				#endif
				break;

			// Five outputs
			case psvhx:
				outputValues[0] = S.pres() / MPa;
				outputValues[1] = S.specentropy() / kJ_kgK;
				outputValues[2] = S.specvol() / m3_kg;
				outputValues[3] = S.specenthalpy() / kJ_kg;
				outputValues[4] = S.quality();

				#ifdef EMSO_DEBUG
					cerr << " => p=" << S.pres() << ", s=" << S.specentropy() << ", v=" << S.specvol() << ", h=" << S.specentropy()<< ", x=" << S.quality() << endl;
				#endif
				break;

			default:
				throw std::runtime_error("Invalid output option (should never happen)");
		}

		#ifdef EMSO_DEBUG
			}catch(std::exception &E){
				cerr << "): error while setting evaluating properties: " << E.what() << endl;
				throw;
			}
			cerr << " OK" << endl;

		#endif
	}

private:

	/// A memory of the last-calculated steam properties, helps with initial guesses.
	SteamCalculator lastState;

	/// The current number of created EMSOfreesteam objects.
	static int_t numOfPackages;

	#ifdef EMSO_DEBUG
		bool debug;
		//int_t numOfCalcCalls;
		//int_t instanceNumber;
		//static int_t instanceSerialNumber;
		//static int_t numOfInstances;
	#endif

	/// Specific property options
	/**
		These must never give nonzero when ANDed with output_arguments or each other
	*/
	enum input_arguments {
		given_pT = 1
		, given_ph
		, given_pu
		, given_uv
		, given_ps
		, given_Ts
		, given_Th
		, given_waterT
		, given_steamT

		, InputMask = 0x00000fff
	};

	/// Returned property options
	/**
		These must never give nonzero when ANDed with input_arguments or each other
	*/
	enum output_arguments{

		T		= 0x00010000
		,p      = 0x00020000
		,v      = 0x00030000
		,u      = 0x00040000
		,s      = 0x00050000
		,h      = 0x00060000
		,cp     = 0x00070000
		,cv     = 0x00080000
		,rho    = 0x00090000
		,x      = 0x000a0000
		,k      = 0x00100000
		,mu     = 0x00200000
		,Tsvx   = 0x01000000
		,Tuvx   = 0x02000000
		,psvx   = 0x03000000
		,psvhx  = 0x04000000
		,Thvx   = 0x05000000
		,Th     = 0x06000000
		,Ts     = 0x07000000
		,hs     = 0x08000000
		,psh	= 0x09000000
		,ph     = 0x0A000000
		,ps     = 0x0B000000
		,pshT   = 0x0C000000
		,psT    = 0x0D000000
		,region = 0x10000000

		,OutputMask = 0xffff0000
	};

public:

	/*
		--------------------------
		See the comment at the top of the file
		for an explanation of this crazy stuff here
	*/
#define M(OUT,IN) OUT ## _ ## IN = OUT | given_ ## IN
#define X ,

	/// The methods
	typedef enum {
		METHOD_LIST
	} Method;

#undef M
#undef X

	typedef map<const string, int_t> MethodMap;
	static MethodMap methodNames;

// Assign each method to the map:
#define M(OUT,IN) methodNames[#OUT "_" #IN] = OUT ## _ ## IN
#define X ;

	void initialiseMethodNames(){
		if(!methodNames.size()){
			METHOD_LIST;
		}
	}

#undef M
#undef X

	void initialiseParamNames(){
		// nothing here
	}

	/*
		End of crazy stuff
		----------------------------
	*/

	string getValidMethodNames(){

		stringstream ss;

		MethodMap::iterator i;

		bool first=true;
		int perthisline=-1;
		for(i = methodNames.begin(); i!=methodNames.end(); i++){
			if(!first){
				ss << ", ";
			}else{
				first=false;
			}

			if(!(++perthisline %= 10)){
				ss << endl;
			}
			ss << (*i).first;

		}

		ss << endl;

		return ss.str();
	}

	/**
		Convert a string method name to one of the enumerators #method_names
		If the given name is not a valid method #unknownMethod is returned.
	*/
	int_t convertMethod(const string &name){

		MethodMap::iterator i;

		i = methodNames.find(name);

		if(i != methodNames.end()){
			return (*i).second;
		}

		return 0;
	}

	/** check if we have an error and print it to the given msg
	*
	*/
	void report_error(char *msg, const string &error){
		if(error.size() > EMSO_MESSAGE_LENGTH){
			stringstream ss;
			ss << error.substr(0,EMSO_MESSAGE_LENGTH-24) << "... (message cut short)";
			snprintf(msg,EMSO_MESSAGE_LENGTH,ss.str().c_str());
		}else{
			snprintf(msg, EMSO_MESSAGE_LENGTH, error.c_str());
		}
	}

};

EMSOfreesteam::MethodMap EMSOfreesteam::methodNames=EMSOfreesteam::MethodMap();

EXPORT_EXTERNAL_OBJECT(EMSOfreesteam)