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

#ifndef SOLVER_H
#define SOLVER_H

#include "steamcalculator.h"
#include "steamproperty.h"
#include "zeroin.h"
#include <stdexcept>
#include <string>

/// Base class for one-property rootfinding
/**
	@param MainProp	fixed value
	@param OtherProp target value
	@param VaryProp value which is varied unti OtherProp has desired value

	Note that dimensionally some properties are equivalent, for example
	specific internal energy and specific enthalpy. In these cases you need
	to use the OtherPropAlt template parameter to specify which one you want.
	Values of this parameter are defined in common.h, such as SOLVE_ENTHALPY
	and SOLVE_IENERGY, etc.

	See SteamProperty
*/
template<class MainProp, class OtherProp, class VaryProp, int MainPropAlt,int OtherPropAlt,int VaryPropAlt>
class SolverBase : public DesignByContract
{

	// The following kills Borland C Builder 5.5. I don't know if it can
	// be worked around or not. I tried :-)
	// friend class ZeroIn<SolverBase,VaryProp,OtherProp>;

	protected:
		/// Create the solver and assign fixed and target properties
		SolverBase(const MainProp &mp,const OtherProp &op){
			//cerr << "Running SolverBase creator" << endl;
			this->mp=mp;
			this->op=op;
		}

		virtual ~SolverBase(){}

	public:

		virtual void setLowerBound(const VaryProp &lowerbound){
			this->lowerbound = lowerbound;
		}
		virtual void setUpperBound(const VaryProp &upperbound){
			this->upperbound = upperbound;
		}


		/// Set the target property
		void setTarget(const OtherProp &op){
			this->op=op;
		}

		/// Set the 'fixed' parameter in the root finding
		void setMainProp(const MainProp &mp){
			this->mp=mp;
		}

		/// Peform the iterations
		/**
			@param maxerror Seek value of OtherProp to within this maximum error
			@param tol Stop iterating when steps of VaryProp are less than this
			@return SteamCalculator containing state of steam with the desired conditions of (MainProp,OtherProp)
		*/
		SteamCalculator
		solve(
			OtherProp   maxerror
			,VaryProp   tol
		){
			ZeroIn<SolverBase,OtherProp,VaryProp> z;

			//cerr << endl << "Solving for "<< SteamProperty<OtherProp,OtherPropAlt>::name() << " = " << op << " by varying " << SteamProperty<VaryProp,VaryPropAlt>::name() << ", and with " << SteamProperty<MainProp,MainPropAlt>::name() << " fixed at " << mp << endl;

			try{

				//S.set_pT(P_CRIT,T);

				z.setLowerBound(lowerbound);
				z.setUpperBound(upperbound);
				z.setTolerance(tol);

				z.setMethod(&SolverBase::getError_vp);

				z.visit(this);

				if(!z.isSolved(maxerror)){
					std::stringstream s;
					s.flags(std::ios_base::showbase);
					s << "Failed solution: target "<< SteamProperty<OtherProp,OtherPropAlt>::name() << " = " << op << ", with " << SteamProperty<MainProp,MainPropAlt>::name() << " fixed at " << mp << std::endl;

					s << " (error was " << z.getError() << ", max allowed is " << maxerror << ")";
					throw std::runtime_error(s.str());
				}

				setVaryProp(z.getSolution());

				return S;
			}catch(std::exception &e){
				std::stringstream s;
				s << "Solver<" << SteamProperty<MainProp,MainPropAlt>::name() << "," << SteamProperty<OtherProp,OtherPropAlt>::name() << "," << SteamProperty<VaryProp,VaryPropAlt>::name() << ">::solve: " << e.what();
				throw std::runtime_error(s.str());
			}
		}
	protected:

		virtual void setVaryProp(const VaryProp &vp) = 0;

		OtherProp getError_vp(const VaryProp &vp){
			//cerr << endl << "Solver::getError_vp: " << SteamProperty<VaryProp,VaryPropAlt>::name() << " = " << vp;

			setVaryProp(vp);

			//cerr << " -> " << SteamProperty<OtherProp,OtherPropAlt>::name() << " = " << SteamProperty<OtherProp,OtherPropAlt>::get(S) ;

			return SteamProperty<OtherProp,OtherPropAlt>::get(S) - op;
		}

	protected:

		OtherProp op;
		MainProp mp;
		SteamCalculator S;
		VaryProp lowerbound;
		VaryProp upperbound;
};

/// Single-property steam solver
/**
	This class will solve for (temperature or pressure) plus another steam property, by varying (pressure or temperature) until the desired value of the other steam property is found.

	@example
		To find p such that (T=600K, u=1500 kJ/kg):

@code
	Solver<Temperature,SpecificEnergy,Pressure,0,SOLVE_ENERGY> PS1(600 * Kelvin,1500 * kJ_kg);
	SteamCalculator S = PS1.solve(0.00001 * kJ_kg, 0.1 * Pascal);
	cout << S.pres() << end;
@endcode

	Note that dimensionally some properties are equivalent, for example
	specific internal energy and specific enthalpy. In these cases you need
	to use the OtherPropAlt template parameter to specify which one you want.
	Values of this parameter are defined in common.h, such as SOLVE_ENTHALPY
	and SOLVE_IENERGY, etc.

	@param MainProp is the one of the correlation properties. In other words it can be fed directly into the known correlation equations.

	@param OtherProp is the quantity to be solved for. In other words it is not correlated for and must be solved for by varying VaryProp.

	@param VaryProp is the quantity which will be varied to in order to home in on the desired value of OtherProp.

	@param OtherPropAlt is the 'alt code' for the other (target) property. You will
	need to set it to SOLVE_ENTHALPY, SOLVE_ENTROPY, etc, becuase dimensional
	equivalence means that this C++ template can't distinguish between SpecificEnergy
	and SpecificEnthalpy (as far as the Units<...> class goes, these are the
	same thing).
*/
template<class MainProp, class OtherProp, class VaryProp, int MainPropAlt=0,int OtherPropAlt=0,int VaryPropAlt=0>
class Solver
	: public SolverBase<MainProp,OtherProp,VaryProp,MainPropAlt,OtherPropAlt,VaryPropAlt>
{
	public:

		Solver(const MainProp &mp,const OtherProp &op)
			: SolverBase<MainProp,OtherProp,VaryProp,MainPropAlt,OtherPropAlt,VaryPropAlt>(mp,op){
			// not implemented
			throw std::runtime_error("Not implemented");
		}


};

/*

// Solving for specific volume given temperature
class Solver<Temperature,SpecificVolume,Pressure,0,0,0>
	: public SolverBase<Temperature,SpecificVolume,Pressure,0,0,0>
{
	public:

		Solver(Temperature T,SpecificVolume v, bool isSuperheated=false)
			: SolverBase<Temperature,SpecificVolume,Pressure,0,0,0>(T,v){


			if(T > T_CRIT){
				lowerbound = P_MIN;
				upperbound = P_MAX;
			}else{
				if(isSuperheated){
					lowerbound = Boundaries::getSatPres_T(T);
					upperbound = P_MAX;
				}else{
					lowerbound = P_MIN;
					upperbound = Boundaries::getSatPres_T(T);
				}
			}
		}

	protected:

		virtual void setVaryProp(const Pressure &p){
			S.set_pT(p,mp);
		}

};

*/

/// Temperature solver
/**
	This solver solves for a given value of the OtherProp property (template parameter) by varying pressure, with a prescribed fixed value of temperature

	@example
		Example: what is the pressure of water if the specific volume is 0.00102 m3/kg, and the temperature is 623.15 K (350°C)?

	@todo
		Give complete example

*/
template<class OtherProp, int OtherPropAlt>
class Solver<Temperature,OtherProp,Pressure,0,OtherPropAlt,0>
	: public SolverBase<Temperature,OtherProp,Pressure,0,OtherPropAlt,0>
{
	public:

		Solver(Temperature T,OtherProp op)
			: SolverBase<Temperature,OtherProp,Pressure,0,OtherPropAlt,0>(T,op){
			SolverBase<Temperature,OtherProp,Pressure,0,OtherPropAlt,0>::lowerbound = P_TRIPLE;
			SolverBase<Temperature,OtherProp,Pressure,0,OtherPropAlt,0>::upperbound = P_MAX;
		}

	protected:

		virtual void setVaryProp(const Pressure &p){
			SolverBase<Temperature,OtherProp,Pressure,0,OtherPropAlt,0>::S.set_pT(p,SolverBase<Temperature,OtherProp,Pressure,0,OtherPropAlt,0>::mp);
		}
};

/// Pressure solver
/**
	This solver solves for a given value of the OtherProp property (template parameter) by varying temperature, with a prescribed fixed value of pressure
*/
template<class OtherProp, int OtherPropAlt>
class Solver<Pressure,OtherProp,Temperature,0,OtherPropAlt,0>
	: public SolverBase<Pressure,OtherProp,Temperature,0,OtherPropAlt,0>
{
	public:

		Solver(Pressure p, OtherProp op)
			: SolverBase<Pressure,OtherProp,Temperature,0,OtherPropAlt,0>(p,op){
			//std::cerr << "Varying Temperature!" << std::endl;
			SolverBase<Pressure,OtherProp,Temperature,0,OtherPropAlt,0>::lowerbound = T_MIN;
			SolverBase<Pressure,OtherProp,Temperature,0,OtherPropAlt,0>::upperbound = T_MAX;
		}

	protected:

		virtual void setVaryProp(const Temperature &T){
			//std::cerr << "Setting temperature to " << T;
			SolverBase<Pressure,OtherProp,Temperature,0,OtherPropAlt,0>::S.set_pT(SolverBase<Pressure,OtherProp,Temperature,0,OtherPropAlt,0>::mp,T);
			//std::cerr << "Gives h = " << SolverBase<Pressure,OtherProp,Temperature,0,OtherPropAlt,0>::S.specenthalpy() << std::endl;
		}
};

#endif
