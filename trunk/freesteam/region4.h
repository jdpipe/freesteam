#ifndef REGION4_H
#define REGION4_H

#include "state.h"
#include "zeroin.h"

class Region4:public SteamState {

	public:
		int getRegion() const;

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

		virtual void set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x);

	private:
		Region4();
		static Region4 *_instance;

};

#endif
