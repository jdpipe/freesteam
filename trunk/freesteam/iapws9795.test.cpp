#include "steamcalculator.h"
#include "exception.h"
#include "batchtest.h"
#include "steamtestpoint.h"
#include "iapws95.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
using namespace std;

#define IAPWS9795_COMPARE(val97,val95,tol) \
	if(!eq(val97,val95,tol*val97)){ \
		stringstream s; \
		s << "Values of " << #val97 << " and " << #val95 << " disagreed. "; \
		s << #val97 << " = " << val97 << ", " << #val95 << " = " << val95 << ", tol = "<< tol << "."; \
		CPPUNIT_FAIL(s.str()); \
	}

class IAPWS9795TestPoint{
	
	private:
		Pressure p;
		Temperature T;
	
	public:
		
		IAPWS9795TestPoint(Pressure p,Temperature T){
			this->p=p;
			this->T=T;
		}
	
		void test(double tol) const{
			SteamCalculator S97;
			Water95 S95;

			try{
				cerr.flags(ios_base::showbase);
				
				cerr << "  p = " << p / MPa << " MPa, T = " << T << endl;
				
				S97.set_pT(p,T);
								
				Pressure p97 = S97.pres();
				Density rho=S97.dens();
				Pressure p95 = S95.p(T/Kelvin, rho/kg_m3) * kilo * Pascal;
				
				IAPWS9795_COMPARE(p97,p95,tol);
				
				/*
				Pressure p97= S97.pres();
				Pressure p95 = S95.p(T/Kelvin, rho/kg_m3) * kilo * Pascal;
				
				cerr << "p97 = " << p97/MPa << " MPa, p95 = " << p95/MPa << " MPa" << endl;
				
				Pressure abserr = fabs(p95 - p97);
				
				double relerr = abserr / p97;
				
				cerr << "rel error is " << relerr << ", tol is " << tol << endl;
				
				if(relerr > tol){
					cerr << "PRESSURE ERROR" << endl;
				}
				
				
				if(relerr > tol){
					CPPUNIT_FAIL("Pressure didn't match");
				}
				
				*/
				
				CPPUNIT_ASSERT(eq(p97, p95, tol*p97));
				
				SpecificEnergy u = S97.specienergy();
				CPPUNIT_ASSERT(eq(u, S95.u(T/Kelvin, rho/kg_m3) * kJ_kg, tol*u));

				SpecificEnergy h = S97.specenthalpy();
				CPPUNIT_ASSERT(eq(h, S95.h(T/Kelvin, rho/kg_m3) * kJ_kg, tol*h));

				SpecificEntropy s = S97.specentropy();
				CPPUNIT_ASSERT(eq(s, S95.s(T/Kelvin, rho/kg_m3) * kJ_kgK, tol*s));

				SpecHeatCap cp = S97.speccp();
				CPPUNIT_ASSERT(eq(cp, S95.cp(T/Kelvin, rho/kg_m3) * kJ_kgK, tol*cp));

				SpecHeatCap cv = S97.speccv();
				CPPUNIT_ASSERT(eq(cv, S95.cv(T/Kelvin, rho/kg_m3) * kJ_kgK, tol*cv));
				
			}catch(SteamCalculatorException *e){
				CPPUNIT_FAIL(e->what());
			}catch(Exception *e){
				CPPUNIT_FAIL(e->what());
			}
		}
};

class IAPWS9795Test : public BatchTest<IAPWS9795TestPoint>{
	
	public:
		
		void setUp(){
			
			tol = 2.0 * Percent;
			
			cerr << endl << "(relative) tolerance is " << tol <<endl;
			
			const int P_COUNT=29;
			const int T_COUNT=22;
			
			const double p_raw[P_COUNT]={0.1, 0.5, 1, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20, 22.5, 25, 27.5, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};

			const double T_raw[T_COUNT]={10, 25, 50, 75, 100, 150, 200, 250, 300, 350, 375, 400, 425, 450, 475, 500, 550, 600, 650, 700, 750, 800};
			
			for(int it=0; it<T_COUNT; it++){
				for(int ip=0;ip<P_COUNT; ip++){

					addTestPoint(IAPWS9795TestPoint(p_raw[ip] * MPa,fromcelsius(T_raw[it])));

				}
			}	
		}
		
		void tearDown(){
		}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(IAPWS9795Test);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

};
		
CPPUNIT_TEST_SUITE_REGISTRATION(IAPWS9795Test);
