#include "steamcalculator.h"
#include "batchtest.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include <cmath>
#include <vector>
#include <sstream>

using namespace std;



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



class Region3RefDataTest : public BatchTest<SteamTestPointR3>{

	private:

		SteamCalculator * S;
		double tol;

		void testOnePoint(SteamTestPointR3 p) {
			
			cerr << endl;
			
			cerr << "Testing R3 point T = " << p.temp << ", rho = " << p.rho << endl;

			S->set_pT(p.pres, p.temp);

			CPPUNIT_ASSERT(S->whichRegion() == 3);

			CHECK_RESULT(pres, p.pres);
			CHECK_RESULT(specenthalpy, p.h);
			CHECK_RESULT(specienergy, p.u);
			CHECK_RESULT(specentropy, p.s);
			CHECK_RESULT(speccp, p.cp);

		}

	public:

		CPPUNIT_TEST_SUITE(Region3RefDataTest);
		CPPUNIT_TEST(testAllPoints);
		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp() {

			S = new SteamCalculator();

			tol = 1e-30;

			data.
			push_back(SteamTestPointR3
			          (650.00000000, 500.0000, 0.255837018E2,
			           0.186343019E4, 0.181226279E4, 0.405427273E1,
			           0.138935717E2, 0.502005554E3));

			data.
			push_back(SteamTestPointR3
			          (650.0000000, 200.00000000, 0.222930643E2,
			           0.237512401E4, 0.226365868E4, 0.485438792E1,
			           0.446579342E2, 0.383444594E3));
			data.
			push_back(SteamTestPointR3
			          (750, 500, 0.783095639E2, 0.225868845E4,
			           0.210206932E4, 0.446971906E1, 0.634165359E1,
			           0.760696041E3));

		}

		void tearDown() {

			delete S;

			data.clear();

		}

};

CPPUNIT_TEST_SUITE_REGISTRATION(Region3RefDataTest);
