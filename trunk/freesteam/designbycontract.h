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

#ifndef DESIGNBYCONTRACT_H
#define DESIGNBYCONTRACT_H

/// Design-by-contract base class
/**
	Base class for all objects in the system. All classes inheriting from this class need to define a method IsValid. This method should perform a consistency check on the state of the object. Note that this method needs to be defined only when a debug build is made.

	@see http://www.eventhelix.com/RealtimeMantra/Object_Oriented/design_by_contract.htm
*/
class DesignByContract {

	public:

#ifndef NDEBUG
		virtual bool isValid() const;

		static void __throw_error_(const char *file, const int line,
		                             const char *expr, const char *type);

		static void DesignByContract::__report_Message_(const char *file, const int line, const char *msg);

#endif

};

// The debug mode also defines the following macros. Failure of any of these macros leads to
// program termination. The user is notified of the error condition with the right file name
// and line number. The actual failing operation is also printed using the stringizing operator #

#ifndef NDEBUG

	#define ASSERT__(type,expr) if (!(expr)) DesignByContract::__throw_error_(__FILE__, __LINE__, #expr,type)

	#define ASSERT(expr) ASSERT__("ASSERTION",expr)
	#define IS_VALID(obj) ASSERT__("CONSTISTENCY CHECK",(obj) != NULL && (obj)->isValid())
	#define IS_VALID_REF(obj) ASSERT__("CONSTISTENCY CHECK",(obj).isValid())
	#define REQUIRE(expr) ASSERT__("PRE-CONDITION",expr)
	#define ENSURE(expr) ASSERT__("POST-CONDITION",expr)

	#define MESSAGE(msg) DesignByContract::__report_Message_(__FILE__,__LINE__,msg)

#else

	// When built in release mode, the _DEBUG flag would not be defined, thus there will be no overhead
	// in the final release from these checks.

	#define ASSERT(expr) ((void) 0)
	#define IS_VALID(expr) ((void) 0)
	#define IS_VALID_REF(expr) ((void) 0)
	#define REQUIRE(expr) ((void) 0)
	#define ENSURE(expr) ((void) 0)

	#define MESGAGE(msg) ((void) 0)

#endif

#endif
