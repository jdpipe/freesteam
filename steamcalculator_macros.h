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

//-------------------------------------------------------------------------
// These #defines set up some series-evaluation and debug-output functions

#include "isinfnan.h"

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
	Num CLASSNAME::FUNCNAME(const SteamCalculator &c) const{ \
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
	Num CLASSNAME::FUNC() const{ \
		Num FUNC=0; \
		FUNC=(EXPR); \
		return FUNC; \
	}

// create a function which returns an evaluated expression within a class
// ... passing in a SteamCalculator object with parameters
#define EXPR_FUNC_OBJ(CLASSNAME,FUNC,EXPR,TYPE) \
	TYPE CLASSNAME::FUNC(const SteamCalculator &c) const{ \
		TYPE FUNC=(EXPR); \
		ENSURE(!isinf(FUNC)); \
		ENSURE(!isnan(FUNC)); \
		return FUNC; \
	}

// this one adds a simple expresssion plus a series expression with control over the loop limits
#define EXPR_LOOP_SUM_FUNC_OBJ(CLASSNAME,FUNCNAME,EXT_EXPR,LOOP_EXPR,LOOP_START,LOOPMAX) \
	Num CLASSNAME::FUNCNAME(const SteamCalculator &c) const{ \
		Num s=(EXT_EXPR); \
		for(int i=(LOOP_START);i<(LOOPMAX);i++){\
			s+=(LOOP_EXPR); \
		} \
		return s; \
	}
