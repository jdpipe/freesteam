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

#ifndef CONVERGENCETEST_H
#define CONVERGENCETEST_H

#include "units.h"

template<class Property,int PropertyAlternative=0>
class ConvergenceTest{

	private:
		static const double errorP = 1e-6;
		static const double errorH = 1e-6;
		static const double errorS = 0.0001;
		static const double errorV = 1e-10;
		static const double errorRHO = 0.0001;
		static const double errorU = 1e-12;
		static const double errorCP = 0.0001;
		static const double errorCV = 0.0001;
		static const double errorX = 0.00001;
		static const double errorT = 1e-10;

	public:
		static bool test(const Property &X, const Pressure &p, const Temperature &T);
};

#endif
