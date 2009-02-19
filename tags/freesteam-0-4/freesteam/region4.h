#ifndef REGION4_H
#define REGION4_H

#include "state.h"
#include "zeroin.h"

class Region4 : public SteamState {

	public:
		int getRegion() const;

		SpecificVolume specvol(const SteamCalculator &c) const;
		SpecificEnergy specienergy(const SteamCalculator &c) const;
		SpecificEntropy specentropy(const SteamCalculator &c) const;
		SpecificEnergy specenthalpy(const SteamCalculator &c) const;
		SpecHeatCap speccp(const SteamCalculator &c) const;
		SpecHeatCap speccv(const SteamCalculator &c) const;

		Density dens(const SteamCalculator &c) const;
		Pressure pres(const SteamCalculator &c) const;
		Temperature temp(const SteamCalculator &c) const;

	protected:
		friend class SteamCalculator;

		static SteamState *Instance();

		virtual void set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x);

		virtual Num pitau_iaps85(const SteamCalculator &c) const;
		virtual Num delpi_iaps85(const SteamCalculator &c) const;

	private:
		Region4();
		static Region4 *_instance;

};

#endif
