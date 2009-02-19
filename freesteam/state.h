#ifndef STEAMSTATE_H
#define STEAMSTATE_H

#include "steamcalculator.h"

class SteamState:public DesignByContract {
	public:

		virtual int getRegion(void) const;

		virtual SpecificVolume specvol(const SteamCalculator &c) const = 0;
		virtual SpecificEnergy specienergy(const SteamCalculator &c) const = 0;
		virtual SpecificEntropy specentropy(const SteamCalculator &c) const = 0;
		virtual SpecificEnergy specenthalpy(const SteamCalculator &c) const = 0;
		virtual SpecHeatCap speccp(const SteamCalculator &c) const = 0;
		virtual SpecHeatCap speccv(const SteamCalculator &c) const = 0;

		virtual Temperature temp(const SteamCalculator &c) const = 0;
		virtual Pressure pres(const SteamCalculator &c) const = 0;
		virtual Density dens(const SteamCalculator &c) const = 0;

	protected:

		friend class SteamCalculator;

		static SteamState *Instance();
		virtual void set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x = -1) = 0;

		virtual Num pitau_iaps85(const SteamCalculator &c) const = 0;
		virtual Num delpi_iaps85(const SteamCalculator &c) const = 0;

		SteamState(){}

};

#endif
