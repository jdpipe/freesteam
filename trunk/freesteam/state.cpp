#include "state.h"
#include "steamcalculator_macros.h"
#include <cstdio>

#define EVAL_STEAM(FUNC,EXPR) EXPR_FUNC(SteamState,FUNC,EXPR)
#define EVAL_STEAM_OBJ(FUNC,EXPR) EXPR_FUNC_OBJ(SteamState,FUNC,EXPR)

int SteamState::getRegion() const{
	return -1;
}
