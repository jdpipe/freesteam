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

/*
	This file is a hack to make everything compile as normal on MinGW, which has a funny implementation of isnan and isinf

	@see
		http://www.opensource.apple.com/darwinsource/10.3/libxml2-4/libxml2/include/win32config.h
		and http://www.devdaily.com/scw/c/cygwin/src/winsup/mingw/mingwex/math/cbrt.c.shtml

*/

#ifndef ISINFNAN_H
	#define ISINFNAN_H

	#if defined(__MINGW32__)

			#include <float.h>

			inline int isnan(const double &x){
				return _isnan(x);
			}

			inline int isinf(const double &x){
				return _finite(x) ? 0 : 1;
			}


	#endif
#endif