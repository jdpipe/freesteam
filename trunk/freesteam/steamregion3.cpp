#include "steamcalculator.h"
#include "steamregion3.h"
#include "../zeroin/zeroin.h"
#include "steamcalculator_macros.h"

SteamRegion3 *SteamRegion3::_instance = 0;

SteamRegion3::SteamRegion3() {}

SteamState *SteamRegion3::Instance() {
	if (_instance == 0) {
		_instance = new SteamRegion3();
	}
	return _instance;
}

int SteamRegion3::getRegion() {
	return 3;
}


//------------------------------------------------------------------------------
// CORRELATION DATA

// NOTE: equations for region 2 are in (rho,T) not (p,T) and series do not use the first value of REGION3_N[i].
// (that's so that the equations can be more readily checked with those published)

#define REG3_COUNT 40

const int REGION3_I[REG3_COUNT] = {
                                      0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3,
                                      4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 8, 9, 9, 10, 10, 11
                                  };

const int REGION3_J[REG3_COUNT] = {
                                      0, 0, 1, 2, 7, 10, 12, 23, 2, 6, 15, 17, 0, 2, 6, 7, 22, 26, 0, 2, 4,
                                      16, 26, 0, 2, 4, 26, 1, 3, 26, 0, 2, 26, 2, 26, 2, 26, 0, 1, 26
                                  };

const Num REGION3_N[REG3_COUNT] = {
            0.10658070028513E+01, -0.15732845290239E+02, 0.20944396974307E+02,
            -0.76867707878716E+01, 0.26185947787954E+01, -0.28080781148620E+01, 0.12053369696517E+01,
            -0.84566812812502E-02, -0.12654315477714E+01, -0.11524407806681E+01, 0.88521043984318E+00,
            -0.64207765181607E+00, 0.38493460186671E+00, -0.85214708824206E+00, 0.48972281541877E+01,
            -0.30502617256965E+01, 0.39420536879154E-01, 0.12558408424308E+00, -0.27999329698710E+00,
            0.13899799569460E+01, -0.20189915023570E+01, -0.82147637173963E-02, -0.47596035734923E+00,
            0.43984074473500E-01, -0.44476435428739E+00, 0.90572070719733E+00, 0.70522450087967E+00,
            0.10770512626332E+00, -0.32913623258954E+00, -0.50871062041158E+00, -0.22175400873096E-01,
            0.94260751665092E-01, 0.16436278447961E+00, -0.13503372241348E-01, -0.14834345352472E-01,
            0.57922953628084E-03, 0.32308904703711E-02, 0.80964802996215E-04, -0.16557679795037E-03,
            -0.44923899061815E-04
        };

//-------------------------------------------------------------------------

#define LOOP_EVAL_REG3(FUNC,EXPR1,EXPR2) EXPR_LOOP_SUM_FUNC_OBJ(SteamRegion3,FUNC,EXPR1,EXPR2,1,REG3_COUNT)
#define EVAL_REG3(FUNC,EXPR) EXPR_FUNC_OBJ(SteamRegion3,FUNC,EXPR)

#include <cstdlib>
#include <cmath>
#include <cstdio>

void SteamRegion3::set_pT(SteamCalculator * c, Pressure p, Temperature T,
                          Num x) {

	SteamCalculator *c2;

	c->T = T;
	c->tau = REG3_TEMP_REF / T;

	c->reg3_target_pressure = p;

	Pressure pb = SteamRegionBoundaries::getpbound_T(T);
	c2 = new SteamCalculator();
	c2->set_pT(pb, T);
	Density rhomin = c2->dens();
	//fprintf(stderr,"set_pT rhomin %.5f\n",rhomin);

	ZeroIn < SteamCalculator, Pressure, Density > *z =
	    new ZeroIn < SteamCalculator, Pressure, Density > ();

	z->setLowerBound(c2->dens());
	z->setUpperBound(REG3_ZEROIN_DENS_MAX);
	z->setTolerance(REG3_ZEROIN_TOL);
	z->setMethod(&SteamCalculator::getRegion3PressureError);

	c->accept(z);

	if (!z->isSolved(0.00001)) {
		throw new Exception("Couldn't solve set_pT in reg3");
	}
	//my_zeroin(c,c2->dens(),REG3_ZEROIN_DENS_MAX,reg3_pres_err,REG3_ZEROIN_TOL);

	delete c2;

	c->x = x;
	c->isset = true;

	//cerr << "SteamRegion3::set_pT(p=" << p << ",T=" << T << ")"<< endl;

	ENSURE(c->whichRegion() == 3);

}

//----------------------------------------------------------------------
// PHYSICAL PROPERTIES FOR REGION 3

// In this region, need to overrride the following default methods defined in SteamState.

SpecificVolume
SteamRegion3::specvol(SteamCalculator *c){
	return 1 / c->rho;
}

Density
SteamRegion3::dens(SteamCalculator *c){
	return c->rho;
}

Pressure
SteamRegion3::pres(SteamCalculator *c){
	return c->rho * R * c->T * c->del * phidel(c);
}

SpecificEnergy
SteamRegion3::specienergy(SteamCalculator *c){
	return R * c->T * c->tau * phitau(c);
}

SpecificEntropy
SteamRegion3::specentropy(SteamCalculator *c){
	return R * (c->tau * phitau(c) - phi(c));
}

SpecificEnergy
SteamRegion3::specenthalpy(SteamCalculator *c){
	R * c->T * (c->tau * phitau(c) + c->del * phidel(c));
}

SpecHeatCap
SteamRegion3::speccp(SteamCalculator *c){
	return R * (-sq(c->tau) * phitautau(c) +
                      (pow
                       (c->del * phidel(c) -
                        c->del * c->tau * phideltau(c),
                        2) / (2 * c->del * phidel(c) +
                              sq(c->del) * phideldel(c))));
}

SpecHeatCap
SteamRegion3::speccv(SteamCalculator *c){
	R * (-sq(c->tau) * phitautau(c));
}


LOOP_EVAL_REG3(phi,
               REGION3_N[0] * log(c->del), +REGION3_N[i] * pow(c->del,
                       REGION3_I
                       [i]) *
               pow(c->tau, REGION3_J[i]));

LOOP_EVAL_REG3(phidel,
               REGION3_N[0] / c->del,
               +REGION3_N[i] * REGION3_I[i] * pow(c->del,
                                                  REGION3_I[i] -
                                                  1) * pow(c->tau,
                                                           REGION3_J[i]));

LOOP_EVAL_REG3(phideldel,
               -REGION3_N[0] / sq(c->del),
               +REGION3_N[i] * REGION3_I[i] * (REGION3_I[i] -
                                               1) * pow(c->del,
                                                        REGION3_I[i] -
                                                        2) * pow(c->tau,
                                                                 REGION3_J
                                                                 [i]));


LOOP_EVAL_REG3(phitau,
               0, +REGION3_N[i] * pow(c->del,
                                      REGION3_I[i]) * REGION3_J[i] *
               pow(c->tau, REGION3_J[i] - 1));

LOOP_EVAL_REG3(phitautau,
               0, +REGION3_N[i] * pow(c->del,
                                      REGION3_I[i]) * REGION3_J[i] *
               (REGION3_J[i] - 1) * pow(c->tau, REGION3_J[i] - 2));

LOOP_EVAL_REG3(phideltau,
               0, +REGION3_N[i] * REGION3_I[i] * pow(c->del,
                                                     REGION3_I[i] -
                                                     1) * REGION3_J[i] *
               pow(c->tau, REGION3_J[i] - 1));


// NOTE: factor of 1e6 removed.
//EVAL_REG3(pitau_iaps85, \
//      IAPS85_TEMP_REF * R * sq(c->del_iaps85()*IAPS85_DENS_REF) * (phidel(c) - (REG3_TEMP_REF / c->T) * phideltau(c)) / //(IAPS85_PRES_REF * REG3_DENS_REF) \
//);
Num SteamRegion3::pitau_iaps85(SteamCalculator * c) {
	Num pitau_iaps85 = 0;
	pitau_iaps85 =
	    (IAPS85_TEMP_REF * R * sq(c->del_iaps85() * IAPS85_DENS_REF) *
	     (phidel(c) -
	      (REG3_TEMP_REF / c->T) * phideltau(c)) / (IAPS85_PRES_REF *
	              REG3_DENS_REF));
	ENSURE(!isinf(pitau_iaps85));
	return pitau_iaps85;
}


// NOTE: factor of 1e6 removed.
Num
SteamRegion3::delpi_iaps85(SteamCalculator *c){
	return (IAPS85_PRES_REF * REG3_DENS_REF)
		/ (IAPS85_DENS_REF * c->del_iaps85() * IAPS85_DENS_REF * R * c->T * (
			2 * phidel(c) + (c->del_iaps85() * IAPS85_DENS_REF / REG3_DENS_REF) * phideldel(c)
		)
	);
}
