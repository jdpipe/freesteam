#include "steamcalculator.h"
#include "exception.h"
#include "units.h"

#include <iostream>
#include <iomanip>
using namespace std;

int main(void){
	
	int n=30;
	
	Temperature T = T_REG1_REG3;
	
	Pressure pmin = PB_LOW + 1.0 * bar;
	Pressure pmax = P_MAX;
	Pressure pstep = (pmax - pmin) / double(n);

	SteamCalculator S;
	
	char tab = '\t';
	
	cout.flags(ios_base::showbase);
	cout << scientific << setprecision(8);

	cout << "Min pressure = " << pmin << endl;
	cout << "Max pressure = " << pmax << endl;
	cout << "Step = " << pstep << endl;
	
	cout        << "T / K"       << tab << "p / MPa"     << tab << "u / kJ/kg";
	cout << tab << "h / kJ/kg"   << tab << "s / kJ/kgK"  << tab << "v / m3/kg";
	cout << tab << "cp / kJ/kgK" << tab << "cv / kJ/kgK" << endl;
	
	for(Pressure p=pmin; p <= pmax; p += pstep){
		cout << T << tab << p << tab;

		try{
			SteamCalculator S;

			S.set_pT(p,T);
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




















































