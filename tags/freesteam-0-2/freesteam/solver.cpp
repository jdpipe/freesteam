#include "solver.h"
#include "units.h"

#include <string>
using namespace std;

//----------------------------------
// PRESSURE SOLVER

#define PSOLVER_GETERROR(OTHERQUANTITY,METHOD) \
	OTHERQUANTITY \
	Solver<Temperature,OTHERQUANTITY,Pressure>::getError(Pressure p){ \
		S.set_pT(p,mq); \
		/* cerr << endl << "p = " << p/MPa << "MPa => " << #METHOD << " = " << S.METHOD() << "(region " << S.whichRegion()  << ")";*/ \
		return S.METHOD() - oq; \
	}

#define PSOLVER_MESSAGE(OTHERQUANTITY,METHOD) \
	string Solver<Temperature,OTHERQUANTITY,Pressure>::getCouldntSolveMessage(Temperature T,OTHERQUANTITY oq){ \
		stringstream s; \
		s << "Couldn't solve p(" << #METHOD << " = " << oq << ", T = " << T << ")"; \
		return s.str(); \
	} 

#define PSOLVER_BOUNDS(OTHERQUANTITY) \
	Pressure \
	Solver<Temperature,OTHERQUANTITY,Pressure>::getLowerBound(){ \
		return P_MIN + STEAM_P_EPS; \
	} \
	Pressure \
	Solver<Temperature,OTHERQUANTITY,Pressure>::getUpperBound(){ \
		return P_MAX; \
	}
	

PSOLVER_GETERROR(SpecificEnergy,specienergy);
PSOLVER_MESSAGE (SpecificEnergy,specienergy);
PSOLVER_BOUNDS (SpecificEnergy);

PSOLVER_GETERROR(SpecificEntropy,specentropy);
PSOLVER_MESSAGE(SpecificEntropy,specentropy);
PSOLVER_BOUNDS (SpecificEntropy);

PSOLVER_GETERROR(SpecificVolume,specvol);
PSOLVER_MESSAGE(SpecificVolume,specvol);
PSOLVER_BOUNDS (SpecificVolume);

//----------------------------------
// TEMPERATURE SOLVER

#define TSOLVER_GETERROR(OTHERQUANTITY,METHOD) \
	OTHERQUANTITY \
	Solver<Pressure,OTHERQUANTITY,Temperature>::getError(Temperature T){ \
		S.set_pT(mq,T); \
		/*cerr << endl << "T = " << T/Kelvin << "K => " << #METHOD << " = " << S.METHOD() << "(region " << S.whichRegion()  << ")";*/ \
		return S.METHOD() - oq; \
	}

#define TSOLVER_MESSAGE(OTHERQUANTITY,METHOD) \
	string Solver<Pressure,OTHERQUANTITY,Temperature>::getCouldntSolveMessage(Pressure p,OTHERQUANTITY oq){ \
		stringstream s; \
		s << "Couldn't solve T(" << #METHOD << " = " << oq << ", p = " << p << ")"; \
		return s.str(); \
	} 

#define TSOLVER_BOUNDS(OTHERQUANTITY) \
	Temperature \
	Solver<Pressure,OTHERQUANTITY,Temperature>::getLowerBound(){ \
		return T_MIN; \
	} \
	Temperature \
	Solver<Pressure,OTHERQUANTITY,Temperature>::getUpperBound(){ \
		return T_MAX; \
	}
	
TSOLVER_GETERROR(SpecificEnergy,specienergy);
TSOLVER_MESSAGE (SpecificEnergy,specienergy);
TSOLVER_BOUNDS (SpecificEnergy);

TSOLVER_GETERROR(SpecificEntropy,specentropy);
TSOLVER_MESSAGE(SpecificEntropy,specentropy);
TSOLVER_BOUNDS (SpecificEntropy);

TSOLVER_GETERROR(SpecificVolume,specvol);
TSOLVER_MESSAGE(SpecificVolume,specvol);
TSOLVER_BOUNDS (SpecificVolume);

