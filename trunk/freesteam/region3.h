#ifndef REGION3_H
#define REGION3_H

#include "state.h"
#include "zeroin.h"

#define REG3_DENS_REF RHO_CRIT	// kg/m³
#define REG3_TEMP_REF T_CRIT	// K

class Region3:public SteamState {

	public:
		int getRegion() const;

		SpecificVolume specvol(const SteamCalculator &c) const;
		SpecificEnergy specienergy(const SteamCalculator &c) const;
		SpecificEntropy specentropy(const SteamCalculator &c) const;
		SpecificEnergy specenthalpy(const SteamCalculator &c) const;
		SpecHeatCap speccp(const SteamCalculator &c) const;
		SpecHeatCap speccv(const SteamCalculator &c) const;

		Pressure pres(const SteamCalculator &c) const;
		Density dens(const SteamCalculator &c) const;

		Temperature temp(const SteamCalculator &c) const;

	protected:
		friend class SteamCalculator;

		static SteamState *Instance();

		virtual void set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x);

	private:

		Region3();
		static Region3 *_instance;

		Num phi(const SteamCalculator &c) const;
		Num phidel(const SteamCalculator &c) const;
		Num phideldel(const SteamCalculator &c) const;
		Num phitau(const SteamCalculator &c) const;
		Num phitautau(const SteamCalculator &c) const;
		Num phideltau(const SteamCalculator &c) const;

		Num pitau_iaps85(const SteamCalculator &c) const;
		Num delpi_iaps85(const SteamCalculator &c) const;

};

#endif
