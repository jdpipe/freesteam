#ifndef SOLVER_H
#define SOLVER_H

#include "steamcalculator.h"
#include "zeroin.h"
#include "exception.h"
#include <string>
using namespace std;

/**
	This class will solve for temperature plus another variable in SteamCalculator, by varying pressure
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
			this->pq=pq;
		}

		void setMainQuantity(MainQuantity p){
			this->mq=mq;
		}
		
		VaryingQuantity getLowerBound();
		VaryingQuantity getUpperBound();

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
