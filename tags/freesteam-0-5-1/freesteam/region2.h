/*

freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2005  John Pye

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef REGION2_H
#define REGION2_H

#include "state.h"

class Region2:public SteamState {

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