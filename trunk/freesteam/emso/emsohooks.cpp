#include "../steamcalculator.h"
#include "../solver2.h"

#ifndef HAVE_EMSO
	#error EMSO NOT CONFIGURED ON YOUR SYSTEM, CHECK CPPFLAGS IN THE FILE PACKAGE_ROOT/make-xxxxx.mak FOR YOUR SYSTEM.
#endif

#ifdef EMSO_DEBUG
	#warning EMSO DEBUG MODE IS SET - START EMSO FROM A CONSOLE TO SEE DEBUG OUTPUT
#endif

#include "external_object.h"

#include <sstream>
#include <cstring>
#include <map>
using namespace std;

#define EF_ASSIGN(out,in) ( m[#out "_" #in]=out ## _ ## in )
#define EF_DECLARE(out,in) out ## _ ## in = out | given_ ## in

/** Data structure for an EMSO 'WaterSteam' package instance.
* This object contains instance-specific data.
*/
class EMSOfreesteam : public ExternalObjectBase {

	public:

		EMSOfreesteam(){
			#ifdef EMSO_DEBUG
				numOfCalcCalls = 0;
				numOfInstances++;
				instanceNumber = instanceSerialNumber++;

				cerr << "Created EMSOfreesteam object " << instanceNumber << " (now " << numOfInstances << " instances)..." << endl;
			#endif

			cerr.flags(ios_base::showbase);
		};

		virtual ~EMSOfreesteam(){
			#ifdef EMSO_DEBUG
				numOfInstances--;

				cerr << "Deleting EMSOfreesteam object " << instanceNumber << " (leaves " << numOfInstances << " instances)..." << endl;
			#endif
		};

		/**
			Create object. Anything risking should go here and throw an exception if necessary
		*/
		virtual int_t create(char *msg){
			try{

				#ifdef EMSO_DEBUG
					cerr << "EMSOfreesteam::create(): instance " << instanceNumber << endl;
				#endif

				return emso_ok;

			}catch(Exception *E){
				stringstream ss;
				ss << "EMSOfreesteam::create: " << E->what();
				delete E;
				report_error(msg, ss.str());
				return emso_error;
			}
		}

		/**
			Destroyes an instance of this object in EMSO
		*/
		virtual int_t destroy(char *msg){
			try{

				#ifdef EMSO_DEBUG
					cerr << "EMSOfreesteam::destroy(): instance " << instanceNumber << endl;
				#endif

				return emso_ok;

			}catch(Exception *E){
				stringstream ss;
				ss << "EMSOfreesteam::destroy: " << E->what();
				delete E;
				report_error(msg, ss.str());
				return emso_error;
			}
		}

		/// Set a parameter for the instance.
		virtual int_t setParameter(const char *parameterName,
				const int_t *valueType,
				const int_t *valueLength,
				const real_t *values,
				const char *valuesText[],
				char *msg
		){
			snprintf(msg, EMSO_MESSAGE_LENGTH, "EMSOfreesteam has no parameters");
			return emso_error;
		}

		/// Check method.
		virtual int_t checkMethod(const char *methodName,
				int_t *methodID,
				int_t *numOfInputs,
				int_t *numOfOutputs,
				char *msg
		){

			try{
				int_t method, input, output;

				#ifdef EMSO_DEBUG
					cerr << "Searching for DLL method '" << methodName << "'..." << endl;
				#endif

				*methodID = method = convertMethod(string(methodName));

				if(method == 0){
					stringstream s;
					s << "Method '" << methodName << "' not found";
					throw new Exception(s.str());
				}

				input = method & InputMask;
				output = (method & OutputMask);

				// The number of input parameters:
				switch(input){

					case given_steamT:
					case given_waterT:
						*numOfInputs = 1;
						break;

					case given_pTx:
						*numOfInputs = 3;
						break;

					case given_ph:
					case given_pu:
					case given_uv:
					case given_ps:
						*numOfInputs = 2;
						break;

					default:
						throw new Exception("Invalid input parameter combo");
				}

				// Number of output parameters:
				switch(output){
					case Tsvx:
					case Tuvx:
					case Thvx:
					case psvx:
						*numOfOutputs = 4;
						break;

					case psvhx:
						*numOfOutputs = 5;
						break;

					default:
						*numOfOutputs = 1;
						break;
				}

				return emso_ok;

			}catch(Exception *E){
				stringstream s;
				s << "EMSOfreesteam::checkMethod: " << E->what();
				delete E;
				report_error(msg, s.str());
				return emso_error;
			}
		}

		/// Method details
		/**
			Relate information about the available methods in this component back to EMSO
		*/
		virtual int_t methodDetails(const char *methodName,
				const int_t *methodID,

				const int_t *numOfInputs,
				int_t *inputLengths,
				int_t *inputTypes,
				char *inputUnits[],

				const int_t *numOfOutputs,
				int_t *outputLengths,
				int_t *outputTypes,
				char *outputUnits[],
				int_t *derivativeMatrix,

				char *msg
		){

			try{
				int_t method, input, output;

				method = *methodID;
				if(!method)
					method = convertMethod(string(methodName));

				if(method == 0){
					stringstream s;
					s << "Method '" << methodName << "' not found.";
					throw new Exception(s.str());
				}

				input = method & InputMask;
				output = method & OutputMask;

				//all of our inputs and outputs are of length 1, hence we do not
				//need to set inputLengths or outputLengths

				// the method inputs
				switch(input){
					case given_pTx:
						strcpy(inputUnits[0], "MPa");
						strcpy(inputUnits[1], "K");
						strcpy(inputUnits[2], "");

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

					case given_waterT:
					case given_steamT:
						strcpy(inputUnits[0], "K");
						break;

					default:
						throw new Exception("Invalid input param combination");
				};

				// the outputs
				switch(output){

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
						strcpy(outputUnits[0], "Pa*sec");
						break;
					case v:
						strcpy(outputUnits[0], "m^3/kg");
						break;

					case rho:
						strcpy(outputUnits[0], "kg/m^3");
						break;

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

					case psvhx:
						strcpy(outputUnits[0], "MPa");
						strcpy(outputUnits[1], "kJ/kg/K");
						strcpy(outputUnits[2], "m^3/kg");
						strcpy(outputUnits[3], "kJ/kg");
						break;

					case region:
						break;

					default:
						throw new Exception("Invalid output param combination");
				}

				return emso_ok;

			}catch(Exception *E){
				stringstream s;
				s << "EMSOfreesteam::calc: " << E->what();
				delete E;
				report_error(msg, s.str());
				return emso_error;
			}
		}

		/**
			Calculation

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
		virtual int_t calc(
				const char *methodName,
				const int_t *methodID,

				const int_t *numOfInputs,
				const int_t *inputLengths,
				const int_t *totalInputLength,
				const real_t *inputValues,

				const int_t *numOfOutputs,
				const int_t *outputLengths,
				const int_t *totalOutputLength,
				real_t *outputValues,

				const int_t *calculateDerivatives,
				real_t *outputDerivatives,

				char *msg
		){
			SteamCalculator S;
			Solver2<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY> SS_PH;
			Solver2<Pressure,SpecificEnergy,0,SOLVE_IENERGY> SS_PU;
			Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> SS_UV;
			Solver2<Pressure,SpecificEntropy,0,SOLVE_ENTROPY> SS_PS;

			try{

				#ifdef EMSO_DEBUG
					++numOfCalcCalls;
				#endif

				int_t method, input, output;
				method = *methodID;
				if(!method)
					method = convertMethod(string(methodName));

				if(method == 0){
					stringstream s;
					s << "Method '" << methodName << " not found";
					throw new Exception(s.str());
				}
				input = method & InputMask;
				output = method & OutputMask;

				#ifdef EMSO_DEBUG
					cerr << "emsofreesteam[" << instanceNumber << "," << numOfCalcCalls << "]: ";
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

					case given_pTx:
						S.set_pT(inputValues[0] * MPa, inputValues[1] * Kelvin, inputValues[2]);
						#ifdef EMSO_DEBUG
							cerr << "p=" << inputValues[0] * MPa << ", T=" << inputValues[1] * Kelvin << ", x=" << inputValues[2];
						#endif
						break;

					case given_ph:
						S = SS_PH.solve(inputValues[0] * MPa, inputValues[1] * kJ_kg);
						#ifdef EMSO_DEBUG
							cerr << "p=" << inputValues[0] * MPa << ", h=" << inputValues[1] * kJ_kg;
						#endif
						break;

					case given_ps:
						S = SS_PS.solve(inputValues[0] * MPa, inputValues[1] * kJ_kgK);

						#ifdef EMSO_DEBUG
							cerr << "p=" << inputValues[0] * MPa << ", s=" << inputValues[1] * kJ_kgK;
						#endif
						break;

					case given_pu:
						S = SS_PU.solve(inputValues[0] * MPa, inputValues[1] * kJ_kg);
						#ifdef EMSO_DEBUG
							cerr << "p=" << inputValues[0] * MPa << ", u=" << inputValues[1] * kJ_kg;
						#endif
						break;

					case given_uv:
						S = SS_UV.solve(inputValues[0] * kJ_kg, inputValues[1] * m3_kg);
						#ifdef EMSO_DEBUG
							cerr << "u=" << inputValues[0] * kJ_kg << ", u=" << inputValues[1] * m3_kg;
						#endif
						break;

					default:
						throw new Exception("EMSOfreesteam::calc: Un-handled input option (may be not yet implemented)");
				}

				#ifdef EMSO_DEBUG
					cerr << " (region " << S.whichRegion() << ")";
				#endif

				switch(output){
					case T:
						outputValues[0] = S.temp() / Kelvin;
						break;
					case p:
						outputValues[0] = S.pres() / MPa;
						break;
					case v:
						outputValues[0] = S.specvol() / m3_kg;
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
						break;
					case x:
						outputValues[0] = S.quality();
						break;
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
						throw new Exception("Invalid output option (should never happen)");
				}

				return emso_ok;

			}catch(Exception *E){
				stringstream s;
				s << "EMSOfreesteam::calc: " << E->what();
				delete E;
				report_error(msg, s.str());
				return emso_error;
			}
		}

	private:

		/// The current number of created EMSOfreesteam objects.
		static int_t numOfPackages;

		#ifdef EMSO_DEBUG
			int_t numOfCalcCalls;
			int_t instanceNumber;
			static int_t instanceSerialNumber;
			static int_t numOfInstances;
		#endif

		/// Specific property options
		/**
			These must never give nonzero when ANDed with output_arguments or each other
		*/
		enum input_arguments {
			given_pTx = 1
			, given_ph
			, given_pu
			, given_uv
			, given_ps
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
			,region = 0x10000000

			,OutputMask = 0xffff0000
		};

		/// The methods
		enum method_name {

			EF_DECLARE(p,waterT)
			, EF_DECLARE(x,waterT)
			, EF_DECLARE(v,waterT)
			, EF_DECLARE(u,waterT)
			, EF_DECLARE(s,waterT)
			, EF_DECLARE(h,waterT)
			, EF_DECLARE(cp,waterT)
			, EF_DECLARE(cv,waterT)
			, EF_DECLARE(k,waterT)
			, EF_DECLARE(mu,waterT)
			, EF_DECLARE(rho,waterT)
			, EF_DECLARE(region,waterT)
			, EF_DECLARE(psvhx,waterT)
			, EF_DECLARE(p,steamT)
			, EF_DECLARE(x,steamT)
			, EF_DECLARE(v,steamT)
			, EF_DECLARE(u,steamT)
			, EF_DECLARE(s,steamT)
			, EF_DECLARE(h,steamT)
			, EF_DECLARE(cp,steamT)
			, EF_DECLARE(cv,steamT)
			, EF_DECLARE(k,steamT)
			, EF_DECLARE(mu,steamT)
			, EF_DECLARE(rho,steamT)
			, EF_DECLARE(region,steamT)
			, EF_DECLARE(psvhx,steamT)
			, EF_DECLARE(T,pTx)
			, EF_DECLARE(p,pTx)
			, EF_DECLARE(x,pTx)
			, EF_DECLARE(v,pTx)
			, EF_DECLARE(u,pTx)
			, EF_DECLARE(s,pTx)
			, EF_DECLARE(h,pTx)
			, EF_DECLARE(cp,pTx)
			, EF_DECLARE(cv,pTx)
			, EF_DECLARE(k,pTx)
			, EF_DECLARE(mu,pTx)
			, EF_DECLARE(rho,pTx)
			, EF_DECLARE(region,pTx)
			, EF_DECLARE(T,ph)
			, EF_DECLARE(x,ph)
			, EF_DECLARE(v,ph)
			, EF_DECLARE(u,ph)
			, EF_DECLARE(s,ph)
			, EF_DECLARE(cp,ph)
			, EF_DECLARE(cv,ph)
			, EF_DECLARE(k,ph)
			, EF_DECLARE(mu,ph)
			, EF_DECLARE(rho,ph)
			, EF_DECLARE(region,ph)
			, EF_DECLARE(T,ps)
			, EF_DECLARE(x,ps)
			, EF_DECLARE(v,ps)
			, EF_DECLARE(u,ps)
			, EF_DECLARE(h,ps)
			, EF_DECLARE(cp,ps)
			, EF_DECLARE(cv,ps)
			, EF_DECLARE(k,ps)
			, EF_DECLARE(mu,ps)
			, EF_DECLARE(rho,ps)
			, EF_DECLARE(region,ps)
			, EF_DECLARE(T,pu)
			, EF_DECLARE(x,pu)
			, EF_DECLARE(v,pu)
			, EF_DECLARE(s,pu)
			, EF_DECLARE(h,pu)
			, EF_DECLARE(cp,pu)
			, EF_DECLARE(cv,pu)
			, EF_DECLARE(k,pu)
			, EF_DECLARE(mu,pu)
			, EF_DECLARE(rho,pu)
			, EF_DECLARE(region,pu)
			, EF_DECLARE(T,uv)
			, EF_DECLARE(p,uv)
			, EF_DECLARE(x,uv)
			, EF_DECLARE(s,uv)
			, EF_DECLARE(h,uv)
			, EF_DECLARE(cp,uv)
			, EF_DECLARE(cv,uv)
			, EF_DECLARE(k,uv)
			, EF_DECLARE(mu,uv)
			, EF_DECLARE(rho,uv)
			, EF_DECLARE(region,uv)
			, EF_DECLARE(Tsvx,ph)
			, EF_DECLARE(Tsvx,pu)
			, EF_DECLARE(Tuvx,ps)
			, EF_DECLARE(Thvx,ps)
			, EF_DECLARE(Tuvx,ph)
		};

		/**
			Convert a string method name to one of the enumerators #method_names
			If the given name is not a valid method #unknownMethod is returned.
		*/
		int_t convertMethod(const string &name){

			map<const string,enum method_name> m;
			map<const string,enum method_name>::iterator i;

			// DON'T EDIT THESE, EDIT method_name AND CREATE THESE USING SOME FIND/REPLACE:
			EF_ASSIGN(p,waterT);
			EF_ASSIGN(x,waterT);
			EF_ASSIGN(v,waterT);
			EF_ASSIGN(u,waterT);
			EF_ASSIGN(s,waterT);
			EF_ASSIGN(h,waterT);
			EF_ASSIGN(cp,waterT);
			EF_ASSIGN(cv,waterT);
			EF_ASSIGN(k,waterT);
			EF_ASSIGN(mu,waterT);
			EF_ASSIGN(rho,waterT);
			EF_ASSIGN(region,waterT);
			EF_ASSIGN(psvhx,waterT);
			EF_ASSIGN(p,steamT);
			EF_ASSIGN(x,steamT);
			EF_ASSIGN(v,steamT);
			EF_ASSIGN(u,steamT);
			EF_ASSIGN(s,steamT);
			EF_ASSIGN(h,steamT);
			EF_ASSIGN(cp,steamT);
			EF_ASSIGN(cv,steamT);
			EF_ASSIGN(k,steamT);
			EF_ASSIGN(mu,steamT);
			EF_ASSIGN(rho,steamT);
			EF_ASSIGN(region,steamT);
			EF_ASSIGN(psvhx,steamT);
			EF_ASSIGN(T,pTx);
			EF_ASSIGN(p,pTx);
			EF_ASSIGN(x,pTx);
			EF_ASSIGN(v,pTx);
			EF_ASSIGN(u,pTx);
			EF_ASSIGN(s,pTx);
			EF_ASSIGN(h,pTx);
			EF_ASSIGN(cp,pTx);
			EF_ASSIGN(cv,pTx);
			EF_ASSIGN(k,pTx);
			EF_ASSIGN(mu,pTx);
			EF_ASSIGN(rho,pTx);
			EF_ASSIGN(region,pTx);
			EF_ASSIGN(T,ph);
			EF_ASSIGN(x,ph);
			EF_ASSIGN(v,ph);
			EF_ASSIGN(u,ph);
			EF_ASSIGN(s,ph);
			EF_ASSIGN(cp,ph);
			EF_ASSIGN(cv,ph);
			EF_ASSIGN(k,ph);
			EF_ASSIGN(mu,ph);
			EF_ASSIGN(rho,ph);
			EF_ASSIGN(region,ph);
			EF_ASSIGN(T,ps);
			EF_ASSIGN(x,ps);
			EF_ASSIGN(v,ps);
			EF_ASSIGN(u,ps);
			EF_ASSIGN(h,ps);
			EF_ASSIGN(cp,ps);
			EF_ASSIGN(cv,ps);
			EF_ASSIGN(k,ps);
			EF_ASSIGN(mu,ps);
			EF_ASSIGN(rho,ps);
			EF_ASSIGN(region,ps);
			EF_ASSIGN(T,pu);
			EF_ASSIGN(x,pu);
			EF_ASSIGN(v,pu);
			EF_ASSIGN(s,pu);
			EF_ASSIGN(h,pu);
			EF_ASSIGN(cp,pu);
			EF_ASSIGN(cv,pu);
			EF_ASSIGN(k,pu);
			EF_ASSIGN(mu,pu);
			EF_ASSIGN(rho,pu);
			EF_ASSIGN(region,pu);
			EF_ASSIGN(T,uv);
			EF_ASSIGN(p,uv);
			EF_ASSIGN(x,uv);
			EF_ASSIGN(s,uv);
			EF_ASSIGN(h,uv);
			EF_ASSIGN(cp,uv);
			EF_ASSIGN(cv,uv);
			EF_ASSIGN(k,uv);
			EF_ASSIGN(mu,uv);
			EF_ASSIGN(rho,uv);
			EF_ASSIGN(region,uv);
			EF_ASSIGN(Tsvx,ph);
			EF_ASSIGN(Tsvx,pu);
			EF_ASSIGN(Tuvx,ps);
			EF_ASSIGN(Thvx,ps);
			EF_ASSIGN(Tuvx,ph);
			// DON'T EDIT THOSE, EDIT method_name AND CREATE THESE USING SOME FIND/REPLACE:

			i = m.find(name);

			if(i != m.end()){
				return (*i).second;
			}

			return 0;
		}

		/** check if we have an error and print it to the given msg
		*
		*/
		void report_error(char *msg, const string &error){
			snprintf(msg, EMSO_MESSAGE_LENGTH, error.c_str());
		}

};

#ifdef EMSO_DEBUG
	int_t EMSOfreesteam::numOfInstances = 0;
	int_t EMSOfreesteam::instanceSerialNumber = 0;
#endif

//* The factory function
ExternalObjectBase* ExternalObjectFactory(){
	return new EMSOfreesteam();
}


/*******************************************************************************
*
* Herein after follows the C interface wrapping the C++ implementation,
* the user should NOT modify these functions.
*/

// Keep this function as is.
void eo_create_ (int_t *objectHandler,
				 int_t *retval, char *msg){
	// Create the objecty instance using the factory.
	ExternalObjectBase *obj = ExternalObjectFactory();

	// Call the correspounding c++ function.
	*retval = obj->create(msg);

	// Return our object address.
	*objectHandler = (int_t)obj;
}



// Keep this function as is.
void eo_destroy_ (const int_t *objectHandler,
                  int_t *retval, char *msg){
	// Cast back the object.
	ExternalObjectBase *obj = (ExternalObjectBase*) *objectHandler;

	// Call the correspounding c++ function.
	*retval = obj->destroy(msg);
	// Delete the object instance.
	delete obj;
}

// Keep this function as is.
void eo_set_parameter_ (const int_t *objectHandler,
						const char *parameterName,
						const int_t *valueType,
						const int_t *valueLength,
						const real_t *values,
						const char *valuesText[],
						int_t *retval, char *msg){
	// Cast back the object.
	ExternalObjectBase *obj = (ExternalObjectBase*) *objectHandler;

	// Call the correspounding c++ function.
	*retval = obj->setParameter(parameterName, valueType, valueLength,
		values, valuesText, msg);
}

// Keep this function as is.
void eo_check_method_(const int_t *objectHandler,
					  const char *methodName,
					  int_t *methodID,
					  int_t *numOfInputs,
					  int_t *numOfOutputs,
					  int_t *retval, char *msg
){
	// Cast back the object.
	ExternalObjectBase *obj = (ExternalObjectBase *)*objectHandler;

	// Call the correspounding c++ function.
	*retval = obj->checkMethod(methodName, methodID,
		numOfInputs, numOfOutputs, msg);
}

// Keep this function as is.
void eo_method_details_(const int_t *objectHandler,
						const char *methodName,
						const int_t *methodID,

						const int_t *numOfInputs,
						int_t *inputLengths,
						int_t *inputTypes,
						char *inputUnits[],

						const int_t *numOfOutputs,
						int_t *outputLengths,
						int_t *outputTypes,
						char *outputUnits[],
						int_t *derivativeMatrix,

						int_t *retval, char *msg
){

	// Cast back the object.
	ExternalObjectBase *obj = (ExternalObjectBase *)*objectHandler;

	// Call the correspounding c++ function.
	*retval = obj->methodDetails(methodName, methodID,
		numOfInputs, inputLengths, inputTypes, inputUnits,
		numOfOutputs, outputLengths, outputTypes, outputUnits,
		derivativeMatrix, msg);
}

// Keep this function as is.
void eo_calc_(const int_t *objectHandler,
			  const char *methodName,
			  const int_t *methodID,

			  const int_t *numOfInputs,
			  const int_t *inputLengths,
			  const int_t *totalInputLength,
			  const real_t *inputValues,

			  const int_t *numOfOutputs,
			  const int_t *outputLengths,
			  const int_t *totalOutputLength,
			  real_t *outputValues,

			  const int_t *calculateDerivatives,
			  real_t *outputDerivatives,

			  int_t *retval, char *msg
){
    // Cast back the object.
	ExternalObjectBase *obj = (ExternalObjectBase *)*objectHandler;

	// Call the correspounding c++ function.
	*retval = obj->calc(methodName, methodID,
		numOfInputs, inputLengths, totalInputLength, inputValues,
		numOfOutputs, outputLengths, totalOutputLength, outputValues,
		calculateDerivatives, outputDerivatives, msg);
}



