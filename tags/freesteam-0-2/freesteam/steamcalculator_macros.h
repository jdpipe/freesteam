//-------------------------------------------------------------------------
// These #defines set up some series-evaluation and debug-output functions

// Output messages and expression debug stuff

#define SHOW_STRVAL(EXPR) \
	printf("NOTE: "); \
	for(int xprloop=0;xprloop<expr_func_level;xprloop++){ \
		printf("  "); \
	} \
	printf("%s = %s\n",#EXPR,(EXPR));

#define SHOW_VALUE_LABEL(EXPR,LABEL) \
	{ \
		printf("NOTE: "); \
		for(int xprloop=0;xprloop<expr_func_level;xprloop++){ \
			printf("  "); \
		} \
		printf("%s = %8.5e\n",#LABEL,(Num)(EXPR)); \
	}

#define SHOW_VALUE(EXPR) SHOW_VALUE_LABEL((EXPR),(EXPR))


// create a function which returns an evaluated series expression within a class
#define LOOP_SUM_FUNC(CLASSNAME,FUNCNAME,EXPR,LOOPMAX) \
	Num CLASSNAME::FUNCNAME(){ \
		Num s=0; \
		for(int i=0;i<LOOPMAX;i++){\
			s+=(EXPR); \
		} \
		return s; \
	}

// create a function which returns an evaluated series expression within a class
// and passes SteamCalculator object
#define LOOP_SUM_FUNC_OBJ(CLASSNAME,FUNCNAME,EXPR,LOOPMAX) \
	Num CLASSNAME::FUNCNAME(SteamCalculator *c){ \
		Num s=0; \
		for(int i=0;i<LOOPMAX;i++){\
			s+=(EXPR); \
		} \
		ENSURE(!isnan(s)); \
		ENSURE(!isinf(s)); \
		return s; \
	}

// create a function which returns an evaluated expression within a class
#define EXPR_FUNC(CLASSNAME,FUNC,EXPR) \
	Num CLASSNAME::FUNC(){ \
		Num FUNC=0; \
		FUNC=(EXPR); \
		return FUNC; \
	}

// create a function which returns an evaluated expression within a class
// ... passing in a SteamCalculator object with parameters
#define EXPR_FUNC_OBJ(CLASSNAME,FUNC,EXPR,TYPE) \
	TYPE CLASSNAME::FUNC(SteamCalculator *c){ \
		TYPE FUNC=(EXPR); \
		ENSURE(!isinf(FUNC)); \
		ENSURE(!isnan(FUNC)); \
		return FUNC; \
	}

// this one adds a simple expresssion plus a series expression with control over the loop limits
#define EXPR_LOOP_SUM_FUNC_OBJ(CLASSNAME,FUNCNAME,EXT_EXPR,LOOP_EXPR,LOOP_START,LOOPMAX) \
	Num CLASSNAME::FUNCNAME(SteamCalculator *c){ \
		Num s=(EXT_EXPR); \
		for(int i=(LOOP_START);i<(LOOPMAX);i++){\
			s+=(LOOP_EXPR); \
		} \
		return s; \
	}
