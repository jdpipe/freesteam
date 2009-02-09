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
*/

#include "backwards.h"
#include <math.h>

/*------------------------------------------------------------------------------
  REGION 1
*/

#define SQ(X) ((X)*(X))

typedef struct{
	double I, J, n;
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

