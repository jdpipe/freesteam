#ifndef STEAMREGION1_H
#define STEAMREGION1_H

#include "steamstate.h"
#include "steamregion4.h"
#include "units.h"

class SteamRegion1:public SteamState {

	protected:
		friend class SteamCalculator;
		friend class SteamRegion4;

		static SteamState *Instance();
		int SteamRegion1::getRegion();
		virtual void set_pT(SteamCalculator * c, Pressure p, Temperature T,
		                    Num x);

	private:
		SteamRegion1();
		static SteamRegion1 *_instance;

		METHOD_H_OBJ(gam);
		METHOD_H_OBJ(gampi);
		METHOD_H_OBJ(gampipi);
		METHOD_H_OBJ(gamtau);
		METHOD_H_OBJ(gamtautau);
		METHOD_H_OBJ(gampitau);

		METHOD_H_OBJ(pitau_iaps85);
		METHOD_H_OBJ(delpi_iaps85);

};

#endif
