#include "state.h"
#include "zeroin.h"

#define REG3_DENS_REF RHO_CRIT	// kg/m³
#define REG3_TEMP_REF T_CRIT	// K

class SteamRegion3:public SteamState {

	public:
		int getRegion();

		METHOD_STST(specvol,      SpecificVolume);
		METHOD_STST(specienergy,  SpecificEnergy);
		METHOD_STST(specentropy,  SpecificEntropy);
		METHOD_STST(specenthalpy, SpecificEnergy);
		METHOD_STST(speccp,       SpecHeatCap);
		METHOD_STST(speccv,       SpecHeatCap);

		METHOD_STST(pres,         Pressure);
		METHOD_STST(dens,         Density);

	protected:
		friend class SteamCalculator;
		friend Pressure reg3_pres_err(double test_dens);

		static SteamState *Instance();

		virtual void set_pT(SteamCalculator * c, Pressure p, Temperature T,
		                    Num x);

		static SteamCalculator *reg3_steamcalculator_zeroin_object;

	private:

		Num my_zeroin(SteamCalculator * c, Num ax,
		                     Num bx, double (*f) (double x),
		                     Num tol);

		SteamRegion3();
		static SteamRegion3 *_instance;

		METHOD_H_OBJ(phi);
		METHOD_H_OBJ(phidel);
		METHOD_H_OBJ(phideldel);
		METHOD_H_OBJ(phitau);
		METHOD_H_OBJ(phitautau);
		METHOD_H_OBJ(phideltau);

		METHOD_H_OBJ(pitau_iaps85);
		METHOD_H_OBJ(delpi_iaps85);

};
