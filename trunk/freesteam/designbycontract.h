#ifndef DESIGNBYCONTRACT_H
#define DESIGNBYCONTRACT_H

#ifndef NDEBUG

// Design-by-contract base class

/**
	Base class for all objects in the system. All classes inheriting from this class need to define a method IsValid. This method should perform a consistency check on the state of the object. Note that this method needs to be defined only when a debug build is made.
	
	@see http://www.eventhelix.com/RealtimeMantra/Object_Oriented/design_by_contract.htm
*/
class DesignByContract {

	public:

#ifndef NDEBUG
		virtual bool isValid();

		static void __abort_Program_(const char *file, const int line,
		                             const char *expr, const char *type);

#endif

};

// The debug mode also defines the following macros. Failure of any of these macros leads to
// program termination. The user is notified of the error condition with the right file name
// and line number. The actual failing operation is also printed using the stringizing operator #

#define ASSERT__(type,expr) if (!(expr)) DesignByContract::__abort_Program_(__FILE__, __LINE__, #expr,type)

#define ASSERT(expr) ASSERT__("ASSERTION",expr)
#define IS_VALID(obj) ASSERT__("CONSTISTENCY CHECK",(obj) != NULL && (obj)->isValid())
#define REQUIRE(expr) ASSERT__("PRE-CONDITION",expr)
#define ENSURE(expr) ASSERT__("POST-CONDITION",expr)

#else

// When built in release mode, the _DEBUG flag would not be defined, thus there will be no overhead
// in the final release from these checks.

#define ASSERT(expr) ((void) 0)
#define IS_VALID(expr) ((void) 0)
#define REQUIRE(expr) ((void) 0)
#define ENSURE(expr) ((void) 0)

#endif

#endif
