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

		METHOD_STST_NUM(gam);
		METHOD_STST_NUM(gampi);
		METHOD_STST_NUM(gamtau);
		METHOD_STST_NUM(gampipi);
		METHOD_STST_NUM(gamtautau);
		METHOD_STST_NUM(gampitau);


		METHOD_STST_NUM(gam0);
		METHOD_STST_NUM(gam0sum);
		METHOD_STST_NUM(gampi0);
		METHOD_STST_NUM(gamtau0);
		METHOD_STST_NUM(gampipi0);
		METHOD_STST_NUM(gamtautau0);
		METHOD_STST_NUM(gampitau0);

		METHOD_STST_NUM(gamr);
		METHOD_STST_NUM(gampir);
		METHOD_STST_NUM(gamtaur);
		METHOD_STST_NUM(gampipir);
		METHOD_STST_NUM(gamtautaur);
		METHOD_STST_NUM(gampitaur);

		METHOD_STST_NUM(pitau_iaps85);
		METHOD_STST_NUM(delpi_iaps85);

};

#endif
