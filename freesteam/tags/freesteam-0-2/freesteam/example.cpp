#include "steamcalculator.h"
#include "solver.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main(){
  SteamCalculator S, S2;

  // turn on display of units
  cerr.flags(ios_base::showbase);
  
  // initialise T1, p1, p2
  Temperature T1 = fromcelsius(200.0);
  Pressure p1 = 5.0 * bar;
  Pressure p2 = 10.0 * bar;  

  // part (1) - density at know pressure and temperature
  S.set_pT(p1, T1);
  Density rho = S.dens();
  cerr << "rho = " << rho << endl;
  SpecificEntropy s1 = S.specentropy();
  cerr << "s1 = " << s1 << endl;
  
  // part (2) - isentropic compression
  Solver<Pressure,SpecificEntropy,Temperature> PS(p2, s1);
  S2 = PS.solve(0.0001 * kJ_kgK, 0.0001 * Kelvin);
  Temperature T2 = S2.temp();
  cerr << "T2 = " << T2;
  cerr << " (or " << tocelsius(T2) << "°C)" << endl;
}
