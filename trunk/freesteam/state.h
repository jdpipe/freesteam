#ifndef STEAMSTATE_H
#define STEAMSTATE_H

#include "steamcalculator.h"

class SteamState:public DesignByContract {
	public:

		virtual int getRegion(void) const;

		virtual SpecificVolume specvol(const SteamCalculator &c) const;
		virtual SpecificEnergy specienergy(const SteamCalculator &c) const;
		virtual SpecificEntropy specentropy(const SteamCalculator &c) const;
		virtual SpecificEnergy specenthalpy(const SteamCalculator &c) const;
		virtual SpecHeatCap speccp(const SteamCalculator &c) const;
		virtual SpecHeatCap speccv(const SteamCalculator &c) const;

		virtual Temperature temp(const SteamCalculator &c) const;
		virtual Pressure pres(const SteamCalculator &c) const;
		virtual Density dens(const SteamCalculator &c) const;

	protected:

		friend class SteamCalculator;

		static SteamState *Instance();
		virtual void set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x = -1);

		virtual Num pitau_iaps85(const SteamCalculator &c) const {}
		virtual Num delpi_iaps85(const SteamCalculator &c) const {}

		virtual Num phi(const SteamCalculator &c) const {}
		virtual Num phidel(const SteamCalculator &c) const {}
		virtual Num phideldel(const SteamCalculator &c) const {}
		virtual Num phitau(const SteamCalculator &c) const {}
		virtual Num phitautau(const SteamCalculator &c) const {}
		virtual Num phideltau(const SteamCalculator &c) const {}
		virtual Num gam(const SteamCalculator &c) const {}
		virtual Num gamtau(const SteamCalculator &c) const {}
		virtual Num gamtautau(const SteamCalculator &c) const {}
		virtual Num gampi(const SteamCalculator &c) const {}
		virtual Num gampipi(const SteamCalculator &c) const {}
		virtual Num gampitau(const SteamCalculator &c) const {}

		SteamState() {}

	private:



};

#endif
