#include "steamcalculator.h"
#include "exception.h"
#include "batchtest.h"
#include "solver.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
using namespace std;

#define TSOLVE2_COMPARE(VAR) \
	if(!eq(S.temp(), T, maxerror)){ \
		stringstream s; \
		s << "Failed to solve " << #VAR << " = " << VAR << ", p = " << p << ": expected T = " << T/Kelvin<< "K but got " << S.temp()/Kelvin<< "K (error was " << fabs(S.temp() - T) << ", max is " << maxerror << ")"; \
		CPPUNIT_FAIL(s.str()); \
	}

class TemperatureSolverTest2Point{
	
	private:
		Pressure p;
		Temperature T;
	
	public:
		
		TemperatureSolverTest2Point(Pressure p,Temperature T){
			this->p=p;
			this->T=T;
		}
	
		void test(double tol) const{
			SteamCalculator S97;
			Water95 S95;

			try{
				// Get the properties at p,T
				SteamCalculator S;
				S.set_pT(p,T);
				SpecificEnergy u = S.specienergy();
				SpecificEntropy s = S.specentropy();
				SpecificVolume v = S.specvol();

				if(
					p/MPa >= 20.0
					&& tocelsius(T) >= 325.0 
					&& tocelsius(T) <= 375.0
				){
					//cerr << "(SKIPPED)" << endl;
					return;
				}
				
				//cerr << "p = " << p/MPa << "MPa, T = " << T << " (" << tocelsius(T) << "°C; region " << S.whichRegion() <<")" << endl;
				
				Temperature maxerror = 5e-5 * Kelvin;
				
				//Solving for T(u,p):
				Solver<Pressure,SpecificEnergy,Temperature> US(p,u);
				S = US.solve(0.00001 * kJ_kg, 1e-6 * Kelvin);				
				TSOLVE2_COMPARE(u);

				//Solving for T(s,p):
				Solver<Pressure,SpecificEntropy,Temperature> SS(p,s);
				S = SS.solve(0.00001 * kJ_kgK, 1e-6 * Kelvin);				
				TSOLVE2_COMPARE(s);

				//Solving for T(v,p):
				Solver<Pressure,SpecificVolume,Temperature> VS(p,v);
				S = VS.solve(0.00001 * metre3 / kilogram, 1e-8 * Kelvin);				
				TSOLVE2_COMPARE(v);
				
			}catch(Exception *e){
				stringstream s;
				s.flags(ios_base::showbase);
				s << "Exception caught: " << e->what();
				s << "(expected value of T was " << T << " = " << tocelsius(T) << "°C)";
				CPPUNIT_FAIL(s.str());
			}
		}
};

class TemperatureSolverTest2 : public BatchTest<TemperatureSolverTest2Point>{
	
	public:
		
		void setUp(){
			
			tol = 0;
			
			//cerr << endl << "(relative) tolerance is " << tol <<endl;
			
			const int T_COUNT=22;
			
			const int P_COUNT=29;
			const double p_raw[P_COUNT]={0.1, 0.5, 1, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20, 22.5, 25, 27.5, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};

			//const int P_COUNT=10;
			//const double p_raw[P_COUNT]={5, 6, 7, 8, 9, 10, 12.5, 13, 14, 15};

			const double T_raw[T_COUNT]={10, 25, 50, 75, 100, 150, 200, 250, 300, 350, 375, 400, 425, 450, 475, 500, 550, 600, 650, 700, 750, 800};
			
			for(int it=0; it<T_COUNT; it++){
				for(int ip=0;ip<P_COUNT; ip++){
					
					if(p_raw[ip]==0.1)continue;

					addTestPoint(TemperatureSolverTest2Point(p_raw[ip] * MPa,fromcelsius(T_raw[it])));

				}
			}	
		}
		
		void tearDown(){
			cerr << " (Skipped T/°C in [325,375], p/MPa >=20)";
		}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(TemperatureSolverTest2);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

};
		
CPPUNIT_TEST_SUITE_REGISTRATION(TemperatureSolverTest2);
