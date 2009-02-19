/*
	This is a modified version of ees_record.h
	from Example C++ DLLs
	at http://www.fchart.com/ees/goodies.shtml
*/

#ifndef EESPARAMETERLIST_H
#define EESPARAMETERLIST_H

const int EES_MSG_LENGTH=256;

#define EES_FUNCTION(EES_NAME) \
	__declspec(dllexport) double EES_NAME(char s[256], int mode, EES_PARAM_REC *rec_in)

#define EES_PROCEDURE(EES_NAME) \
	__declspec(dllexport) void EES_NAME(char s[256], int mode, EES_PARAM_REC *rec_in, EES_PARAM_REC *rec_out)

#define EES_FDL(EES_NAME) \
	__declspec(dllexport) void EES_NAME(char s[256], int mode, EES_PARAM_REC *rec_in, EES_PARAM_REC *rec_out)

#define EES_FUNCTION_START(EES_NAME,EES_USAGE,EES_NUM_INPUTS) \
	__declspec(dllexport) \
	double \
	EES_NAME( \
			char s[EES_MSG_LENGTH] \
			, int mode \
			, EES_PARAM_REC *rec_in \
	){ \
		try { \
			EESParameterList inputs(rec_in); \
			int NInputs = inputs.length(); \
			if(mode==-1)EES_FUNCTION_STRING(EES_USAGE); \
			if(NInputs != EES_NUM_INPUTS){ \
				stringstream ss; \
				ss << "Expected " << EES_NUM_INPUTS << " but received " << NInputs << "."; \
				throw new Exception(ss.str()); \
			}


#define EES_FUNCTION_END(EES_NAME) \
		}catch(Exception *E){ \
			stringstream ss; \
			ss << #EES_NAME << ": " << E->what(); \
			delete E; \
			EES_FUNCTION_STRING(ss.str().c_str()); \
		} \
	}

#define EES_FUNCTION_STRING(EES_USAGE) \
	{ \
		strncpy(s,EES_USAGE,EES_MSG_LENGTH-1); \
		s[EES_MSG_LENGTH-1]='\0'; \
		return 0; \
	}

//--------------------------------------------------------------
//	Tell C++ to use the "C" style calling conventions rather than the C++ mangled names

#ifdef __cplusplus
extern "C"  {
#endif

	/// Structure for parameter records in DLF and DLP formats
	typedef struct EES_PARAM_REC {
		double value;
		struct EES_PARAM_REC *next;
	} EES_PARAM_REC;

#ifdef __cplusplus
};
#endif

//--------------------------------------------------------------
// Declare C++ interface for handling records chained lists

/**
	EES Parameter List - convenient helper class to access parameters being passed to/from EES
*/
class EESParameterList {

	public :

		EESParameterList(EES_PARAM_REC * begin_init); // Constructor

		int length();
		double& operator[] (const int &);

	private:

		/// Pointer to the first parameter
		EES_PARAM_REC * begin;


};

#endif
