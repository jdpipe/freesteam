#ifndef SOLVER_H
#define SOLVER_H

#include "steamcalculator.h"
#include "zeroin.h"
#include "exception.h"
#include <string>
using namespace std;

/**
	This class will solve for 
	

	temperature | or plus another variable in SteamCalculator, by varying | pressure
	pressure    |                                                         | temperature
	
	@example
		To find p such that (T=600K, u=1500 kJ/kg):
		
		@code
			Solver<Temperature,SpecificEnergy,Pressure> PS1(600 * Kelvin,1500 * kJ_kg);
			SteamCalculator S = PS1.solve(0.00001 * kJ_kg, 0.1 * Pascal);
			cout << S.pres() << end;
		@endcode	
*/
template<class MainQuantity, class OtherQuantity, class VaryingQuantity>
class Solver : public DesignByContract {
	
	friend class ZeroIn<Solver,VaryingQuantity,OtherQuantity>;
	
	public:

		Solver(MainQuantity mq,OtherQuantity oq){
			this->mq=mq;
			this->oq=oq;
		}
	
		void setTarget(OtherQuantity oq){
			this->oq=oq;
		}

		void setMainQuantity(MainQuantity mq){
			this->mq=mq;
		}
		
		VaryingQuantity getLowerBound();
		VaryingQuantity getUpperBound();
		
		/// Peform the iterations
		/**
			@param maxerror Seek value of OtherQuantity to within this maximum error
			@param tol Stop iterating when steps of VaryingQuantity are less than this
			@return SteamCalculator containing state of steam with the desired conditions of (MainQuantity,OtherQuantity)
		*/
		SteamCalculator
		solve(
			OtherQuantity 		maxerror
			,VaryingQuantity	tol
		){
			ZeroIn<Solver,OtherQuantity,VaryingQuantity> z;
			
			//cerr << endl << "Solving for otherquantity = " << q << " with T = " << T << "(" << tocelsius(T) << "°C)";
			
			try{
			
				//S.set_pT(P_CRIT,T);
				
				z.setLowerBound(getLowerBound());
				z.setUpperBound(getUpperBound());
				z.setTolerance(tol);

				z.setMethod(&Solver::getError);
				
				z.visit(this);

				if(!z.isSolved(maxerror)){	
					stringstream s;
					s.flags(ios_base::showbase);
					s << "In Solver::solve, " << getCouldntSolveMessage(mq,oq);
					s << " (error was " << z.getError() << ", max allowed is " << maxerror << ")";
					throw new Exception(s.str());
				}

				return S;
			}catch(Exception *e){
				stringstream s;
				s << "Solver::solve caught: " << e->what();
				throw new Exception(s.str());
			}
		}
		
	protected:
	
		OtherQuantity oq;
		MainQuantity mq;
		SteamCalculator S;

		virtual OtherQuantity getError(VaryingQuantity);
		virtual string getCouldntSolveMessage(MainQuantity mq,OtherQuantity oq);
};

#endif
