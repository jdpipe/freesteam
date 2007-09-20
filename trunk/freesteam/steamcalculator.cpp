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

#include "config.h"

#include "steamcalculator.h"
#include "steamcalculator_macros.h"
#include "state.h"
#include "region1.h"
#include "region2.h"
#include "region3.h"
#include "region4.h"
#include "zeroin.h"
#include "units.h"
#include "isinfnan.h"
#include "surfacetension.h"

#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;

#define EVAL_STEAM(FUNC,EXPR) EXPR_FUNC(SteamCalculator,FUNC,EXPR)
#define EVAL_STEAM_OBJ(FUNC,EXPR) EXPR_FUNC_OBJ(SteamCalculator,FUNC,EXPR)

#define PROPERTYSOLVE_TOL 0.00001

//------------------------------------------------
// OBJECT CONSTRUCTION/DESTRUCTION/COPY

/// Constructor
SteamCalculator::SteamCalculator() {
	isset = false;
	gas = NULL;
	liq = NULL;
	p = -1.0 * MPa;
	T = -1.0 * Kelvin;
	x = -1.0;
}

/// Copy constructor
/**
	This function creates a new uninititalised SteamCalculator, or else
	clones an existing one.
*/
SteamCalculator::
SteamCalculator(const SteamCalculator & original):DesignByContract() {
	copy(original);
}

/// Copy function

void
SteamCalculator::copy(const SteamCalculator & original) {
	gas = NULL;
	liq = NULL;

	if (original.isset) {
		//fprintf(stderr,"Copying steamcalculator...\n");

		T = original.T;
		p = original.p;
		x = original.x;
		tau = original.tau;
		pi = original.pi;
		del = original.del;
		rho = original.rho;

		_state = original._state;

		if (whichRegion() == 4) {
			gas = new SteamCalculator(*(original.gas));
			liq = new SteamCalculator(*(original.liq));
		}
		isset = true;

	} else {
		p = -1.0 * Pressure();
		T = -1.0 * Temperature();
		isset = false;
	}
}

/// Assignment operator
SteamCalculator const &
SteamCalculator::operator=(SteamCalculator const &original) {
	if (this != &original) {
		destroy();
		copy(original);
	}
	return *this;
}

/// Destroy function
/**
	Shared by destructor and assignment operators
*/
void
SteamCalculator::destroy() {

	// if we did two-phase calcs, then get rid of the gas SteamCalculator
	if (this->gas != NULL) {
		//cerr << endl << "DESTROY GAS" << endl;
		delete this->gas;
	}
	if (this->liq != NULL) {
		//cerr << endl << "DESTROY LIQ" << endl;
		delete this->liq;
	}
}

/// Destructor
/*
	If saturated, this destroys the gas/liquid SteamCalculators used for the
	calculation of quality.
*/
SteamCalculator::~SteamCalculator() {
	destroy();
}

//------------------------------------------------
// SETTNG/CHANGING STEAM STATE

/// Update the '_state' object according to current steam state
/**
	This is the state/strategy design pattern, tweaked a bit for the case of Region 4...
*/
void
SteamCalculator::changeState(SteamState * state) {
	_state = state;
	isset = false;
	//cerr << "SteamCalculator region set to " << whichRegion() << endl;
}

/// Set the steam conditions by pressure, temperature and optionally quality.
/**
	@param p pressure
	@param T temperature
	@param x quality (only if saturated)
*/
void
SteamCalculator::set_pT(const Pressure &p, const Temperature &T, Num x) {

	try{

		isset = false;

		//REQUIRE(Boundaries::isValid_pT(p, T, true));

		/*
			Test first for saturation, because we need to have a bit of 'slop' there to allow for numerical inconsistencies of using the forwards/backwards saturation curve equation.
		*/

		if(p < P_MIN){
			throw std::runtime_error("SteamCalculator::set_pT: p < P_MIN");
		}

		//REQUIRE(p >= P_MIN);
		REQUIRE(p <= P_MAX);

		REQUIRE(T >= T_MIN);
		REQUIRE(T <= T_MAX);

		if(T <= TB_LOW){
			Pressure psat = Boundaries::getSatPres_T(T);
			if(p > psat){
				changeState(Region1::Instance());
			}else if(p < psat){
				changeState(Region2::Instance());
			}else{
				changeState(Region4::Instance());
			}
		}else{
			Pressure pbound =Boundaries::getpbound_T(T);
			if(p > pbound){
				if(T > T_CRIT){
					// Region 3, above T_CRIT
					changeState(Region3::Instance());
				}else{
					// Region 3, beneath T_CRIT
					setRegion3_rhoT(Boundaries::getSatDensSteam_T(T),T);
					Pressure psat = pres();
					if(p == psat){
						changeState(Region4::Instance());
					}
				}
			}else{
				changeState(Region2::Instance());
			}
		}

		ASSERT(x >= 0.0);
		ASSERT(x <= 1.0);

		_state->set_pT(*this,p,T,x);

		ENSURE(_state->getRegion() == whichRegion());

	}catch(std::exception &E){
		stringstream s;
		s << "SteamCalculator::set_pT(p = " << p << ",T = " << T <<"): " << E.what();
		throw std::runtime_error(s.str());
	}
}


//-------------------------------------
// Saturation curves

/// Set state to be saturated water at a specified pressure.
/*
	Sat water can be either region 1 or region 3

	@param p pressure
*/
void
SteamCalculator::setSatWater_p(const Pressure &p) {

#ifndef ENABLE_SAT_P
	throw std::runtime_error("SteamCalculator::setSatWater_p: disabled for numerical consistence on sat line");
#else

	Temperature T;

	REQUIRE(p <= P_CRIT);
	REQUIRE(p >= REG4_P_MIN);

	if (p == P_CRIT) {
		T = T_CRIT;
	} else {
		T = Boundaries::getSatTemp_p(p);
	}

	ASSERT(T >= T_MIN);
	ASSERT(T <= T_CRIT + EPS_T_CRIT);

	//ASSERT(Boundaries::isSat_pT(p, T));

	if(p > PB_LOW){
		changeState(Region3::Instance());
	}else{
		changeState(Region1::Instance());
	}

	_state->set_pT(*this, p, T, 0);
#endif

}

/// Set state to be saturated steam at a specified pressure.
/*	@param p pressure
*/
void
SteamCalculator::setSatSteam_p(const Pressure &p) {

#ifndef ENABLE_SAT_P
	throw std::runtime_error("SteamCalculator::setSatSteam_p: disabled for numerical consistence on sat line");
#else
	Temperature T;

	REQUIRE(p <= P_CRIT);
	REQUIRE(p >= REG4_P_MIN);

	if (p == P_CRIT) {
		//cerr << "CRITICAL POINT" << endl;
		T = T_CRIT;
	} else {
		T = Boundaries::getSatTemp_p(p);
	}

	ENSURE(T >= T_MIN);
	ENSURE(T <= T_CRIT + EPS_T_CRIT);
	//ASSERT(Boundaries::isSat_pT(p, T));

	changeState(Region2::Instance());
	_state->set_pT(*this, p, T, 1);
#endif
}

/// Set state to be saturated water at a specified temperature.
/*	@param T temperature
*/
void
SteamCalculator::setSatWater_T(const Temperature &T) {

	REQUIRE(T >= T_TRIPLE);
	REQUIRE(T <= T_CRIT);

	if(T <= TB_LOW){
		// Use region 1
		changeState(Region1::Instance());
		_state->set_pT(*this,Boundaries::getSatPres_T(T),T,0.0);
	}else{
		setRegion3_rhoT(Boundaries::getSatDensWater_T(T),T);
	}
}

/// Set state to be saturated steam at a specified temperature.
/*	@param T temperature
*/
void
SteamCalculator::setSatSteam_T(const Temperature &T) {

	REQUIRE(T >= T_TRIPLE);
	REQUIRE(T <= T_CRIT);

	if(T <= TB_LOW){
		// Use region 2
		Pressure p = Boundaries::getSatPres_T(T);
		changeState(Region2::Instance());
		_state->set_pT(*this, p, T, 1.0);
	}else{
		setRegion3_rhoT(Boundaries::getSatDensSteam_T(T),T);
	}
}

//---------------------------------------------------------------------------
// Region boundaries

/// Set properties on B23 curve given temperature
void
SteamCalculator::setB23_T(const Temperature &T){

	REQUIRE(T >= T_MIN);
	REQUIRE(T <= T_MAX);

	if(T < TB_LOW || T > TB_HIGH){
		stringstream s;
		s << "Invalid temperature in setB23_T, T = " << T;
		throw std::runtime_error(s.str());
	}

	Pressure p = Boundaries::getpbound_T(T);

	ASSERT(p >= PB_LOW - 0.001 * MPa);

	changeState(Region2::Instance());

	_state->set_pT(*this, p, T, 1);

	ENSURE(whichRegion() == 2);
}


/// Set properties on B23 curve given pressure
void
SteamCalculator::setB23_p(const Pressure &p){

	REQUIRE(p >= PB_LOW);
	REQUIRE(p <= P_MAX);

	if(p < PB_LOW){
		stringstream s;
		s << "Invalid pressure in setB23_p, p = " << p;
		throw std::runtime_error(s.str());
	}

	Temperature T = Boundaries::getTbound_p(p);

	ASSERT(T >= TB_LOW);
	ASSERT(T <= TB_HIGH);

	changeState(Region2::Instance());

	_state->set_pT(*this, p, T, 1);

	ENSURE(whichRegion() == 2);
}

//--------------------------------------------------------------------------
// Methods for saturated liquid/gas.

const SteamCalculator &
SteamCalculator::getLiquidPart() const{
	ASSERT(whichRegion()==4);
	IS_VALID(this);

	return *liq;
}

const SteamCalculator &
SteamCalculator::getGasPart() const{
	ASSERT(whichRegion()==4);
	IS_VALID(this);

	return *gas;
}

//--------------------------------------------------------------------------
// Overall status

/// Has the state of the SteamCalculator been initialised?
bool
SteamCalculator::isSet() const{
	return isset;
}

/// Used for design-by-contract IS_VALID tests:
bool
SteamCalculator::isValid() const{
	return isSet()
		&& Boundaries::isValid_pT(this->pres(), this->temp());
}

/// Get the region for the present SteamCalculator (1 to 4, according to IAPWS)
/*	@return steam region
*/
int
SteamCalculator::whichRegion(void) const{
	int r;
	/*
	   if(!isset){
	   cerr << "SteamCalculator is not set at SteamCalculator::whichRegion" << endl;
	   }
	 */

	r = _state->getRegion();
	//cerr << "Region is " << r << endl;
	return r;
}

/// Get the state for the present SteamCalculator
/* 	@return steam state, subcooled, superheated, saturated or supercritical, or unknown.
*/
SteamStateCode SteamCalculator::whichState(void){

	if (isset) {
		if(T <= TB_LOW){
			if(pres() == Boundaries::getSatPres_T(temp())){
				return STEAM_SATURATED;
			}
			switch(whichRegion()){
				case 1:
					return STEAM_SUBCOOLED;
				case 2:
					return STEAM_SUPERHEATED;
				default:
					throw std::runtime_error("Invalid state with T<TB_LOW");
			}
		}else{
			if(pres() > Boundaries::getpbound_T(temp())){
				//Region 3, check for saturation
				Density rho_water, rho_steam;

				switch(whichRegion()){
					case 4:
						return STEAM_SATURATED;
					case 3:
						if(temp() > T_CRIT){
							return STEAM_SUPERCRITICAL;
						}else if(temp() == T_CRIT){
							return STEAM_SATURATED;
						}

						rho_water = Boundaries::getSatDensWater_T(temp());
						rho_steam = Boundaries::getSatDensSteam_T(temp());

						if(dens() > rho_water){
							return STEAM_SUBCOOLED;
						}else if(dens() < rho_steam){
							return STEAM_SUPERHEATED;
						}else{
							return STEAM_SATURATED;
						}
					default:
						throw std::runtime_error("Invalid state with T>TB_LOW");
				}
			}else{
				return STEAM_SUPERHEATED;
			}
		}
		throw std::runtime_error("Did not determine state!");
	} else {
		throw std::runtime_error("State is not yet set");
	}
}

/// Get a string version of steam state
/*	@see whichState()
*/
const char *SteamCalculator::whichStateStr(void){
	SteamStateCode st = this->whichState();

	if (!isset) {
		throw std::runtime_error("State is not set at whichStateStr");
	}

	switch(st){
		case STEAM_SATURATED:
			if(temp()==T_CRIT){
				return "SATURATED";
			}

			if (this->x == 0.0) {
				return "SAT LIQUID";
			} else if (this->x == 1.0) {
				return "SAT STEAM";
			} else {
				return "SATURATED";
			}
		case STEAM_SUBCOOLED:
			return "SUBCOOLED";
		case STEAM_SUPERHEATED:
			return "SUPERHEATED";
		case STEAM_SUPERCRITICAL:
			return "SUPERCRITICAL";
		default:
			throw std::runtime_error("Invalid state!");
	}
}

//-----------------------------------------------------------------------
// GETTERS for steam properties

/// Temperature
/**
	Returns the absolute temperature at the state set.
*/
Temperature
SteamCalculator::temp(void) const{
	REQUIRE(isset);
	return _state->temp(*this);
}

/// Pressure
/**
	Returns the pressure at the state set.
*/
Pressure
SteamCalculator::pres(void) const{
	REQUIRE(isset);
	//fprintf(stderr,"About to evaluate SteamCalculator::pres...\n");
	return _state->pres(*this);
}

/// Density, eg kg/m³
Density
SteamCalculator::dens(void) const{
	REQUIRE(isset);
	Density rho = _state->dens(*this);
	ENSURE(!isnan(rho));
	return rho;
}

/// Specific volume, eg m³/kg
SpecificVolume
SteamCalculator::specvol(void) const{
	REQUIRE(isset);
	return _state->specvol(*this);
}

/// Specific internal energy, eg kJ/kg
SpecificEnergy
SteamCalculator::specienergy(void) const{
	REQUIRE(isset);
	//cerr << "About to calculate specienergy at p = " << pres() / bar << " bar, T = " << tocelsius(temp()) << "°C" << endl;
	SpecificEnergy u = _state->specienergy(*this);
	//ENSURE(u > 0.0 * kJ_kg);
	return u;
}

/// Specific entropy, eg kJ/kg
SpecificEntropy
SteamCalculator::specentropy(void) const{
	REQUIRE(isset);
	return _state->specentropy(*this);
}

/// Specific enthalpy, eg kJ/kg
SpecificEnergy
SteamCalculator::specenthalpy(void) const{
	REQUIRE(isset);
	return _state->specenthalpy(*this);
}

/// Specific isobaric hear capacity, eg kJ/kgK
SpecHeatCap
SteamCalculator::speccp(void) const{
	REQUIRE(isset);
	return _state->speccp(*this);
}

/// Specific isochoric heat capacity, eg kJ/kgK
SpecHeatCap
SteamCalculator::speccv(void) const{
	REQUIRE(isset);
	return _state->speccv(*this);
}

/// Steam quality (if saturated)
/**
	If saturated, returns the steam quality. If not saturated, returns 0.0 for region 1, 1.0 for region2, and
	for region 3, indeterminate, could be anything.

	@todo
		Check this, make sure only legal values are possible in region 3. There is a bit of confusion in the code about 'region 4 inside region 3'.
*/
Num
SteamCalculator::quality(void) const{
	REQUIRE(isset);
	return this->x;
}

// VISCOSITY and THERMAL CONDUCTIVITY are below

//------------------------------------------------------------------------
// CORRELATION DATA FOR VISCOSITY & THERMAL CONDUCTIVITY

// viscosity 

#define VISC_N0_COUNT 4

const Num VISC_N0[VISC_N0_COUNT] = { 1, 0.978197, 0.579829, -0.202354 };

#define VISC_COUNT 19

const Num VISC_I[VISC_COUNT] = {
	0, 1, 4, 5, 0
	, 1, 2, 3, 0, 1
	, 2, 0, 1, 2, 3
	, 0, 3, 1, 3 
};

const Num VISC_J[VISC_COUNT]= {
	0, 0, 0, 0, 1
	, 1, 1, 1, 2, 2
	, 2, 3, 3, 3, 3
	, 4, 4, 5, 6
};

const Num VISC_N[VISC_COUNT]
= { 0.5132047, 0.3205656, -0.7782567, 0.1885447, 0.2151778
	, 0.7317883, 1.241044 , 1.476783, -0.2818107, -1.070786
	, -1.263184, 0.1778064, 0.460504, 0.2340379, -0.4924179
	, -0.0417661, 0.1600435, -0.01578386, -0.003629481 
};

// conductivity (now updated to the 1998 revision of the IAPS-1985 release)

/// @see http://www.iapws.org/relguide/thcond.pdf#page=8
const Temperature IAPWS_THCOND_T_REF = 647.26 * Kelvin;
const Density IAPWS_THCOND_DENS_REF = 317.7 * kg_m3;
const ThermalConductivity IAPWS_THCON_REF = 1.0 * Watt / metre / Kelvin;

#define THCON_a_COUNT 4
const Num THCON_a[THCON_a_COUNT] = {
	0.0102811
	,0.0299621
	,0.0156146
	,-0.00422464
};


#define THCON_b0 -0.397070
#define THCON_b1 0.400302
#define THCON_b2 1.060000
#define THCON_B1 -0.171587
#define THCON_B2 2.392190

#define THCON_d1 0.0701309
#define THCON_d2 0.0118520
#define THCON_d3 0.00169937
#define THCON_d4 -1.0200

#define THCON_C1 0.642857
#define THCON_C2 -4.11717
#define THCON_C3 -6.17937
#define THCON_C4 0.00308976
#define THCON_C5 0.0822994
#define THCON_C6 10.0932

//-------------------------------------------------------------------------
// REDUCED QUANTITIES FOR IAPS85 / REVISED IAPWS CORRELATIONS

/// Reduced temperature for IAPWS conductivity calculation
/**
	Reference: IAPWS Revised Release on the IAPS Formulation 1985 for the Thermal Conductivity of Ordinary Water Substance, 1998, IAPWS.

	@see http://www.iapws.org/relguide/thcond.pdf#page=8, Eq. 4
*/
Num
SteamCalculator::tau_iaps85() const{
	return this->T / IAPS85_TEMP_REF;
}

/// Reduced density for IAPWS conductivity calculation
/**
	Reference: IAPWS Revised Release on the IAPS Formulation 1985 for the Thermal Conductivity of Ordinary Water Substance, 1998, IAPWS.

	@see http://www.iapws.org/relguide/thcond.pdf#page=8, Eq. 5

	Density calculated with IAPWS SF95 is expected, in order to meet the reference values given
*/
Num
SteamCalculator::del_iaps85() const{
	return  dens() / IAPS85_DENS_REF;
}

/// Reduced pressure for IAPWS conductivity calculation
/**
	Reference: IAPWS Revised Release on the IAPS Formulation 1985 for the Viscosity of Ordinary Water Substance, 2003, IAPWS.

	@see http://www.iapws.org/relguide/visc.pdf#page=7, Eq. 7
*/
Num
SteamCalculator::pi_iaps85() const{
	return this->p / IAPS85_PRES_REF;
}

//-------------------------------------------------------------------------
// VISCOSITY CALCULATIONS ACCORDING TO IAPS85 / REVISED IAPWS 1997

/// @see http://www.iapws.org/relguide/IF97.pdf
Num
SteamCalculator::mu0() const{

	Num e = 0;

	Num t = tau_iaps85();

	for (int i = 0; i < VISC_N0_COUNT; i++) {
		e += VISC_N0[i] / pow(t, i);
		//printf("NOTE: eta0 = %.5f\n",eta0);
	}
	return (sqrt(t) / e);

}

/// @see http://www.iapws.org/relguide/IF97.pdf
Num
SteamCalculator::mu1() const{

	Num e = 0;

	Num tt = 1 / tau_iaps85() - 1;
	//printf("NOTE: tt = %10.8e\n",tt);

	Num d = del_iaps85();
	Num dd = d - 1;
	//printf("NOTE: dd = %10.8e\n",dd);

	for (int i = 0; i < VISC_COUNT; i++) {
		e += VISC_N[i] * pow(tt, VISC_I[i]) * pow(dd, VISC_J[i]);
		//printf("NOTE: eta1 = %.5f\n",eta1);
	}

	return (exp(d * e));

}

/// @see http://www.iapws.org/relguide/IF97.pdf
Num
SteamCalculator::mu2() const{

	// "For industrial use, the value of mu2 can be taken as 1"
	// IAPWS Rel.. Viscosity 1997

	//return 1;

	Num tt = this->tau_iaps85();
	Num dd = this->del_iaps85();

	if (tt >= 0.9970 && dd >= 0.755 && tt < 1.0082 && dd < 1.290) {
		// Too close to the critical point - not implemented yet.

		Num ddpp = this->delpi_iaps85();
		Num chi = dd * ddpp;

		if (chi > 21.93) {
			return 0.922 * pow(chi, (Num) 0.0263);
		} else {
			return 1;
		}

	} else {
		return 1;
	}

}

/// @see http://www.iapws.org/relguide/IF97.pdf
Num
SteamCalculator::mu() const{
	return mu0() * mu1() * mu2();
}

/// Dynamic viscosity, mu, [µPa.s]
/**
	Returns the dynamic viscosity of water/steam. Assumes this->T in deg K.

	Range of validity is entire regions 1,2,3,4

	Reference: J Phys Chem Ref Data, vol 13, no 1, 1984, p. 175 ff.

	@return Dynamic viscosity [µPa.s]
	@see http://www.iapws.org/relguide/visc.pdf
	@see http://www.iapws.org/relguide/IF97.pdf
*/
DynamicViscosity
SteamCalculator::dynvisc() const{
	REQUIRE(isset);
	return (IAPS85_VISC_REF * mu());
}

/**
	Calculate the surface tension (sample units are N/m).
	@since 0.7
*/
SurfaceTension
SteamCalculator::surftens() const{
	REQUIRE(isset);
	REQUIRE(temp() <= T_CRIT);
	return surfaceTension(temp());
}

//----------------------------------------------------------------------
// THERMAL CONDUCTIVITY CALCULATION TO IAPS85 / REVISED IAPWS 1998

/// Conductivity [mW/m.K]
/**
	Returns the thermal conductivity of water/steam.
	@see http://www.iapws.org/relguide/thcond.pdf
	@see J Phys Chem Ref Data, vol 13, no 1, 1984, p. 175 ff.

	Range of validity is entire regions 1,2,3,4.

	@return Thermal conductivity [W/m.K]
*/
ThermalConductivity
SteamCalculator::conductivity() const{
	REQUIRE(isset);
	Num Tbar = this->T / IAPWS_THCOND_T_REF;
	Num rhobar = dens() / IAPWS_THCOND_DENS_REF;

	ThermalConductivity k = IAPWS_THCON_REF * lam(Tbar,rhobar);
	ENSURE(!isnan(k));
	return k;
}

/// Ideal-gas limit for thermal conductivity, lamda_0
/**
* Reference: IAPWS Revised Release on the IAPS Formulation 1985 for the Thermal
* Conductivity of Ordinary Water Substance, 1998, IAPWS.
*
* @see http://www.iapws.org/relguide/thcond.pdf
* @return glambda_0 [dim'less]
*/
Num
SteamCalculator::lam0(const Num &Tbar) const{

	Num acc = 0;
	for(int k = 0; k < THCON_a_COUNT; ++k) {
		acc += THCON_a[k] * pow(Tbar,k);
	}

	Num l0 = sqrt(Tbar) * acc;

	ENSURE(!isnan(l0));
	ENSURE(!isinf(l0));
	return l0;
}

/// Density adjustment for thermal conductivity
/**
* Reference: IAPWS Revised Release on the IAPS Formulation 1985 for the Thermal
* Conductivity of Ordinary Water Substance, 1998, IAPWS.
*
* @return lambda_0 [dim'less]
* @see http://www.iapws.org/relguide/IF97.pdf
*
*/
Num
SteamCalculator::lam1(const Num &rhobar) const{
	Num l = 0;

	Num l1 = THCON_b0 + THCON_b1 * rhobar + THCON_b2 * exp(THCON_B1 * sq(rhobar + THCON_B2));

	ENSURE(!isnan(l1));
	ENSURE(!isinf(l1));
	return l1;
}

/// Used in the calculation of reduced thermal conductivity
/**
* Reference: IAPWS Revised Release on the IAPS Formulation 1985 for the Thermal
* Conductivity of Ordinary Water Substance, 1998, IAPWS.
*
* Also, refer Bernhard Spang's IAPWS97 Excel Add-in, visual basic source code, version * 1.3.
*
* TODO: crossref with Sengers et al, J Phys Chem Ref Data v13 1984 pp893-933
*
* @return lambda_0 [dim'less]
* @see http://www.iapws.org/relguide/IF97.pdf
*/
Num
SteamCalculator::lam2(const Num &Tbar, const Num &rhobar) const{

	Num DTbar = fabs(Tbar - 1) + THCON_C4;

	Num Q = 2 + THCON_C5 / pow(DTbar,0.6);

	Num S;
	if(Tbar > 1){
		S = 1 / DTbar;
	}else{
		S = THCON_C6 / pow(DTbar,0.6);
	}

	Num l2 = 
		(THCON_d1 / pow(Tbar,10) + THCON_d2) * pow(rhobar,1.8) * 
			exp(THCON_C1 * (1 - pow(rhobar,2.8)))
		+ THCON_d3 * S * pow(rhobar,Q) *
			exp((Q/1+Q)*(1 - pow(rhobar,1+Q)))
		+ THCON_d4 *
			exp(THCON_C2 * pow(Tbar,1.5) + THCON_C3 / pow(rhobar,5));

	ENSURE(!isnan(l2));
	ENSURE(!isinf(l2));
	return l2;
}

Num
SteamCalculator::delpi_iaps85(void) const{
	return _state->delpi_iaps85(*this);
}

Num
SteamCalculator::pitau_iaps85(void) const{
	REQUIRE(isset);
	REQUIRE(this->_state!=NULL);

	//cerr << "Which state? " << whichState() << endl;
	//cerr << "Pressure:    " << this->pres() << endl;
	//cerr << "Temp:        " << this->temp() << endl;

	double pitau = _state->pitau_iaps85(*this);

	ENSURE(!isinf(pitau));
	ENSURE(!isnan(pitau));
	return pitau;
}

/// Reduced thermal conductivity
/**
* @return lambda_0 [dim'less]
* @see http://www.iapws.org/relguide/IF97.pdf
*/
//EVAL_STEAM(lam, lam0() * lam1() + lam2())

Num
SteamCalculator::lam(const Num &Tbar, const Num &rhobar) const{

	Num lam = (lam0(Tbar) + lam1(rhobar) + lam2(Tbar,rhobar));

	ENSURE(!isnan(lam));
	ENSURE(!isinf(lam));

	return lam;
}


Pressure
SteamCalculator::getRegion3PressureError(const Density &rho) {

	//cerr << endl << "SteamCalculator::getRegion3PressureError: T = " << T << ", rho = " << rho << ". ";

	ASSERT(whichRegion() == 3);
	ASSERT(rho <= REG3_ZEROIN_DENS_MAX);

	this->rho = rho;
	del = rho / REG3_DENS_REF;

	Pressure p = _state->pres(*this);

	if(p > P_MAX){
		p = P_MAX + (0.01 * MPa);	// keep pressure *almost* sensible.
	}

	ENSURE(p >= 0.0 * Pascal);
	ENSURE(p <= P_MAX + 0.01 * MPa);

	//cerr << "Found p = " << p / MPa << " MPa. ";

	return (p - reg3_target_pressure);
}

//---------------------------------------------------
// Non-checking routines for use with solvers etc

void
SteamCalculator::setRegion1_pT(const Pressure &p, const Temperature &T){
	ASSERT(Boundaries::isRegion1_pT(p,T));

	changeState(Region1::Instance());
	_state->set_pT(*this,p,T,0.0);

}

void
SteamCalculator::setRegion2_pT(const Pressure &p, const Temperature &T){
	ASSERT(Boundaries::isRegion2_pT(p,T));

	changeState(Region2::Instance());
	_state->set_pT(*this,p,T,0.0);

}

void
SteamCalculator::setRegion4_Tx(const Temperature &T, const Num &x){
	try{
		Boundaries::isSat_Tx(T,x, true);
		ASSERT(T>=T_TRIPLE);
		ASSERT(T<=T_CRIT);

		changeState(Region4::Instance());

		if(gas==NULL){
			gas = new SteamCalculator();
		}
		gas->setSatSteam_T(T);
		if(liq==NULL){
			liq = new SteamCalculator();
		}
		liq->setSatWater_T(T);

		this->x=x;
		this->T=T;
		isset=true;
	}catch(std::exception &E){
		stringstream s;
		s << "SteamCalculator::setRegion4_Tx: " << E.what();
		throw std::runtime_error(s.str());
	}
}

//--------------------------------------------------------------------------

/// Direct setting of rho,T for Region 3
/**
	@todo
		Ensure rho, T are valid for region 3
*/
void
SteamCalculator::setRegion3_rhoT(const Density &rho, const Temperature &T){

	REQUIRE(!isnan(rho));

	changeState(Region3::Instance());

	this->rho = rho;
	del = rho / REG3_DENS_REF;

	this->T = T;
	tau = REG3_TEMP_REF / T;

	if(T >= T_CRIT){
		x = -1;
	}else{
		if(rho > RHO_CRIT){
			x = 0;
		}else{
			x = 1;
		}
	}

	ENSURE(whichRegion()==3);

	isset = true;
}

