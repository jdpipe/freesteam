#ifndef STEAMCALCULATORSTATE_H
#define STEAMCALCULATORSTATE_H

#include "designbycontract.h"
#include "common.h"
#include "steamcalculatorexception.h"
#include "boundaries.h"
#include "state.h"
#include "zeroin.h"

#define METHOD_SC(funcname,units) units funcname()
#define METHOD_SC_NUM(funcname) Num funcname()

typedef enum { STEAM_SUBCOOLED =
                   0, STEAM_SUPERHEATED, STEAM_SATURATED, STEAM_SUPERCRITICAL,
               STEAM_STATE_UNKNOWN = -1 } SteamStateCode;

typedef enum { STEAM_TEMPERATURE =
                   0, STEAM_PRESSURE, STEAM_VOLUME, STEAM_INTERNAL_ENERGY,
               STEAM_ENTHALPY, STEAM_ENTROPY } SteamPropertyCode;

class SteamCalculator:public DesignByContract {

	public:

		// Initializing the SteamCalculator, setting conditions

		SteamCalculator();
		SteamCalculator(const SteamCalculator & original);
		virtual void copy(const SteamCalculator & original);
		SteamCalculator const &operator=(SteamCalculator const &original);
		~SteamCalculator();

		void set_pT(Pressure p, Temperature T, double x = -1);	// pressure [MPa]
		void set_rhou(Density rho, SpecificEnergy u);	// density [kg/m3], specific internal energy [kJ/kg]

		void changeState(SteamState * state);

		void setSatSteam_p(Pressure p);	// p Pressure [MPa]
		void setSatWater_p(Pressure p);	// p Pressure [MPa]
		void setSatWater_T(Temperature T);	// T Temperature [K]
		void setSatSteam_T(Temperature T);	// T Temperature [K]

		bool isSet(void);

#ifndef NDEBUG
		virtual bool isValid(void);
#endif

		// Methods to return properties and state

		int whichRegion(void);
		SteamStateCode whichState(void);
		const char *SteamCalculator::whichStateStr(void);

		METHOD_SC(temp, Temperature);	// Temperature [K]
		METHOD_SC(pres, Pressure);	    // Pressure    [MPa]
		METHOD_SC(dens, Density);       // Density     [kg/m³]

		METHOD_SC(specvol,       SpecificVolume);	// Specific volume                  [m³/kg]
		METHOD_SC(specienergy,   SpecificEnergy);	// Specific internal energy         [kJ/kg]
		METHOD_SC(specentropy,   SpecificEntropy);	// Specific entropy                 [kJ/kg]
		METHOD_SC(specenthalpy,  SpecificEnergy);	// Specific enthalpy                [kJ/kg]
		METHOD_SC(speccp,        SpecHeatCap);	    // Sepcific isobaric hear capacity  [kJ/kg]
		METHOD_SC(speccv,        SpecHeatCap);	    // Specific isochoric heat capacity [kJ/kg]
		METHOD_SC(quality,       Num);	            // Steam quality
		METHOD_SC(dynvisc,       DynamicViscosity);	// Dynamic viscosity, mu            [Pa.s]
		METHOD_SC(conductivity,  Conductivity);	    // Conductivity

		void setTarget(SteamPropertyCode p, Num value);

		Temperature solveTemperature();

		Pressure solvePressure();
		Num solveQuality();

		Num my_zeroin(Num ax, Num bx,
		                     double (*f) (double x), double tol);

		Num getPropertyValue_p(Num p);
		Num getPropertyValue_T(Num T);
		Num getPropertyValue();

		static double wrap_getPropertyValue_p(double);
		static double wrap_getPropertyValue_T(double);

	protected:
		friend class SteamState;
		friend class SteamRegion1;
		friend class SteamRegion2;
		friend class SteamRegion3;
		friend class SteamRegion4;
		friend Pressure reg3_pres_err(double);
		friend class ZeroIn < SteamCalculator, Density, Temperature >;
		friend class ZeroIn < SteamCalculator, SpecificEnergy, Pressure >;
		friend class ZeroIn < SteamCalculator, Pressure, Density >;

		METHOD_SC_NUM(tau_iaps85);
		METHOD_SC_NUM(del_iaps85);
		METHOD_SC_NUM(pi_iaps85);
		METHOD_SC_NUM(delpi_iaps85);
		METHOD_SC_NUM(pitau_iaps85);

		METHOD_SC_NUM(lam);	// for thermal conductivity.
		METHOD_SC_NUM(lam0);
		METHOD_SC_NUM(lam1);
		METHOD_SC_NUM(lam2);

		METHOD_SC_NUM(mu);
		METHOD_SC_NUM(mu0);
		METHOD_SC_NUM(mu1);
		METHOD_SC_NUM(mu2);

		Density rhoerror_T_reg1(Temperature T);
		Density rhoerror_T_reg2(Temperature T);
		SpecificEnergy uerror_p(Pressure p);

		Pressure getRegion3PressureError(Density test_rho);

		// All the intrinsic properties must be stored here because the states are singletons to save memory.

		Temperature T;		///< temperature K
		Pressure p;			///< pressure MPa
		Num x;		///< quality
		Num tau;		///< internal variable in IAPWS calcs
		Num pi;		///< internal variable in IAPWS calcs
		Num del;		///< internal variable in IAPWS calcs
		Density rho;		///< density
		Pressure reg3_target_pressure;	///< internal variable used in root-finding in region 3

	private:

		bool isset;		///< boolean flag @see isSet()

		void destroy();
		
		template <class Abscissa,class Ordinate>
		void accept(ZeroIn<SteamCalculator, Abscissa, Ordinate> *z){
			z->visit(this);
		}

		SteamState *_state;

		SteamCalculator *gas;	// for use in two-phase calcs
		SteamCalculator *liq;	// for use in two-phase calcs

		SteamPropertyCode zeroin_property;
		Num zeroin_target_value;
		SteamStateCode zeroin_asserted_state;

		// for set_rhou:
		Density rho_target;
		SpecificEnergy u_target;
		Temperature T_guess;
		bool rhou_region_1;


};

Pressure reg3_pres_err(double);

double steamcalculator_wrap_pressolve(Pressure p);
double steamcalculator_wrap_tempsolve(Temperature T);


#endif
