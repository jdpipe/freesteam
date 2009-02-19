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
	
	Pressure pmin = 20.3 * MPa;
	Pressure pmax = 20.72 * MPa;
	Pressure pstep = (pmax - pmin) / double(n);
	
	SteamCalculator S;
	
	char tab = '\t';
	
	cout.flags(ios_base::showbase);
	cout << scientific << setprecision(8);
	
	cout << "T = [" << endl;
	for(Temperature T=Tmin; T <= Tmax; T+=Tstep){
		cout << tab << T/Kelvin;
	}
	cout << endl << "];" << endl;

	cout << "p = [" << endl;
	for(Pressure p=pmin; p < pmax; p+=pstep){
		cout << tab << p/MPa;
	}
	cout << endl << "];" << endl;
	
	cout << "v = [" << endl;
	
	for(Temperature T=Tmin; T <= Tmax; T+=Tstep){
		for(Pressure p=pmin; p < pmax; p+=pstep){
			try{
			
				S.set_pT(p,T);
				cout << tab << S.specvol()/(metre3/kilogram);
			
			}catch(Exception *E){
				cerr << "Exception: " << E->what() << endl;
				cout << tab << "0";
			}
		}
		cout << endl;
	}
	cout << "];" << endl;
	
	cout << "contourf(p,T,v);" << endl;
	cout << "xlabel('pressure / MPa');ylabel('temperature / K');zlabel('specific volume / (kg/m³)');" << endl;
	
}
