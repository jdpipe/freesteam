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

#include "steamcalculatorexception.h"

#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

#define CASE_EXPLANATION(FLAG) case FLAG: explanation=#FLAG; break

string SteamCalculatorException::what(void) {

	//fprintf(stderr,"SteamCalculatorException::what()\n");

	string explanation;

	switch (this->type) {

		case STM_UNKNOWN_ERROR:
			explanation = "(Unknown error)";
			break;

			CASE_EXPLANATION(STM_RANGE_OVERALL);
			CASE_EXPLANATION(STM_BEYOND_SAT_CURVE);
			CASE_EXPLANATION(STM_NOT_SAT);
			CASE_EXPLANATION(REG1_RANGE_T_HIGH);
			CASE_EXPLANATION(REG1_RANGE_P_LOW);
			CASE_EXPLANATION(REG3_RANGE_T_LOW_TB);
			CASE_EXPLANATION(REG3_RANGE_P_LOW_PB);
			CASE_EXPLANATION(REG2_RANGE_T_LOW_TS);
			CASE_EXPLANATION(REG2_RANGE_P_HIGH_PB);
			CASE_EXPLANATION(REG2_RANGE_TB_LOW);
			CASE_EXPLANATION(REG2_RANGE_TB_HIGH);
			CASE_EXPLANATION(REG2_RANGE_PB_LOW);
			CASE_EXPLANATION(REG2_RANGE_PB_HIGH);
			CASE_EXPLANATION(STM_SAT_INEXACT);
			CASE_EXPLANATION(STM_UNABLE_DET_REGION);
			CASE_EXPLANATION(STM_ILLEGAL_WRAP_REG3_PRES_ERR);
			CASE_EXPLANATION(STM_PRESSURE_LOW);
			CASE_EXPLANATION(STM_PRESSURE_HIGH);
			CASE_EXPLANATION(STM_TEMPERATURE_LOW);
			CASE_EXPLANATION(STM_TEMPERATURE_HIGH);

		default:
			explanation = "(Error description unavailable)";
	}

	stringstream s;
	s << explanation << ", p=" << (this->p / MPa)<< " MPa, T=" << this->T;

	this->message = s.str();

	return this->message;
}

SteamErrorCode SteamCalculatorException::getType() {
	return this->type;
}

SteamCalculatorException::SteamCalculatorException(Pressure p,
        Temperature T,
        SteamErrorCode c) : std::runtime_error("SteamCalculatorException") {
	this->type = c;
	this->p = p;
	this->T = T;
}

SteamAlmostSaturatedException::SteamAlmostSaturatedException(Pressure p,
        Temperature T) : std::runtime_error("SteamAlmostSaturatedException") {
	this->p = p;
	this->T = T;
}

string SteamAlmostSaturatedException::what() {
	stringstream s;
	s << "Steam almost saturated: p = " << this->p / MPa << "MPa, T = " << this->
	T / Kelvin<< "K";
	return s.str();
}

OverPressureException::OverPressureException(string message) : std::runtime_error(message){
	// Intentionally empty
}
