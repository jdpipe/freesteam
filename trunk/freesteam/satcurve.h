#ifndef SATCURVE_H
#define SATCURVE_H

#include "steamproperty.h"
#include "units.h"
#include "steamcalculator.h"
#include "common.h"
#include "zeroin.h"
#include "exception.h"

const int SAT_WATER=0;
const int SAT_STEAM=1;

const int SAT_LOW_TEMP=2;

/// Base class used to derive SatCurve
template<class Ordinate,class Abscissa,int OrdinateAlt=0, int AbscissaAlt=0>
class SatCurveBase{

	public:

		virtual Ordinate solve(const Abscissa &target, const int &flags=SAT_WATER) = 0;

	protected:

		SatCurveBase(){}
		virtual ~SatCurveBase(){}

		Ordinate getOrdinate(SteamCalculator S){
			return SteamProperty<Ordinate,OrdinateAlt>::get(S);
		}

		Abscissa getAbscissa(SteamCalculator S){
			return SteamProperty<Abscissa,AbscissaAlt>::get(S);
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

	@warning
		This function makes no allowance for expansion of water at temperatures close to zero, so you will get wrong temperatures in this region when solving for <xxx,v>
*/
template<class Ordinate,class Abscissa,int OrdinateAlt=0, int AbscissaAlt=0>
class SatCurve : public SatCurveBase<Ordinate,Abscissa,OrdinateAlt,AbscissaAlt>{

	// ZeroIn will be finding the TEMPERATURE at which SatCurve's ABSCISSA has the desired value:
	friend class ZeroIn<SatCurve,Temperature,Abscissa>;

	public:

		virtual ~SatCurve(){}

		static Temperature getLowerBound(const int &flags){
			/*
			if(flags & SAT_WATER
				&& !(flags & SAT_LOW_TEMP)
			){
				return T_TRIPLE;
			}
			*/
			return T_TRIPLE;
		}

		static Temperature getUpperBound(const int &flags){
			/*
			if(flags & SAT_WATER
				&& !(flags & SAT_LOW_TEMP)
			){
				return T_MIN_VOL;
			}
			*/
			return T_CRIT;
		}

		/**
			@param target Value of the Abscissa variable which is sought
			@param flags SAT_WATER or SAT_STEAM
			@return the value of the Ordinate variable for saturated steam/water where the Abscissa fulfil the target value.

			Note that this function will not solve for T < 3.984°C so you will get some errors there
		*/
		Ordinate solve(const Abscissa &target,const int &flags=SAT_WATER){

			ZeroIn<SatCurve,Abscissa,Temperature> z;

			try{

				//cerr << endl << "SatCurve<" << SteamProperty<Ordinate,OrdinateAlt>::name() << "," << SteamProperty<Abscissa,AbscissaAlt>::name() << ">::solve: Trying to solve for " << SteamProperty<Abscissa,AbscissaAlt>::name() << " = " << target;

				this->target=target;

				// Always try to solve to accuracy of 0.001% of the target value:
				Abscissa maxerror = fabs(target) * 0.002 * Percent;

				//z.setLowerBound(fromcelsius(3.984));
				z.setLowerBound(getLowerBound(flags));

				z.setUpperBound(getUpperBound(flags));
				//z.setUpperBound(fromcelsius(3.984));

				z.setTolerance(0.000001 * Kelvin);

				if(flags & SAT_STEAM){
					z.setMethod(&SatCurve::getAbscissaErrorSteam_T);
				}else{
					z.setMethod(&SatCurve::getAbscissaErrorWater_T);
				}

				z.visit(this);

				if(!z.isSolved(maxerror)){
					stringstream s;
					s.flags(ios_base::showbase);
					s << "Unable to solve for target " << SteamProperty<Abscissa,AbscissaAlt>::name() << " = " << target << " (error was " << z.getError() << ", max allowed is " << maxerror << ")";
					throw new Exception(s.str());
				}

				//cerr << endl << "SatCurve<" << SteamProperty<Ordinate,OrdinateAlt>::name() << "," << SteamProperty<Abscissa,AbscissaAlt>::name() << ">::solve: found solution at T = " << z.getSolution();

				if(flags & SAT_STEAM){
					S.setSatSteam_T(z.getSolution());
				}else{
					S.setSatWater_T(z.getSolution());
				}

				return SatCurveBase<Ordinate,Abscissa,OrdinateAlt,AbscissaAlt>::getOrdinate(S);

			}catch(Exception *e){
				stringstream s;
				s << "SatCurve<" << SteamProperty<Ordinate,OrdinateAlt>::name() << "," << SteamProperty<Abscissa,AbscissaAlt>::name() << ">::solve: " << e->what();
				throw new Exception(s.str());
			}
		}

	private:

		Abscissa getAbscissaErrorWater_T(const Temperature &T){

			//cerr << endl << "SatCurve::getAbscissaErrorWater_T: Trying with T = " << T << ": ";

			S.setSatWater_T(T);
			//cerr << SteamProperty<Abscissa,AbscissaAlt>::name() << " = " << getAbscissa(S);

			return SatCurveBase<Ordinate,Abscissa,OrdinateAlt,AbscissaAlt>::getAbscissa(S) - target;
		}

		Abscissa getAbscissaErrorSteam_T(const Temperature &T){

			//cerr << "SatCurve::getAbscissaErrorSteam_T: Trying with T = " << T << ": ";

			S.setSatSteam_T(T);
			//cerr << SteamProperty<Abscissa,AbscissaAlt>::name() << " = " << getAbscissa(S);

			return SatCurveBase<Ordinate,Abscissa,OrdinateAlt,AbscissaAlt>::getAbscissa(S) - target;
		}

		SteamCalculator S;
		Abscissa target;
};

/// SatCurve for case where Abscissa is temperature
/**
	Use p_sat(T) directly then return Ordinate(p_sat,T)
*/
template<class Ordinate,int OrdinateAlt>
class SatCurve<Ordinate,Temperature,OrdinateAlt,0>
	: public SatCurveBase<Ordinate,Temperature,OrdinateAlt,0>{

	public:

		SatCurve() : SatCurveBase<Ordinate,Temperature,OrdinateAlt,0>(){}
		virtual ~SatCurve(){}

		virtual Ordinate solve(const Temperature &T, const int &flags=SAT_WATER){
			SteamCalculator S;
			if(flags & SAT_STEAM){
				S.setSatSteam_T(T);
			}else{
				S.setSatWater_T(T);
			}
			return SatCurveBase<Ordinate,Temperature,OrdinateAlt,0>::getOrdinate(S);
		}

};

/// SatCurve for case where Abscissa is pressure
/**
	Use T_sat(p) directly then return Ordinate(p,T_sat) - i.e. using the Region 4 backward equation

	DISABLED because setSatSteam_p and setSatWater_p are inconsistent with satSatXxxxx_T
*/
/*
template<class Ordinate,int OrdinateAlt>
class SatCurve<Ordinate,Pressure,OrdinateAlt,0>
	: public SatCurveBase<Ordinate,Pressure,OrdinateAlt,0>{

	public:

		SatCurve() : SatCurveBase<Ordinate,Pressure,OrdinateAlt,0>(){}
		virtual ~SatCurve(){}

		virtual Ordinate solve(const Pressure &p,const int &flags=SAT_WATER){
			SteamCalculator S;
			if(flags & SAT_STEAM){
				S.setSatSteam_p(p);
			}else{
				S.setSatWater_p(p);
			}
			return SatCurveBase<Ordinate,Pressure,OrdinateAlt,0>::getOrdinate(S);
		}
};
*/

#endif
