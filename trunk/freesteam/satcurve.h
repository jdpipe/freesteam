#ifndef SATCURVE_H
#define SATCURVE_H

#include "common.h"
#include "units.h"
#include "zeroin.h"
#include "steamcalculator.h"
#include "exception.h"
#include "steamproperty.h"

const bool SAT_WATER=0;
const bool SAT_STEAM=1;

/// Base class used to derive SatCurve
template<class Ordinate,class Abscissa,int OrdinateAlternative=0, int AbscissaAlternative=0>
class SatCurveBase{
	
	public:
	
		virtual Ordinate solve(Abscissa target, bool state=SAT_WATER){}

	protected:
	
		SatCurveBase(){}
				
		Ordinate getOrdinate(SteamCalculator S){
			return SteamProperty<Ordinate,OrdinateAlternative>::get(S);
		}
		
		Abscissa getAbscissa(SteamCalculator S){
			return SteamProperty<Abscissa,AbscissaAlternative>::get(S);
		}
		
};

/// Saturation curve solver for arbitrary variables
/**
	@example
		To solve for the internal energy of saturated steam at rho=3.6 kg/m3, use:
		
		@code
			SatCurve<SpecificEnergy,Density> C;
			SpecificEnergy u = C.solve(3.6 * kg_m3,SAT_STEAM);
		@endcode
		
*/template<class Ordinate,class Abscissa,int OrdinateAlternative=0, int AbscissaAlternative=0>
class SatCurve : public SatCurveBase<Ordinate,Abscissa,OrdinateAlternative,AbscissaAlternative>{

	// ZeroIn will be finding the TEMPERATURE at which SatCurve's ABSCISSA has the desired value:
	friend class ZeroIn<SatCurve,Temperature,Abscissa>;
	
	public:
	
		Ordinate solve(const Abscissa &target,bool state=SAT_WATER){

			ZeroIn<SatCurve,Abscissa,Temperature> z;
			Boundaries B;

			try{
				
				//cerr << endl << "Trying to solve for Abscissa = " << target << endl;

				this->target=target;
				this->state=state;

				// Always try to solve to accuracy of 0.001% of the target value:
				Abscissa maxerror = target * 0.001 * Percent;

				z.setLowerBound(T_TRIPLE);
				z.setUpperBound(T_CRIT);
				z.setTolerance(0.00001 * Kelvin);

				z.setMethod(&SatCurve::getAbscissaError_T);

				z.visit(this);

				if(!z.isSolved(maxerror)){	
					stringstream s;
					s.flags(ios_base::showbase);
					s << "In SatCurve::solve, unable to solve for target " << target << " (error was " << z.getError() << ", max allowed is " << maxerror << ")";
					throw new Exception(s.str());
				}

				return getOrdinate(S);

			}catch(Exception *e){
				stringstream s;
				s << "SatCurve::solve caught: " << e->what();
				throw new Exception(s.str());
			}	
		}
		
	private:
	
		Abscissa getAbscissaError_T(Temperature T){
			
			//cerr << "Trying with T = " << T << ": ";
			
			if(state==SAT_WATER){
				//cerr << "(water) ";
				S.setSatWater_T(T);
			}else if(state=SAT_STEAM){
				//cerr << "(steam) ";
				S.setSatSteam_T(T);
			}				
			
			//cerr << "Abscissa = " << getAbscissa(S) << endl;
			
			return getAbscissa(S) - target;
		}

		SteamCalculator S;
		Abscissa target;
		bool state;

};

/// SatCurve for case where Abscissa is temperature
/**
	Use p_sat(T) directly then return Ordinate(p_sat,T)
*/
template<class Ordinate,int OrdinateAlternative>
class SatCurve<Ordinate,Temperature,OrdinateAlternative,0> 
	: public SatCurveBase<Ordinate,Temperature,OrdinateAlternative,0>{
	
	public:
	
		SatCurve() : SatCurveBase<Ordinate,Temperature,OrdinateAlternative,0>(){}

		virtual Ordinate solve(const Temperature &T, bool state=SAT_WATER){
			SteamCalculator S;
			if(state==SAT_STEAM){
				S.setSatSteam_T(T);
			}else{
				S.setSatWater_T(T);
			}
			return getOrdinate(S);
		}

};

/// SatCurve for case where Abscissa is pressure
/**
	Use T_sat(p) directly then return Ordinate(p,T_sat) - i.e. using the Region 4 backward equation
*/
template<class Ordinate,int OrdinateAlternative>
class SatCurve<Ordinate,Pressure,OrdinateAlternative,0> 
	: public SatCurveBase<Ordinate,Pressure,OrdinateAlternative,0>{
	
	public:
	
		SatCurve() : SatCurveBase<Ordinate,Pressure,OrdinateAlternative,0>(){}

		virtual Ordinate solve(const Pressure &p,bool state=SAT_WATER){
			SteamCalculator S;
			if(state==SAT_STEAM){
				S.setSatSteam_p(p);
			}else{
				S.setSatWater_p(p);
			}
			return getOrdinate(S);
		}
};


#endif
