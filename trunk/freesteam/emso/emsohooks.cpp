#include "../steamcalculator.h"
#include "../solver2.h"

#ifndef HAVE_EMSO
	#error EMSO not configured on your system, check CPPFLAGS in the file PACKAGE_ROOT/make-xxxxx.mak for your system.
#endif

#include <emso/external_object.h>

#include <sstream>
#include <cstring>
#include <map>
using namespace std;

#define EF_ASSIGN(name) m[#name]=name

/** Data structure for an EMSO 'WaterSteam' package instance.
* This object contains instance-specific data.
*/
class EMSOfreesteam : public ExternalObjectBase {
public:

	EMSOfreesteam(){
		// Intentionally empty
	};

	~EMSOfreesteam(){
		// Intentionally empty
	};

	/**
		Creates an instance of this module in EMSO
	*/
	virtual void create(int_t *retval, char *msg){
		++numOfPackages;
	}

	/**
		Destroyes an instance of this object in EMSO
	*/
	virtual void destroy(int_t *retval, char *msg){
		--numOfPackages;
	}

	/// Set a parameter for the instance.
	virtual void setParameter(const char *parameterName,
			const int_t *valueType,
			const int_t *valueLength,
			const real_t *values,
			const char *valuesText[],
			int_t *retval, char *msg
	){
		*retval = emso_error;
		snprintf(msg, EMSO_MESSAGE_LENGTH, "EMSOfreesteam has no parameters");
	}

	/// Check method.
	virtual void checkMethod(const char *methodName,
			int_t *methodID,
			int_t *numOfInputs,
			int_t *numOfOutputs,
			int_t *retval, char *msg
	){

		try{
			int_t method, input, output;
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
					*numOfInputs = 2;
					break;

				default:
					throw new Exception("Invalid input parameter combo");
			}

			// Number of output parameters:
			switch(output){
				default:
					*numOfOutputs = 1;
					break;
			}

		}catch(Exception *E){
			stringstream s;
			s << "EMSOfreesteam::checkMethod: " << E->what();
			report_error(retval, msg, s.str().c_str());
			delete E;
		}
	}

	/// Method details
	/**
		Relate information about the available methods in this component back to EMSO
	*/
	virtual void methodDetails(const char *methodName,
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

		try{
			int_t method, input, output;

			method = *methodID;
			if(!method)
				method = convertMethod(string(methodName));

			if(method == 0){
				stringstream s;
				s << "Method '" << methodName << "' not found.";
				throw new Exception(s.str());
				return;
			}

			input = method & InputMask;
			output = method & OutputMask;

			//all of our inputs and outputs are of length 1, hence we do not
			//need to set inputLengths or outputLengths

			// the method inputs
			switch(input){
				case given_pTx:
					strcpy(inputUnits[0], "Pa");
					strcpy(inputUnits[1], "K");
					strcpy(inputUnits[2], "");

				case given_ph:
				case given_pu:
					strcpy(inputUnits[0], "Pa");
					strcpy(inputUnits[1], "kJ/kg");
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
					strcpy(outputUnits[0], "Pa");
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

				case region:
					break;

				default:
					throw new Exception("Invalid output param combination");
			}
		}catch(Exception *E){
			stringstream s;
			s << "EMSOfreesteam::calc: " << E->what();
			report_error(retval, msg, s.str().c_str());
			delete E;
		}
	}

	/**
		Calculation
	*/
	virtual void calc(const char *methodName,
			const int_t *methodID,

			const int_t *numOfInputs,
			const int_t *inputLengths,
			const int_t *totalInputLength,
			const real_t *inputValues,

			const int_t *numOfOutputs,
			const int_t *outputLengths,
			const int_t *totalOutputLength,
			real_t *outputValues,

			const int_t *calculeDerivatives,
			real_t *outputDerivatives,

			int_t *retval, char *msg
	){
		SteamCalculator S;
		Solver2<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY> SS_PH;
		Solver2<Pressure,SpecificEnergy,0,SOLVE_IENERGY> SS_PU;
		Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> SS_UV;

		try{
			int_t method, input, output;
			method = *methodID;
			if(!method)
				method = convertMethod(string(methodName));

			if(method == 0){
				stringstream s;
				s << "Method '" << methodName << " not found";
				throw new Exception(s.str());
				return;
			}
			input = method & InputMask;
			output = method & OutputMask;

			switch(input){
				case given_waterT:
					S.setSatWater_T(inputValues[0] * Kelvin);
					break;

				case given_steamT:
					S.setSatSteam_T(inputValues[0] * Kelvin);
					break;

				case given_pTx:
					S.set_pT(inputValues[0] * Pascal, inputValues[1] * Kelvin, inputValues[2]);
					break;

				case given_ph:
					S = SS_PH.solve(inputValues[0] * Pascal, inputValues[1] * kJ_kg);
					break;

				case given_pu:
					S = SS_PU.solve(inputValues[0] * Pascal, inputValues[1] * kJ_kg);
					break;

				case given_uv:
					S = SS_UV.solve(inputValues[0] * kJ_kg, inputValues[1] * m3_kg);
					break;

				default:
					throw new Exception("EMSOfreesteam::calc: Un-handled input option (may be not yet implemented)");
			}

			switch(output){
				case T:
					outputValues[0] = (S.temp()/Kelvin);
					break;
				case p:
					outputValues[0] = S.pres() / Pascal;
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

				default:
					throw new Exception("Invalid output option (should never happen)");
			}
		}catch(Exception *E){
			stringstream s;
			s << "EMSOfreesteam::calc: " << E->what();
			report_error(retval, msg, s.str().c_str());
			delete E;
		}
	}

private:

	/// The current number of created EMSOfreesteam objects.
	static int_t numOfPackages;

	/// Specific property options
	enum input_arguments {
		given_pTx = 1
		, given_ph
		, given_pu
		, given_uv
		, given_waterT
		, given_steamT

		, InputMask = 0x00000fff
	};

	static const int shift = 16;

	/// Returned property options
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
		,region = 0x01000000

		,OutputMask = 0xffff0000
	};

	/// The methods
	enum method_name {

		p_waterT = p | given_waterT
		,x_waterT = x | given_waterT
		,v_waterT = v | given_waterT
		,u_waterT = u | given_waterT
		,s_waterT = s | given_waterT
		,h_waterT = h | given_waterT
		,cp_waterT = cp | given_waterT
		,cv_waterT = cv | given_waterT
		,k_waterT = k | given_waterT
		,mu_waterT = mu | given_waterT
		,rho_waterT = rho | given_waterT
		,region_waterT = region | given_waterT

		,p_steamT = p | given_steamT
		,x_steamT = x | given_steamT
		,v_steamT = v | given_steamT
		,u_steamT = u | given_steamT
		,s_steamT = s | given_steamT
		,h_steamT = h | given_steamT
		,cp_steamT = cp | given_steamT
		,cv_steamT = cv | given_steamT
		,k_steamT = k | given_steamT
		,mu_steamT = mu | given_steamT
		,rho_steamT = rho | given_steamT
		,region_steamT = region | given_steamT

		,T_pTx = T | given_pTx
		,p_pTx = p | given_pTx
		,x_pTx = x | given_pTx
		,v_pTx = v | given_pTx
		,u_pTx = u | given_pTx
		,s_pTx = s | given_pTx
		,h_pTx = h | given_pTx
		,cp_pTx = cp | given_pTx
		,cv_pTx = cv | given_pTx
		,k_pTx = k | given_pTx
		,mu_pTx = mu | given_pTx
		,rho_pTx = rho | given_pTx
		,region_pTx = region | given_pTx

		,T_ph = T | given_ph
		,x_ph = x | given_ph
		,v_ph = v | given_ph
		,u_ph = u | given_ph
		,s_ph = s | given_ph
		,cp_ph = cp | given_ph
		,cv_ph = cv | given_ph
		,k_ph = k | given_ph
		,mu_ph = mu | given_ph
		,rho_ph = rho | given_ph
		,region_ph = region | given_ph

		,T_pu = T | given_pu
		,x_pu = x | given_pu
		,v_pu = v | given_pu
		,s_pu = s | given_pu
		,h_pu = h | given_pu
		,cp_pu = cp | given_pu
		,cv_pu = cv | given_pu
		,k_pu = k | given_pu
		,mu_pu = mu | given_pu
		,rho_pu = rho | given_pu
		,region_pu = region | given_pu

		,T_uv = T | given_uv
		,p_uv = p | given_uv
		,x_uv = x | given_uv
		,s_uv = s | given_uv
		,h_uv = h | given_uv
		,cp_uv = cp | given_uv
		,cv_uv = cv | given_uv
		,k_uv = k | given_uv
		,mu_uv = mu | given_uv
		,rho_uv = rho | given_uv
		,region_uv = region | given_uv
	};

	/**
		Convert a string method name to one of the enumerators #method_names
		If the given name is not a valid method #unknownMethod is returned.
	*/
	int_t convertMethod(const string &name){

		map<const string,enum method_name> m;
		map<const string,enum method_name>::iterator i;

		// These lines can be made using some quick regexps from the above stuff, or vv
		EF_ASSIGN(p_waterT);
		EF_ASSIGN(x_waterT);
		EF_ASSIGN(v_waterT);
		EF_ASSIGN(u_waterT);
		EF_ASSIGN(s_waterT);
		EF_ASSIGN(h_waterT);
		EF_ASSIGN(cp_waterT);
		EF_ASSIGN(cv_waterT);
		EF_ASSIGN(k_waterT);
		EF_ASSIGN(mu_waterT);
		EF_ASSIGN(rho_waterT);
		EF_ASSIGN(region_waterT);

		EF_ASSIGN(p_steamT);
		EF_ASSIGN(x_steamT);
		EF_ASSIGN(v_steamT);
		EF_ASSIGN(u_steamT);
		EF_ASSIGN(s_steamT);
		EF_ASSIGN(h_steamT);
		EF_ASSIGN(cp_steamT);
		EF_ASSIGN(cv_steamT);
		EF_ASSIGN(k_steamT);
		EF_ASSIGN(mu_steamT);
		EF_ASSIGN(rho_steamT);
		EF_ASSIGN(region_steamT);

		EF_ASSIGN(T_pTx);
		EF_ASSIGN(p_pTx);
		EF_ASSIGN(x_pTx);
		EF_ASSIGN(v_pTx);
		EF_ASSIGN(u_pTx);
		EF_ASSIGN(s_pTx);
		EF_ASSIGN(h_pTx);
		EF_ASSIGN(cp_pTx);
		EF_ASSIGN(cv_pTx);
		EF_ASSIGN(k_pTx);
		EF_ASSIGN(mu_pTx);
		EF_ASSIGN(rho_pTx);
		EF_ASSIGN(region_pTx);

		EF_ASSIGN(T_ph);
		EF_ASSIGN(x_ph);
		EF_ASSIGN(v_ph);
		EF_ASSIGN(u_ph);
		EF_ASSIGN(s_ph);
		EF_ASSIGN(cp_ph);
		EF_ASSIGN(cv_ph);
		EF_ASSIGN(k_ph);
		EF_ASSIGN(mu_ph);
		EF_ASSIGN(rho_ph);
		EF_ASSIGN(region_ph);

		EF_ASSIGN(T_pu);
		EF_ASSIGN(x_pu);
		EF_ASSIGN(v_pu);
		EF_ASSIGN(s_pu);
		EF_ASSIGN(h_pu);
		EF_ASSIGN(cp_pu);
		EF_ASSIGN(cv_pu);
		EF_ASSIGN(k_pu);
		EF_ASSIGN(mu_pu);
		EF_ASSIGN(rho_pu);
		EF_ASSIGN(region_pu);

		EF_ASSIGN(T_uv);
		EF_ASSIGN(p_uv);
		EF_ASSIGN(x_uv);
		EF_ASSIGN(s_uv);
		EF_ASSIGN(h_uv);
		EF_ASSIGN(cp_uv);
		EF_ASSIGN(cv_uv);
		EF_ASSIGN(k_uv);
		EF_ASSIGN(mu_uv);
		EF_ASSIGN(rho_uv);
		EF_ASSIGN(region_uv);


		i = m.find(name);

		if(i != m.end()){
			return (*i).second;
		}

		return 0;
	}

	/** check if we have an error and print it to the given msg
	*
	*/
	void report_error(int_t *retval, char *msg, const char *e){
		*retval = emso_error;
		snprintf(msg, EMSO_MESSAGE_LENGTH, e);
	}

};

int_t EMSOfreesteam::numOfPackages = 0;


//* The factory function
ExternalObjectBase* ExternalObjectFactory(){
	return new EMSOfreesteam;
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
					  int_t *retval, char *msg){
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

						int_t *retval, char *msg){
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

			  const int_t *calculeDerivatives,
			  real_t *outputDerivatives,

			  int_t *retval, char *msg){
    // Cast back the object.
	ExternalObjectBase *obj = (ExternalObjectBase *)*objectHandler;

	// Call the correspounding c++ function.
	obj->calc(methodName, methodID,
		numOfInputs, inputLengths, totalInputLength, inputValues,
		numOfOutputs, outputLengths, totalOutputLength, outputValues,
		calculeDerivatives, outputDerivatives,
		retval, msg);
}



