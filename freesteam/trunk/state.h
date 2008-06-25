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

#ifndef STEAMSTATE_H
#define STEAMSTATE_H

//#include "steamcalculator.h"
class SteamCalculator;
#include "common.h"
#include "units.h"
#include "designbycontract.h"

class SteamState : public DesignByContract {
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
