#ifndef B13CURVE_H
#define B13CURVE_H

#include "common.h"
#include "exception.h"
#include "units.h"
#include "zeroin.h"
#include "steamcalculator.h"
#include "steamproperty.h"
#include "solver.h"

#include <iostream>
using namespace std;

/// Region1-Region3 boundary solver (fixed temperature line) for arbitrary variables
/**

	@example
		To solve for the internal energy of steam on the Region1-Region3 boundary when v=0.00012 m3/kg, use:

		@code
			B13Curve<SpecificEnergy,SpecificVolume> C;
			SpecificEnergy u = C.solve(0.00012 * m3_kg);
		@endcode
*/
template<class Ordinate,class Abscissa,int OrdinateAlt=0, int AbscissaAlt=0>
class B13CurveBase{

	public:

		virtual Ordinate solve(Abscissa target){}

	protected:

		B13CurveBase(){}

		Ordinate getOrdinate(SteamCalculator S){
			return SteamProperty<Ordinate,OrdinateAlt>::get(S);
		}

		Abscissa getAbscissa(SteamCalculator S){
			return SteamProperty<Abscissa,AbscissaAlt>::get(S);
		}

};

/// B13 Curve for case where Abscissa is not pressure
/**
	Use a root solving process to first find p where Abscissa(p_b13(T),T) = target
*/
template<class Ordinate,class Abscissa,int OrdinateAlt=0, int AbscissaAlt=0>
class B13Curve : public B13CurveBase<Ordinate,Abscissa,OrdinateAlt,AbscissaAlt>{

	// ZeroIn will be finding the TEMPERATURE at which B13Curve's ABSCISSA has the desired value:
	friend class ZeroIn<SteamCalculator,Temperature,Abscissa>;

	public:

		Ordinate solve(const Abscissa &target){

			try{
				Abscissa maxerr = target * 1e-12;

				Solver<Temperature,Abscissa,Pressure> PS1(T_REG1_REG3,target);
				SteamCalculator S = PS1.solve(maxerr, 0.001 * Pascal);

				//cout << S.pres() << endl;

				return B13CurveBase<Ordinate,Abscissa,OrdinateAlt,AbscissaAlt>::getOrdinate(S);

			}catch(Exception *e){
				stringstream s;
				s << "B13Curve<" << SteamProperty<Ordinate,OrdinateAlt>::name() << "," << SteamProperty<Abscissa,AbscissaAlt>::name() << ">::solve(" << SteamProperty<Abscissa,AbscissaAlt>::name() << " = " << target <<"): " << e->what();
				throw new Exception(s.str());
			}
		}
};

/// B13Curve for case where Abscissa is pressure
/**
	Use Ordinate(p,T_REG1_REG3) directly in this case
*/
template<class Ordinate,int OrdinateAlt>
class B13Curve<Ordinate,Pressure,OrdinateAlt,0>
	: public B13CurveBase<Ordinate,Pressure,OrdinateAlt,0>{

	public:

		B13Curve() : B13CurveBase<Ordinate,Pressure,OrdinateAlt,0>(){}

		virtual Ordinate solve(const Pressure &p){
			SteamCalculator S;
			S.set_pT(p,T_REG1_REG3,0);
			return B13CurveBase<Ordinate,Pressure,OrdinateAlt,0>::getOrdinate(S);
		}
};

#endif

