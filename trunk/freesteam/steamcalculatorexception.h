#ifndef STEAMCALCULATOREXCEPTION_H
#define STEAMCALCULATOREXCEPTION_H

#include "common.h"

#ifndef STEAMCALCULATOR_H
class SteamCalculator;
class SteamRegionBoundaries;
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
		friend class SteamRegionBoundaries;
		//              friend class SteamRegion1;
		//              friend class SteamRegion2;
		//              friend class SteamRegion3;
		friend class SteamState;

		SteamCalculatorException(Pressure p = -1, Temperature T =
		                             -1, SteamErrorCode c = STM_UNKNOWN_ERROR);

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
		friend class SteamRegionBoundaries;
		SteamAlmostSaturatedException(Pressure p = -1, Temperature T = -1);

	private:
		Num p;
		Num T;
};
#endif
