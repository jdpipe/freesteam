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
		s << "At p = " << p/MPa << "MPa, T = " << T << " (rho = " << rho << "),"; \
		s << "values of " << #val97 << " and " << #val95 << " disagreed: "; \
		s << #val97 << " = " << val97 << ", " << #val95 << " = " << val95 << ", tol = "<< tol/Percent << "%"; \
		s << " - actual error is " << (val95-val97)/val97/Percent << "%."; \
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
				
				//cerr << "  p = " << p / MPa << " MPa, T = " << T << endl;
				
				// First get a rough value of rho...
				
				if(Boundaries::isRegion3_pT(p,T)){
					//cerr << "Ignoring region 3 point, p = " << p/MPa << "MPa, T = " << T << endl;
					return;
				}
				
				S97.set_pT(p,T);
				Density rho=S97.dens();
				
				// Use the IAPWS-95 correlation with rho and T to get p...
				
				Pressure p95 = S95.p(T/Kelvin, rho/kg_m3) * MPa;
				
				if(p95 > P_MAX){
					p95 = P_MAX;
				}
				
				// Now, re-set the IAPWS-IF97 values

				if(Boundaries::isRegion3_pT(p95,T)){
					//cerr << "Ignoring region 3 point, p = " << p << ", T = " << T << endl;
					return;
				}
				
				S97.set_pT(p95,T);
				Pressure p97 = S97.pres();
				
				// Compare specific volumes:
				
				SpecificVolume v95 = 1.0/rho;
				SpecificVolume v97 = S97.specvol();
				IAPWS9795_COMPARE(v97,v95, 0.3 * Percent);
				
				SpecificEnergy u97 = S97.specienergy();
				SpecificEnergy u95 = S95.u(T/Kelvin, rho/kg_m3) * kJ_kg;
				IAPWS9795_COMPARE(u97,u95, 0.4 * Percent);				
				
				SpecificEnergy h97 = S97.specenthalpy();
				SpecificEnergy h95 = S95.h(T/Kelvin, rho/kg_m3) * kJ_kg;
				IAPWS9795_COMPARE(h97,h95, 0.4 * Percent);

				SpecificEntropy s97 = S97.specentropy();
				SpecificEntropy s95 = S95.s(T/Kelvin, rho/kg_m3) * kJ_kgK;
				IAPWS9795_COMPARE(s97,s95, 0.4 * Percent);

				SpecHeatCap cp97 = S97.speccp();
				SpecHeatCap cp95 = S95.cp(T/Kelvin, rho/kg_m3) * kJ_kgK;
				IAPWS9795_COMPARE(cp97,cp95, 0.45 * Percent);

				SpecHeatCap cv97 = S97.speccv();
				SpecHeatCap cv95 = S95.cv(T/Kelvin, rho/kg_m3) * kJ_kgK;
				IAPWS9795_COMPARE(cv97,cv95, 0.45 * Percent);
				
			}catch(Exception *e){
				CPPUNIT_FAIL(e->what());
			}
		}
};

class IAPWS9795Test : public BatchTest<IAPWS9795TestPoint>{
	
	public:
		
		void setUp(){
			
			tol = 0;
			
			//cerr << endl << "(relative) tolerance is " << tol <<endl;
			
			const int P_COUNT=29;
			const int T_COUNT=22;
			
			const double p_raw[P_COUNT]={0.1, 0.5, 1, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20, 22.5, 25, 27.5, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};

			const double T_raw[T_COUNT]={10, 25, 50, 75, 100, 150, 200, 250, 300, 350, 375, 400, 425, 450, 475, 500, 550, 600, 650, 700, 750, 800};
			
			for(int it=0; it<T_COUNT; it++){
				for(int ip=0;ip<P_COUNT; ip++){
					
					if(p_raw[ip]==0.1)continue;

					addTestPoint(IAPWS9795TestPoint(p_raw[ip] * MPa,fromcelsius(T_raw[it])));

				}
			}	
		}
		
		void tearDown(){
			cerr << " (Note: didn't test points lying in region 3!)";
		}

	public:

		// Invoke CPPUNIT macros to add tests to a suite:

		CPPUNIT_TEST_SUITE(IAPWS9795Test);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

};
		
CPPUNIT_TEST_SUITE_REGISTRATION(IAPWS9795Test);
