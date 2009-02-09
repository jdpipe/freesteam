/* 
freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2009  John Pye

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
*//** @file
	Backwards equations for IAPWS-IF97. Facilitate calculation of
	properties in terms of (p,h) without requiring any iteration.

	TODO add boundary curves?
	TODO add region 3
	TODO add more equations for (p,s) calculation?

	Numerical for T(p,h) and v(p,h) correlations was extracted from
	the Matlab version 2.6 of Xsteam by by Magnus Holmgren.
*/

#include "backwards.h"
#include <math.h>

/*------------------------------------------------------------------------------
  REGION 1
*/

#define SQ(X) ((X)*(X))

typedef struct{
	int I, J;
	double n;
} TPHData;

/**
	Source: IAPWS-IF97-REV section 5.2.1
*/
TPHData REGION1_TPH_DATA[] = {
	{0, 0, -238.72489924521}
	,{0, 1, 404.21188637945}
	,{0, 2, 113.49746881718}
	,{0, 6, -5.8457616048039}
	,{0, 22, -1.528548241314E-04}
	,{0, 32, -1.0866707695377E-06}
	,{1, 0, -13.391744872602}
	,{1, 1, 43.211039183559}
	,{1, 2, -54.010067170506}
	,{1, 3, 30.535892203916}
	,{1, 4, -6.5964749423638}
	,{1,10, 9.3965400878363E-03}
	,{1,32, 1.157364750534E-07}
	,{2,10,-2.5858641282073E-05}
	,{2,32,-4.0644363084799E-09}
	,{3,10,6.6456186191635E-08}
	,{3,32,8.0670734103027E-11}
	,{4,32,-9.3477771213947E-13}
	,{5,32,5.8265442020601E-15}
	,{6,32,-1.5020185953503E-17}
};

const unsigned REGION1_TPH_MAX = sizeof(REGION1_TPH_DATA)/sizeof(TPHData);

const double REGION1_TPH_HSTAR = 2500e3; /* J/kg */
const double REGION1_TPH_PSTAR = 1e6; /* Pa */

/**
	Backward equation for temperature in terms of pressure and enthalpy
	in IAPWS-IF97 Region 1. Source: IAPWS-IF97-Rev section 5.2.1.

	@param p pressure in Pa
	@param h enthalpy in J/kgK
	@return temperature in K
*/
double freesteam_region1_T_ph(double p, double h){
	double pi = p / REGION1_TPH_PSTAR;
	double e1 = 1. + (h / REGION1_TPH_HSTAR);
	unsigned i;
	TPHData *d;
	double sum = 0;
	for(i=0, d = REGION1_TPH_DATA; i<REGION1_TPH_MAX; ++i, ++d){
		/* TODO some optimisations are possible here with pow(pi,...) */
		sum += d->n * pow(pi,d->I) * pow(e1, d->J);
	}
	return sum /* * REGION1_TPH_TSTAR = 1. */;
}


/*------------------------------------------------------------------------------
  REGION 2 
*/

/* sub-region 2a */
TPHData REGION2A_TPH_DATA[] = {
	{0,	0,	1089.8952318288}
	,{1,	0,	849.51654495535}
	,{2,	0,	-107.81748091826}
	,{3,	0,	33.153654801263}
	,{7,	0,	-7.4232016790248}
	,{20,	0,	11.765048724356}
	,{0,	1,	1.844574935579}
	,{1,	1,	-4.1792700549624}
	,{2,	1,	6.2478196935812}
	,{3,	1,	-17.344563108114}
	,{7,	1,	-200.58176862096}
	,{9,	1,	271.96065473796}
	,{11,	1,	-455.11318285818}
	,{18,	1,	3091.9688604755}
	,{44,	1,	252266.40357872}
	,{0,	2,	-6.1707422868339E-03}
	,{2,	2,	-0.31078046629583}
	,{7,	2,	11.670873077107}
	,{36,	2,	128127984.04046}
	,{38,	2,	-985549096.23276}
	,{40,	2,	2822454697.3002}
	,{42,	2,	-3594897141.0703}
	,{44,	2,	1722734991.3197}
	,{24,	3,	-13551.334240775}
	,{44,	3,	12848734.66465}
	,{12,	4,	1.3865724283226}
	,{32,	4,	235988.32556514}
	,{44,	4,	-13105236.545054}
	,{32,	5,	7399.9835474766}
	,{36,	5,	-551966.9703006}
	,{42,	5,	3715408.5996233}
	,{34,	6,	19127.72923966}
	,{44,	6,	-415351.64835634}
	,{28,	7,	-62.459855192507}
};

const unsigned REGION2A_TPH_MAX = sizeof(REGION2A_TPH_DATA)/sizeof(TPHData);

/* sub-region 2b */

TPHData REGION2B_TPH_DATA[] = {
	{0,	0,	1489.5041079516}
	,{1,	0,	743.07798314034}
	,{2,	0,	-97.708318797837}
	,{12,	0,	2.4742464705674}
	,{18,	0,	-0.63281320016026}
	,{24,	0,	1.1385952129658}
	,{28,	0,	-0.47811863648625}
	,{40,	0,	8.5208123431544E-03}
	,{0,	1,	0.93747147377932}
	,{2,	1,	3.3593118604916}
	,{6,	1,	3.3809355601454}
	,{12,	1,	0.16844539671904}
	,{18,	1,	0.73875745236695}
	,{24,	1,	-0.47128737436186}
	,{28,	1,	0.15020273139707}
	,{40,	1,	-0.002176411421975}
	,{2,	2,	-0.021810755324761}
	,{8,	2,	-0.10829784403677}
	,{18,	2,	-0.046333324635812}
	,{40,	2,	7.1280351959551E-05}
	,{1,	3,	1.1032831789999E-04}
	,{2,	3,	1.8955248387902E-04}
	,{12,	3,	3.0891541160537E-03}
	,{24,	3,	1.3555504554949E-03}
	,{2,	4,	2.8640237477456E-07}
	,{12,	4,	-1.0779857357512E-05}
	,{18,	4,	-7.6462712454814E-05}
	,{24,	4,	1.4052392818316E-05}
	,{28,	4,	-3.1083814331434E-05}
	,{40,	4,	-1.0302738212103E-06}
	,{18,	5,	2.821728163504E-07}
	,{24,	5,	1.2704902271945E-06}
	,{40,	5,	7.3803353468292E-08}
	,{28,	6,	-1.1030139238909E-08}
	,{2,	7,	-8.1456365207833E-14}
	,{28,	7,	-2.5180545682962E-11}
	,{1,	9,	-1.7565233969407E-18}
	,{40,	9,	8.6934156344163E-15}
};

const unsigned REGION2B_TPH_MAX = sizeof(REGION2B_TPH_DATA)/sizeof(TPHData);

/* sub-region 2c */
TPHData REGION2C_TPH_DATA[] ={
	{0,	-7,	-3236839855524.2}
	,{4,	-7,	7326335090218.1}
	,{0,	-6,	358250899454.47}
	,{2,	-6,	-583401318515.9}
	,{0,	-5,	-10783068217.47}
	,{2,	-5,	20825544563.171}
	,{0,	-2,	610747.83564516}
	,{1,	-2,	859777.2253558}
	,{0,	-1,	-25745.72360417}
	,{2,	-1,	31081.088422714}
	,{0,	0,	1208.2315865936}
	,{1,	0,	482.19755109255}
	,{4,	1,	3.7966001272486}
	,{8,	1,	-10.842984880077}
	,{4,	2,	-0.04536417267666}
	,{0,	6,	1.4559115658698E-13}
	,{1,	6,	1.126159740723E-12}
	,{4,	6,	-1.7804982240686E-11}
	,{10,	6,	1.2324579690832E-07}
	,{12,	6,	-1.1606921130984E-06}
	,{16,	6,	2.7846367088554E-05}
	,{20,	6,	-5.9270038474176E-04}
	,{22,	6,	1.2918582991878E-03}
};

const unsigned REGION2C_TPH_MAX = sizeof(REGION2C_TPH_DATA)/sizeof(TPHData);

const double REGION2AB_P = 4.e6; /* Pa */

const double REGION2_HSTAR = 2000e3;
const double REGION2_PSTAR = 1.e6;

/**
	Backward equation for temperature in terms of pressure and enthalpy
	in IAPWS-IF97 Region 2 (composed of sub-regions 2a, 2b, 2c).
	Source: IAPWS-IF97-Rev section 5.2.1.

	@param p pressure in Pa
	@param h enthalpy in J/kgK
	@return temperature in K
*/
double freesteam_region2_T_ph(double p, double h){
	double eta = h / REGION2_HSTAR;
	double pi = p / REGION2_PSTAR;
	double pi1, eta1;
	TPHData *d;
	unsigned i, n;
	double sum = 0;
	if(p < REGION2AB_P){
		pi1 = pi; eta1 = eta - 2.1;
		d = REGION2A_TPH_DATA;
		n = REGION2A_TPH_MAX;
	}else{
		if(p < 905.84278514723 - 0.67955786399241 * h + 1.2809002730136E-04 * SQ(h)){
			pi1 = pi - 2.; eta1 = eta - 2.6;
			d = REGION2B_TPH_DATA;
			n = REGION2B_TPH_MAX;
		}else{
			pi1 = pi + 25.; eta1 = eta - 1.8;
			d = REGION2C_TPH_DATA;
			n = REGION2C_TPH_MAX;
		}
	}

	for(i = 0; i<n; ++i, ++d){
		sum += d->n * pow(pi1, d->I) * pow(eta1, d->J);
	}

	return sum /* * REGION2_TSTAR = 1 K */;
}


/*------------------------------------------------------------------------------
  REGION 3 T_PH
*/

/* sub-region 3a */
TPHData REGION3A_TPH_DATA[] = {
	{-12,	0,	-1.33645667811215E-07}
	,{-12,	1,	4.55912656802978E-06}
	,{-12,	2,	-1.46294640700979E-05}
	,{-12,	6,	6.3934131297008E-03}
	,{-12,	14,	372.783927268847}
	,{-12,	16,	-7186.54377460447}
	,{-12,	20,	573494.7521034}
	,{-12,	22,	-2675693.29111439}
	,{-10,	1,	-3.34066283302614E-05}
	,{-10,	5,	-2.45479214069597E-02}
	,{-10,	12,	47.8087847764996}
	,{-8,	0,	7.64664131818904E-06}
	,{-8,	2,	1.28350627676972E-03}
	,{-8,	4,	1.71219081377331E-02}
	,{-8,	10,	-8.51007304583213}
	,{-5,	2,	-1.36513461629781E-02}
	,{-3,	0,	-3.84460997596657E-06}
	,{-2,	1,	3.37423807911655E-03}
	,{-2,	3,	-0.551624873066791}
	,{-2,	4,	0.72920227710747}
	,{-1,	0,	-9.92522757376041E-03}
	,{-1,	2,	-0.119308831407288}
	,{0,	0,	0.793929190615421}
	,{0,	1,	0.454270731799386}
	,{1,	1,	0.20999859125991}
	,{3,	0,	-6.42109823904738E-03}
	,{3,	1,	-0.023515586860454}
	,{4,	0,	2.52233108341612E-03}
	,{4,	3,	-7.64885133368119E-03}
	,{10,	4,	1.36176427574291E-02}
	,{12,	5,	-1.33027883575669E-02}
};

const unsigned REGION3A_TPH_MAX = sizeof(REGION3A_TPH_DATA)/sizeof(TPHData);

TPHData REGION3B_TPH_DATA[] = {
	{-12,	0,	3.2325457364492E-05}
	,{-12,	1,	-1.27575556587181E-04}
	,{-10,	0,	-4.75851877356068E-04}
	,{-10,	1,	1.56183014181602E-03}
	,{-10,	5,	0.105724860113781}
	,{-10,	10,	-85.8514221132534}
	,{-10,	12,	724.140095480911}
	,{-8,	0,	2.96475810273257E-03}
	,{-8,	1,	-5.92721983365988E-03}
	,{-8,	2,	-1.26305422818666E-02}
	,{-8,	4,	-0.115716196364853}
	,{-8,	10,	84.9000969739595}
	,{-6,	0,	-1.08602260086615E-02}
	,{-6,	1,	1.54304475328851E-02}
	,{-6,	2,	7.50455441524466E-02}
	,{-4,	0,	2.52520973612982E-02}
	,{-4,	1,	-6.02507901232996E-02}
	,{-3,	5,	-3.07622221350501}
	,{-2,	0,	-5.74011959864879E-02}
	,{-2,	4,	5.03471360939849}
	,{-1,	2,	-0.925081888584834}
	,{-1,	4,	3.91733882917546}
	,{-1,	6,	-77.314600713019}
	,{-1,	10,	9493.08762098587}
	,{-1,	14,	-1410437.19679409}
	,{-1,	16,	8491662.30819026}
	,{0,	0,	0.861095729446704}
	,{0,	2,	0.32334644281172}
	,{1,	1,	0.873281936020439}
	,{3,	1,	-0.436653048526683}
	,{5,	1,	0.286596714529479}
	,{6,	1,	-0.131778331276228}
	,{8,	1,	6.76682064330275E-03}
};

const unsigned REGION3B_TPH_MAX = sizeof(REGION3B_TPH_DATA)/sizeof(TPHData);

#define REGION3AB_PSTAR (1.e6)
#define REGION3AB_HSTAR (1.e3)

#define REGION3AB_PH(P,H) ((H)/REGION3AB_HSTAR - (\
	2014.64004206875 \
	+ 3.74696550136983*((P)/REGION3AB_PSTAR) \
	- 2.19921901054187E-02 * SQ((P)/REGION3AB_PSTAR) \
	+ 8.7513168600995E-05 * (P)/REGION3AB_PSTAR*SQ((P)/REGION3AB_HSTAR) \
	))

const double REGION3A_TPH_HSTAR = 2300e3;
const double REGION3A_TPH_PSTAR = 100.e6;
const double REGION3A_TPH_TSTAR = 760;

const double REGION3B_TPH_HSTAR = 2800e3;
const double REGION3B_TPH_PSTAR = 100.e6;
const double REGION3B_TPH_TSTAR = 860;

/**
	Backward equation for temperature in terms of pressure and enthalpy
	in IAPWS-IF97 Region 3 (composed of sub-regions 3a, 3b).
	Source: IAPWS 'Revised Supplementary Release on Backward Equations for the Functions
	T(p,h), v(p,h) and T(p,s), v(p,s) for Region 3 of the IAPWS Industrial
	Formulation 1997 for the Thermodynamic Properties of Water and Steam', 2004.

	@param p pressure in Pa
	@param h enthalpy in J/kgK
	@return temperature in K
*/
double freesteam_region3_T_ph(double p, double h){
	double pi1, eta1;
	double Tstar;
	TPHData *d;
	unsigned i, n;
	double sum = 0;
	if(REGION3AB_PH(p,h) <= 0.){
		/* sub-region 3a */
		pi1 = p/REGION3A_TPH_PSTAR + 0.240; eta1 = h/REGION3A_TPH_HSTAR - 0.615;
		d = REGION3A_TPH_DATA;
		n = REGION3A_TPH_MAX;
		Tstar = REGION3A_TPH_TSTAR;
	}else{
		/* sub-region 3b */
		pi1 = p/REGION3B_TPH_PSTAR + 0.298; eta1 = h/REGION3B_TPH_HSTAR - 0.720;
		d = REGION3B_TPH_DATA;
		n = REGION3B_TPH_MAX;
		Tstar = REGION3B_TPH_TSTAR;
	}

	for(i = 0; i<n; ++i, ++d){
		sum += d->n * pow(pi1, d->I) * pow(eta1, d->J);
	}

	return sum * Tstar;
}


/*------------------------------------------------------------------------------
  REGION 3 V_PH
*/

TPHData REGION3A_VPH_DATA[] = {
	{-12,	6,	5.29944062966028E-03}
	,{-12,	8,	-0.170099690234461}
	,{-12,	12,	11.1323814312927}
	,{-12,	18,	-2178.98123145125}
	,{-10,	4,	-5.06061827980875E-04}
	,{-10,	7,	0.556495239685324}
	,{-10,	10,	-9.43672726094016}
	,{-8,	5,	-0.297856807561527}
	,{-8,	12,	93.9353943717186}
	,{-6,	3,	1.92944939465981E-02}
	,{-6,	4,	0.421740664704763}
	,{-6,	22,	-3689141.2628233}
	,{-4,	2,	-7.37566847600639E-03}
	,{-4,	3,	-0.354753242424366}
	,{-3,	7,	-1.99768169338727}
	,{-2,	3,	1.15456297059049}
	,{-2,	16,	5683.6687581596}
	,{-1,	0,	8.08169540124668E-03}
	,{-1,	1,	0.172416341519307}
	,{-1,	2,	1.04270175292927}
	,{-1,	3,	-0.297691372792847}
	,{0,	0,	0.560394465163593}
	,{0,	1,	0.275234661176914}
	,{1,	0,	-0.148347894866012}
	,{1,	1,	-6.51142513478515E-02}
	,{1,	2,	-2.92468715386302}
	,{2,	0,	6.64876096952665E-02}
	,{2,	2,	3.52335014263844}
	,{3,	0,	-1.46340792313332E-02}
	,{4,	2,	-2.24503486668184}
	,{5,	2,	1.10533464706142}
	,{8,	2,	-4.08757344495612E-02}
};

const unsigned REGION3A_VPH_MAX = sizeof(REGION3A_VPH_DATA)/sizeof(TPHData);

TPHData REGION3B_VPH_DATA[] = {
	{-12,	0,	-2.25196934336318E-09}
	,{-12,	1,	1.40674363313486E-08}
	,{-8,	0,	2.3378408528056E-06}
	,{-8,	1,	-3.31833715229001E-05}
	,{-8,	3,	1.07956778514318E-03}
	,{-8,	6,	-0.271382067378863}
	,{-8,	7,	1.07202262490333}
	,{-8,	8,	-0.853821329075382}
	,{-6,	0,	-2.15214194340526E-05}
	,{-6,	1,	7.6965608822273E-04}
	,{-6,	2,	-4.31136580433864E-03}
	,{-6,	5,	0.453342167309331}
	,{-6,	6,	-0.507749535873652}
	,{-6,	10,	-100.475154528389}
	,{-4,	3,	-0.219201924648793}
	,{-4,	6,	-3.21087965668917}
	,{-4,	10,	607.567815637771}
	,{-3,	0,	5.57686450685932E-04}
	,{-3,	2,	0.18749904002955}
	,{-2,	1,	9.05368030448107E-03}
	,{-2,	2,	0.285417173048685}
	,{-1,	0,	3.29924030996098E-02}
	,{-1,	1,	0.239897419685483}
	,{-1,	4,	4.82754995951394}
	,{-1,	5,	-11.8035753702231}
	,{0,	0,	0.169490044091791}
	,{1,	0,	-1.79967222507787E-02}
	,{1,	1,	3.71810116332674E-02}
	,{2,	2,	-5.36288335065096E-02}
	,{2,	6,	1.6069710109252}
};

const unsigned REGION3B_VPH_MAX = sizeof(REGION3B_VPH_DATA)/sizeof(TPHData);

const double REGION3A_VPH_HSTAR = 2100e3; /* J/kg */
const double REGION3A_VPH_PSTAR = 100.e6; /* Pa */
const double REGION3A_VPH_VSTAR = 0.0028; /* m³/kg */

const double REGION3B_VPH_HSTAR = 2800e3;
const double REGION3B_VPH_PSTAR = 100.e6;
const double REGION3B_VPH_VSTAR = 0.0088;

/**
	Backward equation for temperature in terms of pressure and enthalpy
	in IAPWS-IF97 Region 2 (composed of sub-regions 2a, 2b, 2c).
	Source: IAPWS-IF97-Rev section 5.2.1.

	@param p pressure in Pa
	@param h enthalpy in J/kgK
	@return temperature in K
*/
double freesteam_region3_v_ph(double p, double h){
	double pi1, eta1;
	TPHData *d;
	unsigned i, n;
	double sum = 0;
	double vstar;
	if(REGION3AB_PH(p,h) <= 0.){
		/* sub-region 3a */
		pi1 = p/REGION3A_VPH_PSTAR + 0.128; eta1 = h/REGION3A_VPH_HSTAR - 0.727;
		d = REGION3A_VPH_DATA;
		n = REGION3A_VPH_MAX;
		vstar = REGION3A_VPH_VSTAR;
	}else{
		/* sub-region 3b */
		pi1 = p/REGION3B_VPH_PSTAR + 0.0661; eta1 = h/REGION3B_VPH_HSTAR - 0.720;
		d = REGION3B_VPH_DATA;
		n = REGION3B_VPH_MAX;
		vstar = REGION3B_VPH_VSTAR;
	}

	for(i = 0; i<n; ++i, ++d){
		sum += d->n * pow(pi1, d->I) * pow(eta1, d->J);
	}

	return sum * vstar;
}

