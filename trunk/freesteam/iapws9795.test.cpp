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
				
				cerr << "  p = " << p << ", T = " << T << endl;
				
				S97.set_pT(p,T);

				Density rho  = S97.dens();
				CPPUNIT_ASSERT(eq(p, S95.p(T/Kelvin, rho/kg_m3) * MPa, tol*p));

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
			}catch(Exception *e){
				CPPUNIT_FAIL(e->what());
			}catch(...){
				CPPUNIT_FAIL("Unknown exception in IAPWS9795TestPoint::test");
			}
		}
};

class IAPWS9795Test : public BatchTest<IAPWS9795TestPoint>{
	
	public:
		
		void setUp(){
						
			tol = 0.05 * Percent;
			
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
