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
	#define SOLVER2_DEBUG_PARAM debug
#else
	#define SOLVER2_DEBUG_PARAM
#endif

#include <emso/external_object.h>

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
				debug=false;

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
		virtual void create(int_t *retval, char *msg){
			try{

				#ifdef EMSO_DEBUG
					cerr << "EMSOfreesteam::create(): instance " << instanceNumber << endl;
				#endif

				*retval = emso_ok;

			}catch(Exception *E){
				stringstream ss;
				ss << "EMSOfreesteam::create: " << E->what();
				delete E;
				report_error(msg, ss.str());
				*retval = emso_error;
			}
		}

		/**
			Destroyes an instance of this object in EMSO
		*/
		virtual void destroy(int_t *retval, char *msg){
			try{

				#ifdef EMSO_DEBUG
					cerr << "EMSOfreesteam::destroy(): instance " << instanceNumber << endl;
				#endif

				*retval = emso_ok;

			}catch(Exception *E){
				stringstream ss;
				ss << "EMSOfreesteam::destroy: " << E->what();
				delete E;
				report_error(msg, ss.str());
				*retval = emso_error;
			}
		}

		/// Set a parameter for the instance.
		virtual void setParameter(
				const char *parameterName
				,const int_t *valueType
				,const int_t *valueLength
				,const real_t *values
				,const char *valuesText[]
				,int_t *retval, char *msg
		){
			#ifdef EMSO_DEBUG
				snprintf(msg, EMSO_MESSAGE_LENGTH, "EMSOfreesteam parameters have not yet been implemented");
				*retval = emso_error;
			#else
				snprintf(msg, EMSO_MESSAGE_LENGTH, "This EMSOfreesteam DLL was compiled without debug support, so you won't see any debug output.");
				*retval = emso_warning;
			#endif
		}

		/// Check method.
		virtual void checkMethod(
				const char *methodName
				,int_t *methodID
				,int_t *numOfInputs
				,int_t *numOfOutputs
				,int_t *retval, char *msg
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
				output = method & OutputMask;

				// The number of input parameters:
				switch(input){

					case given_steamT:
					case given_waterT:
						*numOfInputs = 1;
						break;

					case given_pT:
						*numOfInputs = 2;
						break;

					case given_ph:
					case given_pu:
					case given_uv:
					case given_ps:
					case given_Ts:
					case given_Th:
						*numOfInputs = 2;
						break;

					default:
						throw new Exception("Invalid input parameter combo");
				}

				// Number of output parameters:
				switch(output){
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
						*numOfOutputs = 1;
						break;

					case Th:
					case Ts:
					case hs:
					case ph:
					case ps:
						*numOfOutputs = 2;
						break;

					case Tsvx:
					case Tuvx:
					case Thvx:
					case psvx:
						*numOfOutputs = 4;
						break;

					case psh:
						*numOfOutputs = 3;
						break;

					case psvhx:
						*numOfOutputs = 5;
						break;


					default:
						throw new Exception("Invalid output parameter combo");
				}

				*retval = emso_ok;

			}catch(Exception *E){
				stringstream s;
				s << "EMSOfreesteam::checkMethod: " << E->what();
				delete E;
				report_error(msg, s.str());
				*retval = emso_error;
			}
		}

		/// Method details
		/**
			Relate information about the available methods in this component back to EMSO
		*/
		virtual void methodDetails(
				const char *methodName
				,const int_t *methodID

				,const int_t *numOfInputs
				,int_t *inputLengths
				,int_t *inputTypes
				,char *inputUnits[]

				,const int_t *numOfOutputs
				,int_t *outputLengths
				,int_t *outputTypes
				,char *outputUnits[]
				,int_t *derivativeMatrix

				,int_t *retval, char *msg
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
						throw new Exception("Invalid input param combination (when giving EMSO units for method inputs)");
				};

				// the outputs
				switch(output){

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
						strcpy(outputUnits[0], "Pa*sec");
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
						throw new Exception("Invalid output param combination (when giving EMSO units for method outputs)");
				}

				*retval = emso_ok;

			}catch(Exception *E){
				stringstream s;
				s << "EMSOfreesteam::calc: " << E->what();
				delete E;
				report_error(msg, s.str());
				*retval = emso_error;
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
		virtual void calc(
				const char *methodName
				,const int_t *methodID

				,const int_t *numOfInputs
				,const int_t *inputLengths
				,const int_t *totalInputLength
				,const real_t *inputValues

				,const int_t *numOfOutputs
				,const int_t *outputLengths
				,const int_t *totalOutputLength
				,real_t *outputValues

				,const int_t *calculateDerivatives
				,real_t *outputDerivatives

				,int_t *retval, char *msg
		){
			SteamCalculator S;
			Solver2<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY> SS_PH(SOLVER2_DEBUG_PARAM);
			Solver2<Pressure,SpecificEnergy,0,SOLVE_IENERGY> SS_PU(SOLVER2_DEBUG_PARAM);
			Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> SS_UV(SOLVER2_DEBUG_PARAM);
			Solver2<Pressure,SpecificEntropy,0,SOLVE_ENTROPY> SS_PS(SOLVER2_DEBUG_PARAM);
			Solver2<Temperature,SpecificEntropy,0,SOLVE_ENTROPY> SS_TS(SOLVER2_DEBUG_PARAM);
			Solver2<Temperature,SpecificEnergy,0,SOLVE_ENTHALPY> SS_TH(SOLVER2_DEBUG_PARAM);

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
					cerr << "EMSOfreesteam[" << instanceNumber << "," << numOfCalcCalls << "]: " << methodName << "(";
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
						S = SS_PU.solve(inputValues[0] * MPa, inputValues[1] * kJ_kg, lastState);
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
						throw new Exception("EMSOfreesteam::calc: Un-handled input option (may be not yet implemented)");
				}

				#ifdef EMSO_DEBUG
					}catch(Exception *E){
						cerr << "): error while setting state: " << E->what() << endl;
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
						throw new Exception("Invalid output option (should never happen)");
				}

				#ifdef EMSO_DEBUG
					}catch(Exception *E){
						cerr << ": error while getting property values: " << E->what() << endl;
						throw;
					}
				#endif
				*retval = emso_ok;
				lastState = S;

			}catch(Exception *E){
				stringstream s;
				s << "EMSOfreesteam::calc: " << E->what();
				delete E;

				report_error(msg, s.str());
				*retval = emso_error;
			}
		}

	private:

		SteamCalculator lastState;

		/// The current number of created EMSOfreesteam objects.
		static int_t numOfPackages;

		#ifdef EMSO_DEBUG
			bool debug;
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
			, EF_DECLARE(psh,waterT)
			, EF_DECLARE(ps,waterT)
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
			, EF_DECLARE(psh,steamT)
			, EF_DECLARE(ps,steamT)
			, EF_DECLARE(x,pT)
			, EF_DECLARE(v,pT)
			, EF_DECLARE(u,pT)
			, EF_DECLARE(s,pT)
			, EF_DECLARE(h,pT)
			, EF_DECLARE(cp,pT)
			, EF_DECLARE(cv,pT)
			, EF_DECLARE(k,pT)
			, EF_DECLARE(mu,pT)
			, EF_DECLARE(rho,pT)
			, EF_DECLARE(region,pT)
			, EF_DECLARE(hs,pT)
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
			, EF_DECLARE(Ts,ph)
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
			, EF_DECLARE(Th,ps)
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
			, EF_DECLARE(Th,pu)
			, EF_DECLARE(Ts,pu)
			, EF_DECLARE(hs,pu)
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
			, EF_DECLARE(Th,uv)
			, EF_DECLARE(Ts,uv)
			, EF_DECLARE(hs,uv)
			, EF_DECLARE(psh,uv)
			, EF_DECLARE(ps,uv)
			, EF_DECLARE(p,Ts)
			, EF_DECLARE(x,Ts)
			, EF_DECLARE(v,Ts)
			, EF_DECLARE(u,Ts)
			, EF_DECLARE(h,Ts)
			, EF_DECLARE(cp,Ts)
			, EF_DECLARE(cv,Ts)
			, EF_DECLARE(k,Ts)
			, EF_DECLARE(mu,Ts)
			, EF_DECLARE(rho,Ts)
			, EF_DECLARE(region,Ts)
			, EF_DECLARE(ph,Ts)
			, EF_DECLARE(p,Th)
			, EF_DECLARE(x,Th)
			, EF_DECLARE(v,Th)
			, EF_DECLARE(u,Th)
			, EF_DECLARE(s,Th)
			, EF_DECLARE(cp,Th)
			, EF_DECLARE(cv,Th)
			, EF_DECLARE(k,Th)
			, EF_DECLARE(mu,Th)
			, EF_DECLARE(rho,Th)
			, EF_DECLARE(region,Th)
			, EF_DECLARE(ps,Th)
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
			// Find: \t\t\t,? ?EF_DECLARE(\([^)]+\))
			// Replace: \t\t\tEF_ASSIGN(\1);

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
			EF_ASSIGN(psh,waterT);
			EF_ASSIGN(ps,waterT);
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
			EF_ASSIGN(psh,steamT);
			EF_ASSIGN(ps,steamT);
			EF_ASSIGN(x,pT);
			EF_ASSIGN(v,pT);
			EF_ASSIGN(u,pT);
			EF_ASSIGN(s,pT);
			EF_ASSIGN(h,pT);
			EF_ASSIGN(cp,pT);
			EF_ASSIGN(cv,pT);
			EF_ASSIGN(k,pT);
			EF_ASSIGN(mu,pT);
			EF_ASSIGN(rho,pT);
			EF_ASSIGN(region,pT);
			EF_ASSIGN(hs,pT);
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
			EF_ASSIGN(Ts,ph);
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
			EF_ASSIGN(Th,ps);
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
			EF_ASSIGN(Th,pu);
			EF_ASSIGN(Ts,pu);
			EF_ASSIGN(hs,pu);
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
			EF_ASSIGN(Th,uv);
			EF_ASSIGN(Ts,uv);
			EF_ASSIGN(hs,uv);
			EF_ASSIGN(psh,uv);
			EF_ASSIGN(ps,uv);
			EF_ASSIGN(p,Ts);
			EF_ASSIGN(x,Ts);
			EF_ASSIGN(v,Ts);
			EF_ASSIGN(u,Ts);
			EF_ASSIGN(h,Ts);
			EF_ASSIGN(cp,Ts);
			EF_ASSIGN(cv,Ts);
			EF_ASSIGN(k,Ts);
			EF_ASSIGN(mu,Ts);
			EF_ASSIGN(rho,Ts);
			EF_ASSIGN(region,Ts);
			EF_ASSIGN(ph,Ts);
			EF_ASSIGN(p,Th);
			EF_ASSIGN(x,Th);
			EF_ASSIGN(v,Th);
			EF_ASSIGN(u,Th);
			EF_ASSIGN(s,Th);
			EF_ASSIGN(cp,Th);
			EF_ASSIGN(cv,Th);
			EF_ASSIGN(k,Th);
			EF_ASSIGN(mu,Th);
			EF_ASSIGN(rho,Th);
			EF_ASSIGN(region,Th);
			EF_ASSIGN(ps,Th);
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
	obj->create(retval, msg);

	// Return our object address.
	*objectHandler = (int_t)obj;
}



// Keep this function as is.
void eo_destroy_ (const int_t *objectHandler,
                  int_t *retval, char *msg){
	// Cast back the object.
	ExternalObjectBase *obj = (ExternalObjectBase*) *objectHandler;

	// Call the correspounding c++ function.
	obj->destroy(retval, msg);
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
	obj->setParameter(parameterName, valueType, valueLength,
		values, valuesText, retval, msg);
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
	obj->checkMethod(methodName, methodID,
		numOfInputs, numOfOutputs, retval, msg);
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
	obj->methodDetails(methodName, methodID,
		numOfInputs, inputLengths, inputTypes, inputUnits,
		numOfOutputs, outputLengths, outputTypes, outputUnits,
		derivativeMatrix, retval, msg);
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
	obj->calc(methodName, methodID,
		numOfInputs, inputLengths, totalInputLength, inputValues,
		numOfOutputs, outputLengths, totalOutputLength, outputValues,
		calculateDerivatives, outputDerivatives, retval, msg);
}



