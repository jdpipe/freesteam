#ifndef REGION3_H
#define REGION3_H

#include "state.h"
#include "zeroin.h"

#define REG3_DENS_REF RHO_CRIT	// kg/m³
#define REG3_TEMP_REF T_CRIT	// K

class Region3:public SteamState {

	public:
		int getRegion() const;

		METHOD_STST(specvol,      SpecificVolume);
		METHOD_STST(specienergy,  SpecificEnergy);
		METHOD_STST(specentropy,  SpecificEntropy);
		METHOD_STST(specenthalpy, SpecificEnergy);
		METHOD_STST(speccp,       SpecHeatCap);
		METHOD_STST(speccv,       SpecHeatCap);

		METHOD_STST(pres,         Pressure);
		METHOD_STST(dens,         Density);

	protected:
		friend class SteamCalculator;

		static SteamState *Instance();

		virtual void set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x);

	private:

		Region3();
		static Region3 *_instance;

		METHOD_H_OBJ(phi);
		METHOD_H_OBJ(phidel);
		METHOD_H_OBJ(phideldel);
		METHOD_H_OBJ(phitau);
		METHOD_H_OBJ(phitautau);
		METHOD_H_OBJ(phideltau);

		METHOD_H_OBJ(pitau_iaps85);
		METHOD_H_OBJ(delpi_iaps85);

};

#endif
