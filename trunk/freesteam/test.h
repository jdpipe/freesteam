#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "units.h"

#define CHECK_RESULT(method,val) if(!eq(S->method(),val,tol)){\
	stringstream s; \
	s<< #method << ": expected " << val << ", got " << S->method() << "."; \
	CPPUNIT_FAIL(s.str()); \
	}

template<class TestPoint> 
class BatchTest : public CppUnit::TestFixture {

	typedef typename vector<TestPoint>::iterator TestPointIterator;
	
	protected:
		vector < TestPoint > data;

		double tol;

		void testAllPoints() {

			for (TestPointIterator i = data.begin(); i < data.end(); ++i) {
				this->testOnePoint(*i);
			}
		}

		virtual void testOnePoint(TestPoint& t) {
		//CPPUNIT_FAIL("'testOnePoint' not implemented.");
		}

		void setTolerance(double tol) {
			this->tol = tol;
		}

		void addTestPoint(TestPoint p) {
			data.push_back(p);
		}

	public:
		virtual void setUp(){
			throw new Exception("'setUp' not implemented");
		}
		
		virtual void tearDown(){
			throw new Exception("'tearDown' not implemented");
		}
	
};

class SteamTestPoint {

	public:

		// Easy initialiser for values of known units
		SteamTestPoint(double t, double p, double v, double h, double u,
		               double s, double cp, double w) {

			this->temp = t * Kelvin;
			this->pres = p * MPa;
			this->v = v * metre3 / kilogram;
			this->h = h * kJ_kg;
			this->u = u * kJ_kg;
			this->s = s * kJ_kgK;
			this->cp = cp * kJ_kgK;
			this->w = w * metre / second;

		}
		Temperature temp;		// K
		Pressure pres;		// MPa
		SpecificVolume v;		// m³/kg
		SpecificEnergy h;		// kJ/kg
		SpecificEnergy u;		// kJ/kg
		SpecificEntropy s;		// kJ/kg.K
		SpecHeatCap cp;		///< // kJ/kg.K
		Velocity w;			///< speed of sound

};

/**
	In region 3, points are defined in terms of temp,density, so define them that way here
	
	NOTE this approach will introduce some errors since rootfinding is used to find the target pres value, rather than defining the given rho.
*/
class SteamTestPointR3 {

	public:

		/// Easy initialiser for values of known units
		SteamTestPointR3(double t, double rho, double p, double h, double u,
		                 double s, double cp, double w) {

			this->temp = t * Kelvin;
			this->rho = rho * kilogram / metre3;
			this->pres = p * MPa;
			this->h = h * kJ_kg;
			this->u = u * kJ_kg;
			this->s = s * kJ_kgK;
			this->cp = cp * kJ_kgK;
			this->w = w * metre / second;

		}
		Temperature temp;
		Density rho;
		Pressure pres;
		SpecificEnergy h;		///< enthalpy
		SpecificEnergy u;		///< internal energy
		SpecificEntropy s;
		SpecHeatCap cp;
		Velocity w;			///< speed of sound

};
