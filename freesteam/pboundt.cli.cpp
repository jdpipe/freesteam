#include "steamcalculator.h"
#include "exception.h"
#include "units.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main(void){
	
	int n=200;
	
	Temperature Tmin = 652.0 * Kelvin;
	Temperature Tmax = 654.0 * Kelvin;
	Temperature Tstep = (Tmax - Tmin) / double(n);

	SteamCalculator S;
	
	char tab = '\t';
	
	cout.flags(ios_base::showbase);
	cout << scientific << setprecision(8);
	
	cout << "T = [" << endl;
	for(Temperature T=Tmin; T <= Tmax; T+=Tstep){
		cout << tab << T/Kelvin;
	}
	cout << endl << "];" << endl;

	cout << "pbound = [" << endl;
	
	for(Temperature T=Tmin; T <= Tmax; T+=Tstep){
		try{

			Pressure p = Boundaries::getpbound_T(T);
			cout << tab << p/MPa;

		}catch(Exception *E){
			cerr << "Exception: " << E->what() << endl;
			cout << tab << "0";
		}

		cout << endl;
	}
	cout << "];" << endl;
	
	cout << "plot(pbound,T);" << endl;
	cout << "xlabel('pressure / MPa');ylabel('temperature / K');" << endl;
	
}
