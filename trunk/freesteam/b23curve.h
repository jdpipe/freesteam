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

#ifndef B23CURVE_H
#define B23CURVE_H

#include "common.h"
#include "exception.h"
#include "units.h"
#include "zeroin.h"
#include "steamcalculator.h"
#include "steamproperty.h"

/// Region2-Region3 boundary solver for arbitrary variables
/**
	This needs to be high-precision, hopefully comparable to the Boundaries::isSat_pT test.

	@example
		To solve for the internal energy of steam on the Region2-Region3 boundary when v=0.004 m3/kg, use:

		@code
			B23Curve<SpecificEnergy,SpecificVolume> C;
			SpecificEnergy u = C.solve(0.004 * m3_kg);
		@endcode

*/
template<class Ordinate,class Abscissa,int OrdinateAlternative=0, int AbscissaAlternative=0>
class B23CurveBase{

	public:

		virtual Ordinate solve(const Abscissa &target) = 0;

	protected:

		B23CurveBase(){}
		virtual ~B23CurveBase(){};

		Ordinate getOrdinate(SteamCalculator S){
			return SteamProperty<Ordinate,OrdinateAlternative>::get(S);
		}

		Abscissa getAbscissa(SteamCalculator S){
			return SteamProperty<Abscissa,AbscissaAlternative>::get(S);
		}

};

/// B23 Curve for case where Abscissa is neither temperature nor pressure
/**
	Use a root solving process to first find T where Abscissa(p_sat(T),T) = target
*/
template<class Ordinate,class Abscissa,int OrdinateAlternative=0, int AbscissaAlternative=0>
class B23Curve : public B23CurveBase<Ordinate,Abscissa,OrdinateAlternative,AbscissaAlternative>{

	// ZeroIn will be finding the TEMPERATURE at which B23Curve's ABSCISSA has the desired value:
	friend class ZeroIn<B23Curve,Temperature,Abscissa>;

	public:

		B23Curve(){
			// Intentionally empty
		}

		virtual ~B23Curve(){};

		Ordinate solve(const Abscissa &target){

			ZeroIn<B23Curve,Abscissa,Temperature> z;

			try{

				//cerr << endl << "B23Curve::solve: Trying to solve for " << 				SteamProperty<Abscissa,AbscissaAlternative>::name() << " = " << target << endl;

				this->target=target;
				this->state=state;

				// Always try to solve to accuracy of 0.001% of the target value:
				Abscissa maxerror = fabs(target) * 0.001 * Percent;

				z.setLowerBound(TB_LOW);
				z.setUpperBound(TB_HIGH);
				z.setTolerance(0.00001 * Kelvin);

				z.setMethod(&B23Curve::getAbscissaError_T);

				z.visit(this);

				if(!z.isSolved(maxerror)){
					stringstream s;
					s.flags(ios_base::showbase);
					s << "Unable to solve for target " << target << " (error was " << z.getError() << ", max allowed is " << maxerror << ")";
					throw Exception(s.str());
				}


				Temperature T = z.getSolution();
				//cerr << endl << "Solution found was T = " << T;
				S.setB23_T(T);
				//cerr << endl << "At which abscissa = " << getAbscissa(S) << endl;
				//cerr << endl << "Found solution in region " << S.whichRegion() << ", T = " << S.temp() << ", p = " << S.pres() << ". ";

				return B23CurveBase<Ordinate,Abscissa,OrdinateAlternative,AbscissaAlternative>::getOrdinate(S);

			}catch(Exception &e){
				stringstream s;
				s << "B23Curve::solve: " << e.what();
				throw Exception(s.str());
			}
		}

	private:

		Abscissa getAbscissaError_T(const Temperature &T){

			//cerr << "B23Curve::getAbscissaError_T: T = " << T;

			S.setB23_T(T);

			//cerr << " --> " << SteamProperty<Abscissa,AbscissaAlternative>::name() << " = " << getAbscissa(S) << endl;

			return B23CurveBase<Ordinate,Abscissa,OrdinateAlternative,AbscissaAlternative>::getAbscissa(S) - target;
		}

		SteamCalculator S;
		Abscissa target;
		bool state;

};

/// B23Curve for case where Abscissa is temperature
/**
	Use p_sat(T) directly then return Ordinate(p_sat,T)
*/
template<class Ordinate,int OrdinateAlternative>
class B23Curve<Ordinate,Temperature,OrdinateAlternative,0>
	: public B23CurveBase<Ordinate,Temperature,OrdinateAlternative,0>{

	public:

		B23Curve() : B23CurveBase<Ordinate,Temperature,OrdinateAlternative,0>(){}
		virtual ~B23Curve(){};

		virtual Ordinate solve(const Temperature &T){
			SteamCalculator S;
			S.setB23_T(T);
			return B23CurveBase<Ordinate,Temperature,OrdinateAlternative,0>::getOrdinate(S);
		}
};

/// B23Curve for case where Abscissa is pressure
/**
	Use T_sat(p) directly then return Ordinate(p,T_sat) - i.e. using the Region 4 backward equation
*/
/*
template<class Ordinate,int OrdinateAlternative>
class B23Curve<Ordinate,Pressure,OrdinateAlternative,0>
	: public B23CurveBase<Ordinate,Pressure,OrdinateAlternative,0>{

	public:

		B23Curve() : B23CurveBase<Ordinate,Pressure,OrdinateAlternative,0>(){}
		virtual ~B23Curve(){};

		virtual Ordinate solve(const Pressure &p){
			SteamCalculator S;
			S.setB23_p(p);
			return B23CurveBase<Ordinate,Pressure,OrdinateAlternative,0>::getOrdinate(S);
		}
};
*/
#endif
