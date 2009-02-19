#ifndef STEAMSTATE_H
#define STEAMSTATE_H

#include "steamcalculator.h"

#define METHOD_STST(fun,type) type fun(SteamCalculator *c)
#define METHOD_STST_NUM(fun)  Num fun(SteamCalculator *c)

class SteamState:public DesignByContract {
	public:

		virtual int getRegion(void);

		virtual METHOD_STST(specvol,      SpecificVolume);
		virtual METHOD_STST(specienergy,  SpecificEnergy);
		virtual METHOD_STST(specentropy,  SpecificEntropy);
		virtual METHOD_STST(specenthalpy, SpecificEnergy);
		virtual METHOD_STST(speccp,       SpecHeatCap);
		virtual METHOD_STST(speccv,       SpecHeatCap);

		virtual METHOD_STST(temp,         Temperature);
		virtual METHOD_STST(pres,         Pressure);
		virtual METHOD_STST(dens,         Density);

		//virtual METHOD_STST(dynvisc,      DynamicViscosity);
		//virtual METHOD_STST(conductivity, Conductivity);

	protected:

		friend class SteamCalculator;

		static SteamState *Instance();
		virtual void set_pT(SteamCalculator * c, Pressure p, Temperature T,
		                    Num x = -1);

		virtual METHOD_STST_NUM(pitau_iaps85) {}
		virtual METHOD_STST_NUM(delpi_iaps85) {}		

		virtual METHOD_STST_NUM(phi) {}
		virtual METHOD_STST_NUM(phidel) {}
		virtual METHOD_STST_NUM(phideldel) {}
		virtual METHOD_STST_NUM(phitau) {}
		virtual METHOD_STST_NUM(phitautau) {}
		virtual METHOD_STST_NUM(phideltau) {}
		virtual METHOD_STST_NUM(gam) {}
		virtual METHOD_STST_NUM(gamtau) {}
		virtual METHOD_STST_NUM(gamtautau) {}
		virtual METHOD_STST_NUM(gampi) {}
		virtual METHOD_STST_NUM(gampipi) {}
		virtual METHOD_STST_NUM(gampitau) {}

		SteamState() {}

	private:



};

#endif
