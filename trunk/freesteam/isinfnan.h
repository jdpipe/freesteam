/*
	Taken from

	http://www.opensource.apple.com/darwinsource/10.3/libxml2-4/libxml2/include/win32config.h

	the replaced with stuff form

	http://www.devdaily.com/scw/c/cygwin/src/winsup/mingw/mingwex/math/cbrt.c.shtml
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
