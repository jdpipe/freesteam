#ifndef REGION1_H
#define REGION1_H

#include "state.h"
#include "region4.h"
#include "units.h"

class Region1:public SteamState {

	protected:
		friend class SteamCalculator;
		friend class Region4;

		virtual SpecificVolume specvol(const SteamCalculator &c) const;
		virtual SpecificEnergy specienergy(const SteamCalculator &c) const;
		virtual SpecificEntropy specentropy(const SteamCalculator &c) const;
		virtual SpecificEnergy specenthalpy(const SteamCalculator &c) const;
		virtual SpecHeatCap speccp(const SteamCalculator &c) const;
		virtual SpecHeatCap speccv(const SteamCalculator &c) const;

		virtual Temperature temp(const SteamCalculator &c) const;
		virtual Pressure pres(const SteamCalculator &c) const;
		virtual Density dens(const SteamCalculator &c) const;

		static SteamState *Instance();
		int Region1::getRegion() const;
		virtual void set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x);

	private:
		Region1();
		static Region1 *_instance;

		Num gam(const SteamCalculator &c) const;
		Num gampi(const SteamCalculator &c) const;
		Num gampipi(const SteamCalculator &c) const;
		Num gamtau(const SteamCalculator &c) const;
		Num gamtautau(const SteamCalculator &c) const;
		Num gampitau(const SteamCalculator &c) const;

		Num pitau_iaps85(const SteamCalculator &c) const;
		Num delpi_iaps85(const SteamCalculator &c) const;

};

#endif
