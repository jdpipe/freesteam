#include "units.h"
#include "steamproperty.h"
#include "b23curve.h"

#include <sstream>
#include <vector>
using namespace std;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class B23Point{

	protected:
	
		Temperature T;
		Pressure p;
		SpecificEnergy u;
		SpecificEnergy h;
		SpecificEntropy s;
		SpecificVolume v;
		SpecHeatCap cp;
		SpecHeatCap cv;

	public:
	
		void fill(Temperature T){
			this->T=T;
			SteamCalculator S;
			S.setB23_T(T);
			p = S.pres();
			
			//cerr << endl << "p = " << p << ", T = " << T << ": region is " << S.whichRegion() << ". ";
			
			h = S.specenthalpy();
			u = S.specienergy();
			v = S.specvol();
			s = S.specentropy();
			
			CPPUNIT_ASSERT(v > 0.0 * m3_kg);
		}

		void testUV(){
			//cerr << endl << "---- testUV: T = " << T << ", v = " << v << ", u = " << u << endl;
		
			B23Curve<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> Buv;
			SpecificEnergy u_solved = Buv.solve(v);

			if(fabs(u_solved - u)/u > 0.001 * Percent){
				stringstream s;
				s << "Expected u = " << u << ", got u_b23 = " << u_solved << " with v = " << v;
				CPPUNIT_FAIL(s.str());
			}
		}

		void testHT(){
			B23Curve<SpecificEnergy,Temperature,SOLVE_ENTHALPY,0> BhT;
			SpecificEnergy h_solved = BhT.solve(T);

			if(fabs(h_solved - h)/h > 0.001 * Percent){
				stringstream s;
				s << "Expected h = " << h << ", got h_b23 = " << h_solved << " with T = " << T;
				CPPUNIT_FAIL(s.str());
			}	
		}
};

/**
	Tests B23 curve solver for

			  T_b23(h)

		and   u_b23(v)
*/				
class B23CurveTest: public CppUnit::TestFixture{

	public:
		void tearDown() {
		}
		
		void setUp(){
		
			try{
			
				Tmin=TB_LOW;
				Tmax=TB_HIGH;
				for(Temperature T = Tmin; T < Tmax; T+=(Tmax - Tmin)/double(nsteps-1)){
					//cerr << endl << "B23CurveTest::setUp: T = " << T << "... ";
					B23Point p;
					p.fill(T);
					points.push_back(p);
				}
				
			}catch(Exception *E){
				stringstream s;
				s << "Initialisation error in setUp: " << E->what();
				CPPUNIT_FAIL(s.str());
			}
		}
	
	private:
		Temperature Tmin;
		Temperature Tmax;
		static const int nsteps = 10;
		
		vector<B23Point> points;
		
	protected:
		
		void testUV(){
			try{				
				for(int i=0; i<nsteps; i++){	
					B23Point &p = points.at(i);					
					p.testUV();					
				}			
			}catch(Exception *E){
				stringstream s;
				s << "B23CurveTest::testUV: " << E->what();
				CPPUNIT_FAIL(s.str());
			}	
		}	

		void testHT(){
			try{				
				for(int i=0; i<nsteps; i++){	
					B23Point &p = points.at(i);					
					p.testHT();					
				}			
			}catch(Exception *E){
				stringstream s;
				s << "B23CurveTest::testHT: " << E->what();
				CPPUNIT_FAIL(s.str());
			}	
		}	
	
	public:

		// Invoke CPPUNIT macros to add tests to a suite:
		CPPUNIT_TEST_SUITE(B23CurveTest);
		
		CPPUNIT_TEST(testHT);
		CPPUNIT_TEST(testUV);

		CPPUNIT_TEST_SUITE_END();
		
};

CPPUNIT_TEST_SUITE_REGISTRATION(B23CurveTest);
