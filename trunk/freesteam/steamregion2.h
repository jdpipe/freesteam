#include "steamstate.h"


class SteamRegion2:public SteamState {

	protected:
		friend class SteamCalculator;
		friend class SteamRegion4;

		static SteamState *Instance();
		int getRegion();
		virtual void set_pT(SteamCalculator * c, Pressure p, Temperature T,
		                    Num x);

	private:
		SteamRegion2();
		static SteamRegion2 *_instance;

		METHOD_H_OBJ(gam);
		METHOD_H_OBJ(gampi);
		METHOD_H_OBJ(gamtau);
		METHOD_H_OBJ(gampipi);
		METHOD_H_OBJ(gamtautau);
		METHOD_H_OBJ(gampitau);


		METHOD_H_OBJ(gam0);
		METHOD_H_OBJ(gam0sum);
		METHOD_H_OBJ(gampi0);
		METHOD_H_OBJ(gamtau0);
		METHOD_H_OBJ(gampipi0);
		METHOD_H_OBJ(gamtautau0);
		METHOD_H_OBJ(gampitau0);

		METHOD_H_OBJ(gamr);
		METHOD_H_OBJ(gampir);
		METHOD_H_OBJ(gamtaur);
		METHOD_H_OBJ(gampipir);
		METHOD_H_OBJ(gamtautaur);
		METHOD_H_OBJ(gampitaur);

		METHOD_H_OBJ(pitau_iaps85);
		METHOD_H_OBJ(delpi_iaps85);

};
