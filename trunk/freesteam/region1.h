#ifndef REGION1_H
#define REGION1_H

#include "state.h"
#include "region4.h"
#include "units.h"

class Region1:public SteamState {

	protected:
		friend class SteamCalculator;
		friend class Region4;
		
		static SteamState *Instance();
		int Region1::getRegion() const;
		virtual void set_pT(SteamCalculator &c, const Pressure &p, const Temperature &T, Num x);

	private:
		Region1();
		static Region1 *_instance;

		METHOD_H_OBJ(gam);
		METHOD_H_OBJ(gampi);
		METHOD_H_OBJ(gampipi);
		METHOD_H_OBJ(gamtau);
		METHOD_H_OBJ(gamtautau);
		METHOD_H_OBJ(gampitau);

		METHOD_H_OBJ(pitau_iaps85);
		METHOD_H_OBJ(delpi_iaps85);

};

#endif
