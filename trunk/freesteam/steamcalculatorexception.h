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

#ifndef STEAMCALCULATOREXCEPTION_H
#define STEAMCALCULATOREXCEPTION_H

#include "common.h"

#ifndef STEAMCALCULATOR_H
class SteamCalculator;
class Boundaries;
class SteamState;
#endif

#include "exception.h"

typedef enum {
    STM_UNKNOWN_ERROR =
        0, STM_RANGE_OVERALL, STM_BEYOND_SAT_CURVE, STM_NOT_SAT,
    REG1_RANGE_T_HIGH, REG1_RANGE_P_LOW, REG3_RANGE_T_LOW_TB,
    REG3_RANGE_P_LOW_PB, REG2_RANGE_T_LOW_TS, REG2_RANGE_P_HIGH_PB,
    REG2_RANGE_TB_LOW, REG2_RANGE_TB_HIGH, REG2_RANGE_PB_LOW,
    REG2_RANGE_PB_HIGH, STM_UNABLE_DET_REGION, STM_SAT_INEXACT,
    STM_ILLEGAL_WRAP_REG3_PRES_ERR, STM_PRESSURE_HIGH,
    STM_PRESSURE_LOW, STM_TEMPERATURE_HIGH, STM_TEMPERATURE_LOW
} SteamErrorCode;

class SteamCalculatorException:public Exception {

	public:

		string what();
		SteamErrorCode getType();

	protected:
		friend class SteamCalculator;
		friend class Boundaries;
		friend class SteamState;

		SteamCalculatorException(Pressure p = -1.0 * MPa, Temperature T =
		                             -1.0 * Kelvin, SteamErrorCode c = STM_UNKNOWN_ERROR);

	private:

		SteamErrorCode type;
		Pressure p;
		Temperature T;
		string message;
};

class SteamAlmostSaturatedException:public Exception {

	public:
		string what();

	protected:
		friend class Boundaries;
		SteamAlmostSaturatedException(Pressure p = -1.0 * MPa, Temperature T = -1.0 *Kelvin);

	private:
		Pressure p;
		Temperature T;
};

class OverPressureException : public Exception {
	OverPressureException(string message);
};

#endif
