// EMSO External Object Interface for C++
// with macros for convenience
// by John Pye, 2005, http://pye.dyndns.org/

#ifndef EMSOEXTOBJ_H
#define EMSOEXTOBJ_H

#include <sstream>
#include <exception>
#include <stdexcept>
#include <string>
#include <map>

#include <emso/external_object.h>


typedef std::map<const std::string, int_t> MethodMap;
typedef std::map<const std::string, int_t> ParamMap;


/// SolverContext<SolverStrategy> class.
/**
	Template Class to facility C++ plugin-style architecture for External Objects.
	Because of the current C hooks technique, it's hard to fully realise this
	approach however it should cut down on the amount of code required to
	write an external object for EMSO.

	Also includes an extensible system for retreiving method names from the External Object.
*/
template<class Strategy>
class ExternalObjectContext
{
private:
	Strategy strategy; // specified at compile time, see GOF p 319.

public:

	void create(int_t *retVal, char *msg){
		try{
			strategy.create();
			strategy.initialiseMethodNames();
			strategy.initialiseParamNames();

			*retVal = emso_ok;
		}catch(std::exception &e){
			snprintf(msg,EMSO_MESSAGE_LENGTH,e.what());
			*retVal = emso_error;
		}
	}

	void destroy(
			int_t *retVal, char *msg
	){
		try{
			strategy.destroy();
			*retVal = emso_ok;
		}catch(std::exception &e){
			snprintf(msg,EMSO_MESSAGE_LENGTH,e.what());
			*retVal = emso_error;
		}
	}

	/**
		Set parameters (not used?)
	*/
	void setParameter(
			const char *parameterName
			,const int_t *valueType
			,const int_t *valueLength
			,const real_t *values
			,const char *valuesText[]
			,int_t *retval, char *msg
	){
		try{

			int param = convertParam(std::string(parameterName));
			if(param==0){
				std::stringstream s;
				s << "Parameter '" << parameterName << "' not found. Valid params are: " << getValidParamNames();
				throw std::runtime_error(s.str());
			}

			throw std::runtime_error("Setting of parameter not implemented yet.");

		}catch(std::exception &E){
			std::stringstream s;
			s << "ExternalObjectContext::setParameter: " << E.what();
			report_error(msg, s.str());
			*retval = emso_error;
		}
	}

	/**
		Look up the method ID and number of inputs/outputs given a method ID or method names
	*/
	void checkMethod(
			const char *methodName
			,int_t *methodID
			,int_t *numOfInputs
			,int_t *numOfOutputs
			,int_t *retval, char *msg
	){

		try{
			int_t method;

			*methodID = method = convertMethod(std::string(methodName));
			if(method == 0){
				std::stringstream s;
				s << "Method '" << methodName << "' not found. Valid methods are: " << getValidMethodNames();
				throw std::runtime_error(s.str());
			}

			*numOfInputs = getNumOfInputs(method);
			*numOfOutputs = getNumOfOutputs(method);

			*retval = emso_ok;

		}catch(std::exception &E){
			std::stringstream s;
			s << "ExternalObjectContext::checkMethod: " << E.what();
			report_error(msg, s.str());
			*retval = emso_error;
		}
	}

	/// Method details
	/**
		Relate information about the available methods in this component back to EMSO
	*/
	void methodDetails(
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
			if(!method){
				method = convertMethod(std::string(methodName));
			}


			if(method == 0){
				std::stringstream s;
				s << "Method '" << methodName << "' not found.";
				throw std::runtime_error(s.str());
			}

			strategy.getInputUnits(method, inputUnits);
			strategy.getOutputUnits(method, outputUnits);

			*retval = emso_ok;

		}catch(std::exception &e){
			std::stringstream s;
			s << "ExternalObjectContext::calc: " << e.what();
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
		@return EMSO status code (@see <emso/emso_types.h>)
	*/
	inline void calc(
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

			,int_t *retVal, char *msg
	){
		try{
			strategy.msg = msg;
			
			if(methodID==NULL){
				throw std::runtime_error("Method ID has not been looked up");
			}
	
			if(*methodID == 0){
				std::stringstream ss;
				ss << "Method '" << methodName << " not found";
				throw std::runtime_error(ss.str());
			}
	
			strategy.calc(methodID, methodName, inputValues, outputValues);
	
			*retVal = emso_ok;
		}catch(std::exception &e){
			report_error(msg,e.what());
			*retVal = emso_error;
		}catch(...){
			report_error(msg,"ExternalObjectContext::solve: Unknown exception");
			*retVal = emso_error;
		}
	}

private:

	std::string getValidMethodNames(){

		std::stringstream ss;

		MethodMap::iterator i;

		bool first=true;
		int perthisline=-1;
		for(i = strategy.methodNames.begin(); i!=strategy.methodNames.end(); i++){
			if(!first){
				ss << ", ";
			}else{
				first=false;
			}

			if(!(++perthisline %= 10)){
				ss << std::endl;
			}
			ss << (*i).first;

		}

		ss << std::endl;

		return ss.str();
	}


	std::string getValidParamNames(){

		std::stringstream ss;

		ParamMap::iterator i;

		bool first=true;
		int perthisline=-1;
		for(i = strategy.paramNames.begin(); i!=strategy.paramNames.end(); i++){
			if(!first){
				ss << ", ";
			}else{
				first=false;
			}

			if(!(++perthisline %= 10)){
				ss << std::endl;
			}
			ss << (*i).first;

		}

		ss << std::endl;

		return ss.str();
	}


	/**
		Convert a string method name to one of the enumerators #method_names
		If the given name is not a valid method #unknownMethod is returned.
	*/
	int_t convertMethod(const std::string &name){

		MethodMap::iterator i;

		i = strategy.methodNames.find(name);

		if(i != strategy.methodNames.end()){
			return (*i).second;
		}

		return 0;
	}

	/**
		Convert a string method name to one of the enumerators #method_names
		If the given name is not a valid method #unknownParam is returned.
	*/
	int_t convertParam(const std::string &name){

		ParamMap::iterator i;

		i = strategy.paramNames.find(name);

		if(i != strategy.paramNames.end()){
			return (*i).second;
		}

		return 0;
	}

	inline int_t getNumOfInputs(const int_t &method){
		return strategy.getNumOfInputs(method);
	}

	inline int_t getNumOfOutputs(const int_t &method){
		return strategy.getNumOfOutputs(method);
	}

	inline void getInputUnits(const int_t &method, char *inputUnits[]){
		strategy.getInputUnits(method, inputUnits);
	}

	inline void getOutputUnits(const int_t &method, char *outputUnits[]){
		strategy.getOutputUnits(method, outputUnits);
	}

	/** check if we have an error and print it to the given msg
	*
	*/
	void report_error(char *msg, const std::string &error){
		if(error.size() > EMSO_MESSAGE_LENGTH){
			std::stringstream ss;
			ss << error.substr(0,EMSO_MESSAGE_LENGTH-24) << "... (message cut short)";
			snprintf(msg,EMSO_MESSAGE_LENGTH,ss.str().c_str());
		}else{
			snprintf(msg, EMSO_MESSAGE_LENGTH, error.c_str());
		}
	}

};

/**
	Abstract SolverStrategy class

	Note that ResFn, LSetup and LSolve are for-convenience member functions that
	call the functions pointed to by *resfn, *lsetup and *lsolve.
*/
class ExternalObjectStrategy{

public:

	char *msg; // for possible use by destructor
	ParamMap paramNames;
	MethodMap methodNames;
	
public:

	virtual ~ExternalObjectStrategy(){
		// assume nothing to do here
	}

	virtual void create(void) = 0;

	virtual void calc(const int_t *methodID, const char *methodName, const real_t *inputValues, real_t *outputValues) = 0;

	virtual void destroy() = 0;

	virtual void initialiseMethodNames() = 0;
	virtual void initialiseParamNames() = 0;

	virtual int_t getNumOfInputs(const int_t &method) = 0;
	virtual int_t getNumOfOutputs(const int_t &method) = 0;

	virtual void getInputUnits(const int_t &method, char *inputUnits[]) = 0;
	virtual void getOutputUnits(const int_t &method, char *inputUnits[]) = 0;

};

//------------------------------
// WRAPPER MACROS
//
// These methods will define the necessary C functions to wrap your C++ External Object 'Strategy' class.

#define DECLARE_FACTORY(STRATEGYCLASS) \
	ExternalObjectContext<STRATEGYCLASS> *ExternalObjectFactory(){ \
		return new ExternalObjectContext<STRATEGYCLASS>(); \
	} \

#define EO_CAST_OBJ(POINTER,STRATEGYCLASS) \
	ExternalObjectContext<STRATEGYCLASS> *obj = (ExternalObjectContext<STRATEGYCLASS>*) *POINTER;

#define EXPORT_EO_CREATE(STRATEGYCLASS) \
	void eo_create_ (int_t *objectHandler, int_t *retval, char *msg){ \
		ExternalObjectContext<STRATEGYCLASS> *obj = ExternalObjectFactory(); \
		obj->create(retval, msg); \
		*objectHandler = (int_t)obj; \
	}

#define EXPORT_EO_DESTROY(STRATEGYCLASS) \
	void eo_destroy_ (const int_t *objectHandler, int_t *retval, char *msg){ \
		EO_CAST_OBJ(objectHandler,STRATEGYCLASS) \
 		obj->destroy(retval, msg); \
		delete obj; \
	}

#define EXPORT_EO_SET_PARAMETER(STRATEGYCLASS) \
	void eo_set_parameter_ (const int_t *objectHandler, const char *parameterName, const int_t *valueType, const int_t *valueLength, const real_t *values, const char *valuesText[], int_t *retval, char *msg){ \
		EO_CAST_OBJ(objectHandler,STRATEGYCLASS) \
		obj->setParameter(parameterName, valueType, valueLength, values, valuesText, retval, msg); \
	}

#define EXPORT_EO_CHECK_METHOD(STRATEGYCLASS) \
	void eo_check_method_(const int_t *objectHandler, const char *methodName, int_t *methodID, int_t *numOfInputs, int_t *numOfOutputs, int_t *retval, char *msg){ \
		EO_CAST_OBJ(objectHandler,STRATEGYCLASS) \
		obj->checkMethod(methodName, methodID, numOfInputs, numOfOutputs, retval, msg); \
	}

#define EXPORT_EO_METHOD_DETAILS(STRATEGYCLASS) \
	void eo_method_details_(const int_t *objectHandler, const char *methodName, const int_t *methodID, const int_t *numOfInputs, int_t *inputLengths, int_t *inputTypes, char *inputUnits[], const int_t *numOfOutputs, int_t *outputLengths, int_t *outputTypes, char *outputUnits[], int_t *derivativeMatrix, int_t *retval, char *msg){ \
		EO_CAST_OBJ(objectHandler,STRATEGYCLASS) \
		obj->methodDetails(methodName, methodID, numOfInputs, inputLengths, inputTypes, inputUnits, numOfOutputs, outputLengths, outputTypes, outputUnits, derivativeMatrix, retval, msg); \
	}

#define EXPORT_EO_CALC(STRATEGYCLASS) \
	void eo_calc_(const int_t *objectHandler, const char *methodName, const int_t *methodID, const int_t *numOfInputs, const int_t *inputLengths, const int_t *totalInputLength, const real_t *inputValues, const int_t *numOfOutputs, const int_t *outputLengths, const int_t *totalOutputLength, real_t *outputValues, const int_t *calculateDerivatives, real_t *outputDerivatives, int_t *retval, char *msg){ \
		EO_CAST_OBJ(objectHandler,STRATEGYCLASS) \
		obj->calc(methodName, methodID, numOfInputs, inputLengths, totalInputLength, inputValues, numOfOutputs, outputLengths, totalOutputLength, outputValues, calculateDerivatives, outputDerivatives, retval, msg); \
	}

#define EXPORT_EXTERNAL_OBJECT(STRATEGYCLASS) \
	DECLARE_FACTORY(STRATEGYCLASS) \
	EXPORT_EO_CREATE(STRATEGYCLASS) \
	EXPORT_EO_DESTROY(STRATEGYCLASS) \
	EXPORT_EO_SET_PARAMETER(STRATEGYCLASS) \
	EXPORT_EO_CHECK_METHOD(STRATEGYCLASS) \
	EXPORT_EO_METHOD_DETAILS(STRATEGYCLASS) \
	EXPORT_EO_CALC(STRATEGYCLASS)

#endif
