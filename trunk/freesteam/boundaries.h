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

#ifndef BOUNDARIES_H
#define BOUNDARIES_H

#include "designbycontract.h"
#include "units.h"
#include "common.h"

class Boundaries : public DesignByContract {

	public:

		static bool isValid_pT(Pressure p, Temperature T, bool throw_me =
		                           false);
		static bool isRegion1_pT(Pressure p, Temperature T, bool throw_me =
		                             false);
		static bool isRegion2_pT(Pressure p, Temperature T, bool throw_me =
		                             false);
		static bool isRegion3_pT(Pressure p, Temperature T, bool throw_me =
		                             false);
		//static bool isSat_pT(Pressure p, Temperature T, bool throw_me = false);	// (region 4)
		static bool isBound_pT(Pressure p, Temperature T, bool throw_me = false);	// curved boundary roughly below region 2

		static bool isSat_Tx(const Temperature &T, const Num &x, const bool throw_me=false);

		// Region 4 boundary: the following should be 100% consistent, FPU errors aside:
		static Pressure getSatPres_T(Temperature T);	// Forwards equation
		static Temperature getSatTemp_p(Pressure p);	// Backwards equation

		// Region 4 boundary for T > TB_LOW
		static Density getSatDensWater_T(const Temperature &T);
		static Density getSatDensSteam_T(const Temperature &T);

		static Pressure getpbound_T(Temperature T, bool throw_me = false);	// use this function wherever possible
		static Temperature getTbound_p(Pressure p, bool throw_me = false);	// in preference to this one.

	private:

		static Num upsilon(Temperature T);
		static Num A(Num ups);
		static Num B(Num ups);
		static Num C(Num ups);

		static Num beta(Pressure p);
		static Num E(Num bet);
		static Num F(Num bet);
		static Num G(Num bet);
		static Num D(Num E, Num F, Num G);
};

#endif
