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
#ifndef STEAMCALCULATOR_H
#define STEAMCALCULATOR_H

#include "designbycontract.h"
#include "common.h"
#include "steamcalculatorexception.h"
#include "boundaries.h"
#include "state.h"
#include "zeroin.h"

typedef enum { STEAM_SUBCOOLED =
                   0, STEAM_SUPERHEATED, STEAM_SATURATED, STEAM_SUPERCRITICAL,
               STEAM_STATE_UNKNOWN = -1 } SteamStateCode;

typedef enum { STEAM_TEMPERATURE =
                   0, STEAM_PRESSURE, STEAM_VOLUME, STEAM_INTERNAL_ENERGY,
               STEAM_ENTHALPY, STEAM_ENTROPY } SteamPropertyCode;

/**
	@mainpage

	@section intro Introduction

	This is the C++ class documentation for freesteam, generated using the doxygen tool. Please visit http://freesteam.sf.net/ for the latest information on freesteam.

	Freesteam implements the international-standard steam tables from the International Association for the Properties of Water and Steam, IAPWS. The correlations for both IAPWS-IF97 and IAPWS-95 are provided, although the emphasis is on the use of the IAPWS-IF97 correlation.

	@section license License

	freesteam - IAPWS-IF97 steam tables library \n
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

	@see http://www.gnu.org/copyleft/copyleft.html
*/

/// Core class - define steam state in terms of p,T and optionally x
/**
	@example
		To find the enthalpy of steam at p = 10 bar, T = 250 �C,

		@code
			SteamCalculator S;
			S.set_pT(10.0 * bar, fromcelsius(250));
			cerr << S.specenthalpy() << endl;
		@endcode
*/
class SteamCalculator:public DesignByContract {

	public:

		//// Constructor
		SteamCalculator();

		/// Copy constructor
		SteamCalculator(const SteamCalculator & original);

		/// Copy operation, virtual
		virtual void copy(const SteamCalculator & original);

		/// Assignment operator (assigns a copy)
		SteamCalculator const &operator=(SteamCalculator const &original);

		// Destructor
		virtual ~SteamCalculator();

		// Defining state, simple methods
		void set_pT(const Pressure &p, const Temperature &T, double x = 1.0);	// pressure [MPa]
		void setRegion3_rhoT(const Density &rho, const Temperature &T);	// will throw an error if not valid point in region 3

		void changeState(SteamState * state);

		void setSatSteam_p(const Pressure &p);	// p Pressure [MPa]
		void setSatWater_p(const Pressure &p);	// p Pressure [MPa]
		void setSatWater_T(const Temperature &T);	// T Temperature [K]
		void setSatSteam_T(const Temperature &T);	// T Temperature [K]

		void setB23_T(const Temperature &T);
		void setB23_p(const Pressure &p);

		void setRegion1_pT(const Pressure &p, const Temperature &T);
		void setRegion2_pT(const Pressure &p, const Temperature &T);
		void setRegion4_Tx(const Temperature &T, const Num &x);

		bool isSet(void) const;

		#ifndef NDEBUG
		// Design by contract stuff...
		virtual bool isValid(void) const;
		#endif

		// Methods to return properties and state

		int whichRegion(void) const;

		SteamStateCode whichState(void);
		const char *SteamCalculator::whichStateStr(void);

		Temperature temp() const;               // Temperature
		Pressure pres() const;                  // Pressure
		Density dens() const;                   // Density

		SpecificVolume specvol() const;         // Specific volume
		SpecificEnergy specienergy() const;     // Specific internal energy
		SpecificEntropy specentropy() const;    // Specific entropy
		SpecificEnergy specenthalpy() const;    // Specific entropy
		SpecHeatCap speccp() const;             // Specific isobaric heat capacity  [kJ/kgK]
		SpecHeatCap speccv() const;             // Specific isochoric heat capacity [kJ/kgK]
		Num quality() const;                    // Steam quality
		DynamicViscosity dynvisc() const;       // Dynamic viscosity, mu            [Pa.s]
		Conductivity conductivity() const;      // Conductivity

		//void setTarget(SteamPropertyCode p, Num value);

		/*
		HOW TO IMPLEMENT THE SOLVER WITH THESE STRONGLY TYPED THINGOS?

		template<SteamPropertyCode prop>
		Temperature
		solveTemperature(){

			ZeroIn<SteamCalculator,prop,
		*/

		//Temperature solveTemperature();

		//Pressure solvePressure();
		//Num solveQuality();

		//Num my_zeroin(Num ax, Num bx, double (*f) (double x), double tol);

		//Num getPropertyValue_p(Num p);
		//Num getPropertyValue_T(Num T);
		//Num getPropertyValue();

		//static double wrap_getPropertyValue_p(double);
		//static double wrap_getPropertyValue_T(double);

	protected:
		friend class SteamState;
		friend class Region1;
		friend class Region2;
		friend class Region3;
		friend class Region4;
		friend class ZeroIn < SteamCalculator, Density, Temperature >;
		friend class ZeroIn < SteamCalculator, SpecificEnergy, Pressure >;
		friend class ZeroIn < SteamCalculator, Pressure, Density >;

		Num tau_iaps85() const;
		Num del_iaps85() const;
		Num pi_iaps85() const;
		Num delpi_iaps85() const;
		Num pitau_iaps85() const;

		Num lam() const;	// for thermal conductivity.
		Num lam0() const;
		Num lam1() const;
		Num lam2() const;

		Num mu() const;
		Num mu0() const;
		Num mu1() const;
		Num mu2() const;

		Pressure getRegion3PressureError(const Density &test_rho);

		// All the intrinsic properties must be stored here because the states are singletons to save memory.

		Temperature T;			            ///< Temperature K
		Pressure p;			                ///< Pressure MPa
		Density rho;		                ///< Density, used by Region 3 only
		Num x;		                        ///< Quality, used by Region 4

		Num tau;	                        ///< Internal variable in IAPWS calcs
		Num pi;		                        ///< Internal variable in IAPWS calcs
		Num del;		                    ///< Internal variable in IAPWS calcs
		Pressure reg3_target_pressure;      ///< Internal variable used in root-finding in region 3

	private:

		bool isset;		///< boolean flag @see isSet()

		void destroy();

		template <class Abscissa,class Ordinate>
		void accept(ZeroIn<SteamCalculator, Abscissa, Ordinate> *z){
			z->visit(this);
		}

		SteamState *_state;

		SteamCalculator *gas;	// for use in two-phase calcs
		SteamCalculator *liq;	// for use in two-phase calcs

		//SteamPropertyCode zeroin_property;
		//Num zeroin_target_value;
		//SteamStateCode zeroin_asserted_state;

		// for set_rhou:
		Density rho_target;
		SpecificEnergy u_target;
		Temperature T_guess;
		bool rhou_region_1;


};

#endif
