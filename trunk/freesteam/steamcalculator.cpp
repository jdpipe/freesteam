#include "steamcalculator.h"
#include "steamcalculator_macros.h"
#include "state.h"
#include "region1.h"
#include "region2.h"
#include "region3.h"
#include "region4.h"
#include "zeroin.h"
#include "units.h"

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
			throw new Exception("SteamCalculator::set_pT: p < P_MIN");
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

	}catch(Exception *E){
		stringstream s;
		s << "SteamCalculator::set_pT(p = " << p << ",T = " << T <<"): " << E->what();
		throw new Exception(s.str());
	}
}


//-------------------------------------

/// Set state to be saturated water at a specified pressure.
/*
	Sat water can be either region 1 or region 3

	@param p pressure
*/
void
SteamCalculator::setSatWater_p(const Pressure &p) {

	throw new Exception("SteamCalculator::setSatWater_p: disabled for numerical consistence on sat line");

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
}

/// Set state to be saturated steam at a specified pressure.
/*	@param p pressure
*/
void
SteamCalculator::setSatSteam_p(const Pressure &p) {

	throw new Exception("SteamCalculator::setSatSteam_p: disabled for numerical consistence on sat line");

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

/// Set properties on B23 curve given temperature
void
SteamCalculator::setB23_T(const Temperature &T){

	REQUIRE(T >= T_MIN);
	REQUIRE(T <= T_MAX);

	if(T < TB_LOW || T > TB_HIGH){
		stringstream s;
		s << "Invalid temperature in setB23_T, T = " << T;
		throw new Exception(s.str());
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
		throw new Exception(s.str());
	}

	Temperature T = Boundaries::getTbound_p(p);

	ASSERT(T >= TB_LOW);
	ASSERT(T <= TB_HIGH);

	changeState(Region2::Instance());

	_state->set_pT(*this, p, T, 1);

	ENSURE(whichRegion() == 2);
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

	ENSURE(whichRegion()==3);

	isset = true;
}

//--------------------------------------------------------------------------
// Methods for saturated liquid/gas.

/// Has the state of the SteamCalculator been initialised?
bool
SteamCalculator::isSet() const{
	return isset;
}

#ifndef NDEBUG
/// Used for design-by-contract IS_VALID tests:
bool
SteamCalculator::isValid() const{
	return isSet()
		&& Boundaries::isValid_pT(this->pres(), this->temp());
}
#endif

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
					throw new Exception("Invalid state with T<TB_LOW");
			}
		}else{
			if(pres() > Boundaries::getpbound_T(temp())){
				//Region 3, check for saturation
				switch(whichRegion()){
					case 4:
						return STEAM_SATURATED;
					case 3:
						return STEAM_SUPERCRITICAL;
					default:
						throw new Exception("Invalid state with T>TB_LOW");
				}
			}
		}
	} else {
		throw new Exception("State is not yet set");
	}
}

/// Get a string version of steam state
/*	@see whichState()
*/
const char *SteamCalculator::whichStateStr(void){
	SteamStateCode st = this->whichState();

	if (!isset) {
		throw new Exception("State is not set at whichStateStr");
	}

	switch(st){
		case STEAM_SATURATED:
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
			throw new Exception("Invalid state!");
	}
}

//-----------------------------------------------------------------------
// GETTERS for steam properties

/// Temperature [K]
Temperature
SteamCalculator::temp(void) const{
	REQUIRE(isset);
	return _state->temp(*this);
}

/// Pressure [MPa]
Pressure
SteamCalculator::pres(void) const{
	REQUIRE(isset);
	//fprintf(stderr,"About to evaluate SteamCalculator::pres...\n");
	return _state->pres(*this);
}

/// Density [kg/m³]
Density
SteamCalculator::dens(void) const{
	REQUIRE(isset);
	Density rho = _state->dens(*this);
	ENSURE(!isnan(rho));
	return rho;
}

/// Specific volume [m³/kg]
SpecificVolume
SteamCalculator::specvol(void) const{
	REQUIRE(isset);
	return _state->specvol(*this);
}

/// Specific internal energy [kJ/kg]
SpecificEnergy
SteamCalculator::specienergy(void) const{
	REQUIRE(isset);
	//cerr << "About to calculate specienergy at p = " << pres() / bar << " bar, T = " << tocelsius(temp()) << "°C" << endl;
	SpecificEnergy u = _state->specienergy(*this);
	//ENSURE(u > 0.0 * kJ_kg);
	return u;
}

/// Specific entropy [kJ/kg]
SpecificEntropy
SteamCalculator::specentropy(void) const{
	REQUIRE(isset);
	return _state->specentropy(*this);
}

/// Specific enthalpy [kJ/kg]
SpecificEnergy
SteamCalculator::specenthalpy(void) const{
	REQUIRE(isset);
	return _state->specenthalpy(*this);
}

/// Specific isobaric hear capacity [kJ/kgK]
SpecHeatCap
SteamCalculator::speccp(void) const{
	REQUIRE(isset);
	return _state->speccp(*this);
}

/// Specific isochoric heat capacity [kJ/kgK]
SpecHeatCap
SteamCalculator::speccv(void) const{
	REQUIRE(isset);
	return _state->speccv(*this);
}

/// Steam quality (if saturated)
Num
SteamCalculator::quality(void) const{
	REQUIRE(isset);
	return this->x;
}

// VISCOSITY and THERMAL CONDUCTIVITY are below

//------------------------------------------------------------------------
// CORRELATION DATA FOR VISCOSITY & THERMAL CONDUCTIVITY

// astyle made a mess of these :(

#define VISC_N0_COUNT 4

const Num VISC_N0[VISC_N0_COUNT] = { 1, 0.978197, 0.579829, -0.202354 };

#define VISC_COUNT 19

const Num VISC_I[VISC_COUNT] = { 0, 1, 4, 5, 0, 1, 2, 3, 0, 1, 2, 0, 1, 2, 3, 0, 3, 1, 3 };
const Num VISC_J[VISC_COUNT]= { 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 5, 6 };

const Num VISC_N[VISC_COUNT]
= { 0.5132047, 0.3205656, -0.7782567, 0.1885447, 0.2151778, 0.7317883, 1.241044, 1.476783, -0.2818107, -1.070786, -1.263184, 0.1778064, 0.460504, 0.2340379, -0.4924179, -0.0417661, 0.1600435, -0.01578386, -0.003629481 };

#define THCON_N0_COUNT 4

const Num THCON_N0[THCON_N0_COUNT] = { 1, 6.978267, 2.599096, -0.998254 };

#define THCON_I_COUNT 5
#define THCON_J_COUNT 6

// reference as nthcon[i][j] = down then across.
const Num THCON_N[THCON_I_COUNT][THCON_J_COUNT]
= {
	{ 1.3293046, -0.40452437, 0.2440949, 0.018660751, -0.12961068, 0.044809953 }
	, {1.7018363, -2.2156845, 1.6511057, -0.76736002, 0.37283344, -0.1120316}
	, {5.2246158, -10.124111, 4.9874687, -0.27297694, -0.43083393, 0.13333849}
	, {8.7127675, -9.5000611, 4.3786606, -0.91783782, 0, 0}
	, {-1.8525999, 0.9340469, 0, 0, 0, 0}
};


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

//----------------------------------------------------------------------
// THERMAL CONDUCTIVITY CALCULATION TO IAPS85 / REVISED IAPWS 1998

/**
	@note
		I can't remember why, but it appears I implemented the scientific formulation version of the conductivity equations. I think it was to do with difficulies in evaluating one of the partial derivatives, or non-availablity of some information... but I can't remember exactly why...
*/

/// Conductivity [mW/m.K]
/**
	Returns the thermal conductivity of water/steam.

	Range of validity is entire regions 1,2,3,4.

	Reference: J Phys Chem Ref Data, vol 13, no 1, 1984, p. 175 ff.

	@return Thermal conductivity [W/m.K]
	@see http://www.iapws.org/relguide/thcond.pdf
	@see http://www.iapws.org/relguide/IF97.pdf
*/
Conductivity
SteamCalculator::conductivity() const{
	REQUIRE(isset);
	Conductivity k = IAPS85_THCOND_REF * lam();
	ENSURE(!isnan(k));
	return k;
}

/// Used in the calculation of reduced thermal conductivity
/**
* Reference: IAPWS Revised Release on the IAPS Formulation 1985 for the Thermal
* Conductivity of Ordinary Water Substance, 1998, IAPWS.
*
* @return lambda_0 [dim'less]
* @see http://www.iapws.org/relguide/IF97.pdf
*
*/
Num
SteamCalculator::lam0() const{

	Num l = 0;
	Num ttau = this->tau_iaps85();

	for (int i = 0; i < THCON_N0_COUNT; i++) {	// nb i is important here...
		l += THCON_N0[i] / pow(ttau, i);
		//printf("NOTE: i=%d, n0_i=%.5f, l = %.5f\n",i,THCON_N0[i],l);
	}

	Num l1 = (sqrt(ttau) / l);
	//printf("NOTE: lam0 = %.5f\n",l1);

	ENSURE(!isnan(l1));
	ENSURE(!isinf(l1));
	return l1;
}

/// Used in the calculation of reduced thermal conductivity
/**
* Reference: IAPWS Revised Release on the IAPS Formulation 1985 for the Thermal
* Conductivity of Ordinary Water Substance, 1998, IAPWS.
*
* @return lambda_0 [dim'less]
* @see http://www.iapws.org/relguide/IF97.pdf
*
*/
Num
SteamCalculator::lam1() const{
	Num l = 0;
	Num tttau = 1 / this->tau_iaps85();
	Num ddel = this->del_iaps85();

	for (int i = 0; i < THCON_I_COUNT; i++) {
		for (int j = 0; j < THCON_J_COUNT; j++) {	// nb i and j important here...
			l += THCON_N[i][j] * pow(tttau - 1, i) * pow(ddel - 1, j);
			//printf("NOTE: i=%2d, j=%2d, n_ij=%.5f, l=%.5f\n",i,j,THCON_N[i][j],l);
		}
	}

	Num l1 = exp(ddel * l);
	//printf("NOTE: lam1 = %.5f\n",l1);

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
*
*/
Num
SteamCalculator::lam2() const{

	Num tt = tau_iaps85();
	Num dd = del_iaps85();


	//fprintf(stderr,"SteamCalculator::lam2 dd=%.5f\n",dd);

	Num ddpp = this->delpi_iaps85();
	if (isnan(ddpp)) {
		printf("ERROR: delpi_iaps85 returns not a number\n");
	}
	//printf("NOTE: delpi_iaps85 = %.5f\n",ddpp);


	Num pptt = this->pitau_iaps85();
	if (isnan(pptt)) {
		printf("ERROR: pitau_iaps85 returns not a number\n");
	}
	//printf("NOTE: pitau_iaps85 = %.5f\n",ddpp);

	ASSERT(!isinf(tt));
	ASSERT(dd != 0);
	ASSERT(!isinf(pptt));
	ASSERT(!isinf(dd));

	Num chi = dd * ddpp;

	ASSERT(!isinf(chi));

	Num ll = 0.0013848 / mu0() / mu1()
	                * sq(tt / dd)
	                * sq(pptt)
	                * pow(chi, (Num) 0.4678)
	                * sqrt(dd)
	                * exp(-18.66 * sq(tt - 1) - pow(dd - 1, 4));

	if (isnan(ll)) {
		printf("ERROR: lam2 returns not a number\n");
	}

	ENSURE(!isnan(ll));
	ENSURE(!isinf(ll));
	return ll;
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
SteamCalculator::lam() const{
	Num l0 = lam0();
	Num l1 = lam1();
	Num l2 = lam2();
	//Num l2=0;
	Num lam = (lam0() * lam1() + lam2());
	//printf("NOTE: lam0=%.5f, lam1=%.5f, lam2=%.5f => lam=%.5f\n",l0,l1,l2,lam);
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

		gas = new SteamCalculator();
		gas->setSatSteam_T(T);
		liq = new SteamCalculator();
		liq->setSatWater_T(T);

		this->x=x;
		this->T=T;
		isset=true;
	}catch(Exception *E){
		stringstream s;
		s << "SteamCalculator::setRegion4_Tx: " << E->what();
		throw new Exception(s.str());
	}
}
