#ifndef STEAMREGION4_H
#define STEAMREGION4_H

#include "state.h"
#include "zeroin.h"

class SteamRegion4:public SteamState {

	public:
		int getRegion();

		METHOD_STST(specvol,      SpecificVolume);
		METHOD_STST(specienergy,  SpecificEnergy);
		METHOD_STST(specentropy,  SpecificEntropy);
		METHOD_STST(specenthalpy, SpecificEnergy);
		METHOD_STST(speccp,       SpecHeatCap);
		METHOD_STST(speccv,       SpecHeatCap);

		METHOD_STST(dens,         Density);

	protected:
		friend class SteamCalculator;

		static SteamState *Instance();

		virtual void set_pT(SteamCalculator * c, Pressure p, Temperature T,
		                    Num x);

	private:
		SteamRegion4();
		static SteamRegion4 *_instance;

		SteamState *liquidstate;

};

#endif
