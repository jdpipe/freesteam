/*

freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2005  John Pye

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "steamcalculator.h"
#include "region3.h"
#include "zeroin.h"
#include "steamcalculator_macros.h"
#include "isinfnan.h"

Region3 *Region3::_instance = 0;

Region3::Region3() {}

SteamState *Region3::Instance() {
	if (_instance == 0) {
		_instance = new Region3();
	}
	return _instance;
}

int Region3::getRegion() const{
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

#define LOOP_EVAL_REG3(FUNC,EXPR1,EXPR2) EXPR_LOOP_SUM_FUNC_OBJ(Region3,FUNC,EXPR1,EXPR2,1,REG3_COUNT)
#define EVAL_REG3(FUNC,EXPR) EXPR_FUNC_OBJ(Region3,FUNC,EXPR)

#include <cstdlib>
#include <cmath>
#include <cstdio>

/**
	I think this is how this is (vertical lines only approximately vertical)

@code
	^
	|
  p	|s~5.1                   |T~T13
	|                        |
	|          crit          |
	|           __pt         |		p = P_CRIT
	|       _-''  ''-_       |-------------
	|     .'          '.     |
	|   .'sat        sat'.   |
	|  - vap          liq -  |
	| /                    \ |
	|/                      \|
	+=================================>
	                             rho
@endcode
*/
void Region3::set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x) {

	//cerr << endl << "About to solve p = " << p / MPa << " MPa, T = " << T << " in Region3... ";
	ZeroIn<SteamCalculator, Pressure, Density> z;

	c.T = T;
	c.tau = REG3_TEMP_REF / T;

	c.reg3_target_pressure = p;

	Pressure pb = Boundaries::getpbound_T(T);

	//cerr << endl << "p_bound(T) = " << pb / MPa << " MPa. ";

	SteamCalculator S2;


	if(p > P_CRIT){
		//cerr << endl << "Region3::set_pT: above P_CRIT. ";

		// We are above the saturation pressure, clearly out in the open in region 3
		S2.set_pT(pb, T);

		//cerr << endl << "region(p_bount(T),T) is " << S2.whichRegion() << ". ";
		z.setLowerBound(S2.dens());
		z.setUpperBound(REG3_ZEROIN_DENS_MAX);
	}else{
		if(T < T_CRIT){

			//cerr << endl << "Region3::set_pT: p < P_CRIT, T < T_CRIT";

			z.setLowerBound(RHO_CRIT);
			z.setUpperBound(REG3_ZEROIN_DENS_MAX);

		}else{
			//cerr << endl << "Region3::set_pT: p < P_CRIT, T >= T_CRIT";

			// We are to the left of the saturation curve on the p-rho chart
			z.setUpperBound(RHO_CRIT);
			z.setLowerBound(0.0001 * kg_m3);
		}
	}

	//cerr << endl << "Lower bound is " << z.getLowerBound() << ". ";
	//cerr << endl << "Upper bound is " << z.getUpperBound() << ". ";

	z.setTolerance(REG3_ZEROIN_TOL);
	z.setMethod(&SteamCalculator::getRegion3PressureError);

	z.visit(&c);

	Pressure maxerror = 0.001 * Pascal;

	if (!z.isSolved(maxerror)) {
		stringstream s;
		s << "Region3::set_pT: Couldn't solve for p = " << p/MPa << "MPa, T = " << T;
		s << " (error was " << z.getError() << ", max allowed is " << maxerror << ")";
		throw std::runtime_error(s.str());
	}

	/**
		@todo: remove redundent 'x' from Region3::set_pT function?
	*/
	c.x = x;
	c.isset = true;

	ENSURE(c.whichRegion() == 3);

}

//----------------------------------------------------------------------
// INTERNALLY-HELD-VALUE PROPERTIES

Temperature
Region3::temp(const SteamCalculator &c) const{
	return c.T;
}

//----------------------------------------------------------------------
// PHYSICAL PROPERTIES FOR REGION 3

// In this region, need to overrride the following default methods defined in SteamState.

SpecificVolume
Region3::specvol(const SteamCalculator &c) const{
	return 1.0 / c.rho;
}

Density
Region3::dens(const SteamCalculator &c) const{
	return c.rho;
}

Pressure
Region3::pres(const SteamCalculator &c) const{
	return c.rho * R * c.T * c.del * phidel(c);
}

SpecificEnergy
Region3::specienergy(const SteamCalculator &c) const{
	return R * c.T * c.tau * phitau(c);
}

SpecificEntropy
Region3::specentropy(const SteamCalculator &c) const{
	return R * (c.tau * phitau(c) - phi(c));
}

SpecificEnergy
Region3::specenthalpy(const SteamCalculator &c) const{
	return R * c.T * (c.tau * phitau(c) + c.del * phidel(c));
}

SpecHeatCap
Region3::speccp(const SteamCalculator &c) const{
	return R * (
		-sq(c.tau) * phitautau(c)
		+ (
			pow (c.del * phidel(c) - c.del * c.tau * phideltau(c), 2)
			/ (2 * c.del * phidel(c) + sq(c.del) * phideldel(c))
		)
	);
}

SpecHeatCap
Region3::speccv(const SteamCalculator &c) const{
	return R * (-sq(c.tau) * phitautau(c));
}


LOOP_EVAL_REG3(phi
	, REGION3_N[0] * log(c.del)
	, +REGION3_N[i] * pow(c.del, REGION3_I [i]) * pow(c.tau, REGION3_J[i]));

LOOP_EVAL_REG3(phidel
	, REGION3_N[0] / c.del
	, +REGION3_N[i] * REGION3_I[i] * pow(c.del, REGION3_I[i] - 1) * pow(c.tau, REGION3_J[i]));

LOOP_EVAL_REG3(phideldel
	, -REGION3_N[0] / sq(c.del)
	, +REGION3_N[i] * REGION3_I[i] * (REGION3_I[i] - 1) * pow(c.del, REGION3_I[i] - 2) * pow(c.tau, REGION3_J [i])
);


LOOP_EVAL_REG3(phitau
	, 0
	, +REGION3_N[i] * pow(c.del, REGION3_I[i]) * REGION3_J[i] * pow(c.tau, REGION3_J[i] - 1)
);

LOOP_EVAL_REG3(phitautau
	, 0
	, +REGION3_N[i] * pow(c.del, REGION3_I[i]) * REGION3_J[i] * (REGION3_J[i] - 1) * pow(c.tau, REGION3_J[i] - 2)
);

LOOP_EVAL_REG3(phideltau
	, 0
	, +REGION3_N[i] * REGION3_I[i] * pow(c.del, REGION3_I[i] - 1) * REGION3_J[i] * pow(c.tau, REGION3_J[i] - 1)
);


/*/ NOTE: factor of 1e6 removed.
//EVAL_REG3(pitau_iaps85, \
//      IAPS85_TEMP_REF * R * sq(c.del_iaps85()*IAPS85_DENS_REF) * (phidel(c) - (REG3_TEMP_REF / c.T) * phideltau(c)) / //(IAPS85_PRES_REF * REG3_DENS_REF) \
//);
*/
Num Region3::pitau_iaps85(const SteamCalculator &c) const {
	Num pitau_iaps85 = 0;
	pitau_iaps85 =
	    (IAPS85_TEMP_REF * R * sq(c.del_iaps85() * IAPS85_DENS_REF) *
	     (phidel(c) -
	      (REG3_TEMP_REF / c.T) * phideltau(c)) / (IAPS85_PRES_REF *
	              REG3_DENS_REF));
	ENSURE(!isinf(pitau_iaps85));
	return pitau_iaps85;
}


// NOTE: factor of 1e6 removed.
Num
Region3::delpi_iaps85(const SteamCalculator &c) const{
	return (IAPS85_PRES_REF * REG3_DENS_REF)
		/ (IAPS85_DENS_REF * c.del_iaps85() * IAPS85_DENS_REF * R * c.T * (
			2 * phidel(c) + (c.del_iaps85() * IAPS85_DENS_REF / REG3_DENS_REF) * phideldel(c)
		)
	);
}
