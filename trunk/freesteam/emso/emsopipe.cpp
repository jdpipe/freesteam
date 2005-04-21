#include "../exception.h"
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

/** Data structure for an EMSO 'WaterSteam' package instance.
* This object contains instance-specific data.
*/
class EMSOpipe : public ExternalObjectBase {

	public:

		/**
			Constructor
		*/
		EMSOpipe(){
			#ifdef EMSO_DEBUG
				debug=false;

				numOfCalcCalls = 0;
				numOfInstances++;
				instanceNumber = instanceSerialNumber++;

				cerr << "Created EMSOpipe object " << instanceNumber << " (now " << numOfInstances << " instances)..." << endl;
			#endif

			initialiseMethodNames();
			initialiseParamNames();

			cerr.flags(ios_base::showbase);
		};

		/**
			Destructor
		*/
		virtual ~EMSOpipe(){
			#ifdef EMSO_DEBUG
				numOfInstances--;

				cerr << "Deleting EMSOpipe object " << instanceNumber << " (leaves " << numOfInstances << " instances)..." << endl;
			#endif
		};

		/**
			Create object. Anything risky should go here and throw an exception if necessary
		*/
		virtual void create(int_t *retval, char *msg){
			*retval = emso_ok;
		}

		/**
			Destroyes an instance of this object in EMSO
		*/
		virtual void destroy(int_t *retval, char *msg){
			*retval = emso_ok;
		}

		/**
			Set parameters (not used)
		*/
		virtual void setParameter(
				const char *parameterName
				,const int_t *valueType
				,const int_t *valueLength
				,const real_t *values
				,const char *valuesText[]
				,int_t *retval, char *msg
		){
			try{

				int param = convertParam(string(parameterName));
				if(param==0){
					stringstream s;
					s << "Parameter '" << parameterName << "' not found. Valid params are: " << getValidParamNames();
					throw Exception(s.str());
				}

				throw Exception("Setting of parameter not implemented yet.");

			}catch(Exception &E){
				stringstream s;
				s << "EMSOpipe::setParameter: " << E.what();
				report_error(msg, s.str());
				*retval = emso_error;
			}
		}

		/**
			Look up the method ID and number of inputs/outputs given a method ID or method names
		*/
		virtual void checkMethod(
				const char *methodName
				,int_t *methodID
				,int_t *numOfInputs
				,int_t *numOfOutputs
				,int_t *retval, char *msg
		){

			try{
				int_t method;

				*methodID = method = convertMethod(string(methodName));
				if(method == 0){
					stringstream s;
					s << "Method '" << methodName << "' not found. Valid methods are: " << getValidMethodNames();
					throw Exception(s.str());
				}

				// Set number of input and output variables
				switch(method & InputMask){

					case given_ReEd:
						*numOfInputs = 2;
						break;

					default:
						throw Exception("Invalid input parameter combo");
				}
				switch(method & OutputMask){
					case f:
						*numOfOutputs = 1;
						break;

					default:
						throw Exception("Invalid output parameter combo");
				}

				*retval = emso_ok;

			}catch(Exception &E){
				stringstream s;
				s << "EMSOpipe::checkMethod: " << E.what();
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
				int_t method;

				//----------------------
				// METHOD NAME/ID

				method = *methodID;
				if(!method)
					method = convertMethod(string(methodName));

				if(method == 0){
					stringstream s;
					s << "Method '" << methodName << "' not found.";
					throw Exception(s.str());
				}

				// All of our inputs and outputs are of length 1, hence we do not
				// need to set inputLengths or outputLengths

				//----------------------
				// UNITS

				// Units for this inputs

				switch(method & InputMask){
					case given_ReEd:
						// Dimensionless
						break;

					default:
						throw Exception("Invalid input param combination (when giving EMSO units for method inputs)");
				};

				// Units for the outputs
				switch(method & OutputMask){

					case f:
						// Dimensionless
						break;

					default:
						throw Exception("Invalid output param combination (when giving EMSO units for method outputs)");
				}

				*retval = emso_ok;

			}catch(Exception &E){
				stringstream s;
				s << "EMSOpipe::calc: " << E.what();
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
			try{

				#ifdef EMSO_DEBUG
					++numOfCalcCalls;
				#endif

				int_t method;
				method = *methodID;
				if(!method)
					method = convertMethod(string(methodName));

				if(method == 0){
					stringstream s;
					s << "Method '" << methodName << " not found";
					throw Exception(s.str());
				}

				#ifdef EMSO_DEBUG
					cerr << "EMSOpipe[" << instanceNumber << "," << numOfCalcCalls << "]: " << methodName << "(";
					try{
				#endif
						switch(method){
							case f | given_ReEd:
								cerr << "Re = " << inputValues[0] << "," << "eps/D = " << inputValues[1] << ")";
								outputValues[0] = getFrictionFactor(inputValues[0],inputValues[1]);
								cerr << " = " << outputValues[0] << endl;
								break;

							default:
								throw Exception("EMSOpipe::calc: Method not implemented.");
						}

				#ifdef EMSO_DEBUG
					}catch(Exception &E){
						cerr << methodName << ": error during evaluation: " << E.what() << endl;
						throw;
					}
				#endif

				*retval = emso_ok;

			}catch(Exception &E){
				stringstream s;
				s << "EMSOpipe::calc: " << E.what();

				report_error(msg, s.str());
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
					throw Exception("EMSOpipe::frictionFactor: Reynolds number out of range");
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
						throw Exception(s.str());
					}
				}

				return 1.0 / sq(oneOnRootFnew);
			}
		}

		/// The current number of created EMSOpipe objects.
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
		void initialiseMethodNames(){
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


		string getValidParamNames(){

			stringstream ss;

			ParamMap::iterator i;

			bool first=true;
			int perthisline=-1;
			for(i = paramNames.begin(); i!=paramNames.end(); i++){
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

		/**
			Convert a string method name to one of the enumerators #method_names
			If the given name is not a valid method #unknownMethod is returned.
		*/
		int_t convertParam(const string &name){

			ParamMap::iterator i;

			i = paramNames.find(name);

			if(i != paramNames.end()){
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

#ifdef EMSO_DEBUG
	int_t EMSOpipe::numOfInstances = 0;
	int_t EMSOpipe::instanceSerialNumber = 0;
#endif

EMSOpipe::MethodMap EMSOpipe::methodNames=EMSOpipe::MethodMap();
EMSOpipe::ParamMap EMSOpipe::paramNames=EMSOpipe::ParamMap();


//* The factory function
ExternalObjectBase* ExternalObjectFactory(){
	return new EMSOpipe();
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



