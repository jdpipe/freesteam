#ifndef REGION2_H
#define REGION2_H

#include "state.h"

class Region2:public SteamState {

	protected:
		friend class SteamCalculator;
		friend class Region4;

		static SteamState *Instance();
		int getRegion() const;
		virtual void set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x);

	private:
		Region2();
		static Region2 *_instance;

		Num gam(const SteamCalculator &c) const;
		Num gampi(const SteamCalculator &c) const;
		Num gamtau(const SteamCalculator &c) const;
		Num gampipi(const SteamCalculator &c) const;
		Num gamtautau(const SteamCalculator &c) const;
		Num gampitau(const SteamCalculator &c) const;


		Num gam0(const SteamCalculator &c) const;
		Num gam0sum(const SteamCalculator &c) const;
		Num gampi0(const SteamCalculator &c) const;
		Num gamtau0(const SteamCalculator &c) const;
		Num gampipi0(const SteamCalculator &c) const;
		Num gamtautau0(const SteamCalculator &c) const;
		Num gampitau0(const SteamCalculator &c) const;

		Num gamr(const SteamCalculator &c) const;
		Num gampir(const SteamCalculator &c) const;
		Num gamtaur(const SteamCalculator &c) const;
		Num gampipir(const SteamCalculator &c) const;
		Num gamtautaur(const SteamCalculator &c) const;
		Num gampitaur(const SteamCalculator &c) const;

		Num pitau_iaps85(const SteamCalculator &c) const;
		Num delpi_iaps85(const SteamCalculator &c) const;

};

#endif
