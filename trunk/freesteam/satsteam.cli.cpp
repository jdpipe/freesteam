#include "steamcalculator.h"
#include "exception.h"
#include "units.h"

#include <iostream>
#include <iomanip>
using namespace std;

int main(void){
	
	int n=1000;
		
	Temperature Tmin = T_TRIPLE;
	Temperature Tmax = T_CRIT;
	Temperature Tstep = (Tmax - Tmin) / double(n-1);

	SteamCalculator S;
	
	char tab = '\t';
	
	//cout.flags(ios_base::showbase);
	cout << scientific << setprecision(8);
	
	cout        << "T / K"       << tab << "p / MPa"     << tab << "u / kJ/kg";
	cout << tab << "h / kJ/kg"   << tab << "s / kJ/kgK"  << tab << "v / m3/kg";
	cout << tab << "cp / kJ/kgK" << tab << "cv / kJ/kgK" << endl;
	
	for(Temperature T=Tmin; T <= Tmax; T += Tstep){

		try{
			SteamCalculator S;

			S.setSatSteam_T(T);

			cout << T << tab << S.pres() << tab;

			SpecificEnergy u = S.specienergy();
			SpecificEnergy h = S.specenthalpy();
			SpecificEntropy s = S.specentropy();
			SpecificVolume v = S.specvol();
			SpecHeatCap cp = S.speccp();
			SpecHeatCap cv = S.speccv();
			
			cout <<                                                     u  ;
			cout << tab << h            << tab << s           << tab << v ;
			cout << tab << cp           << tab << cv          << endl;

		}catch(Exception *E){
			cout.flush();
			cerr << "Exception: " << E->what() << endl;
		}
		
	}
}


