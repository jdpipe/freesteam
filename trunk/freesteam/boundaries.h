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
