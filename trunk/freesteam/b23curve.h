#ifndef SATCURVE_H
#define SATCURVE_H

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
	
		virtual Ordinate solve(Abscissa target){}

	protected:
	
		B23CurveBase(){}
				
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
	
		Ordinate solve(const Abscissa &target){

			ZeroIn<B23Curve,Abscissa,Temperature> z;
			Boundaries B;

			try{
				
				//cerr << endl << "Trying to solve for Abscissa = " << target << endl;

				this->target=target;
				this->state=state;

				// Always try to solve to accuracy of 0.001% of the target value:
				Abscissa maxerror = target * 0.001 * Percent;

				z.setLowerBound(TB_LOW + 0.01 * Kelvin);
				z.setUpperBound(TB_HIGH);
				z.setTolerance(0.00001 * Kelvin);

				z.setMethod(&B23Curve::getAbscissaError_T);

				z.visit(this);

				if(!z.isSolved(maxerror)){	
					stringstream s;
					s.flags(ios_base::showbase);
					s << "In B23Curve::solve, unable to solve for target " << target << " (error was " << z.getError() << ", max allowed is " << maxerror << ")";
					throw new Exception(s.str());
				}

				return getOrdinate(S);

			}catch(Exception *e){
				stringstream s;
				s << "B23Curve::solve caught: " << e->what();
				throw new Exception(s.str());
			}	
		}
		
	private:
	
		Abscissa getAbscissaError_T(Temperature T){
			
			//cerr << "T = " << T;
			
			Pressure p_bound = Boundaries::getpbound_T(T);
			S.set_pT(p_bound,T);
			
			//cerr << " --> Abscissa = " << getAbscissa(S) << endl;
			
			return getAbscissa(S) - target;
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

		virtual Ordinate solve(const Temperature &T){
			Pressure p_bound = Boundaries::getpbound_T(T);
			SteamCalculator S;
			S.set_pT(p_bound,T);
			return getOrdinate(S);
		}
};

/// B23Curve for case where Abscissa is pressure
/**
	Use T_sat(p) directly then return Ordinate(p,T_sat) - i.e. using the Region 4 backward equation
*/
template<class Ordinate,int OrdinateAlternative>
class B23Curve<Ordinate,Pressure,OrdinateAlternative,0> 
	: public B23CurveBase<Ordinate,Pressure,OrdinateAlternative,0>{
	
	public:
	
		B23Curve() : B23CurveBase<Ordinate,Pressure,OrdinateAlternative,0>(){}

		virtual Ordinate solve(const Pressure &p){
			Temperature T_bound = Boundaries::getTbound_p(p);
			SteamCalculator S;
			S.set_pT(p,T_bound);
			return getOrdinate(S);
		}
};

#endif
