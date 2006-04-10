%module pyfreesteam

%{
#include "steamcalculator.h"
#include "solver2.h"
%}

%rename(steam) SteamCalculator;

class SteamCalculator{

	public:
		SteamCalculator();
		SteamCalculator(const SteamCalculator & original);
		SteamCalculator const &operator=(SteamCalculator const &original);
		~SteamCalculator();

		// Defining state, simple methods
		void set_pT(const Pressure &p, const Temperature &T, double x = 1.0);	// pressure [MPa]

		template<class F,class S>
		inline void set(const F &f, const S &s);

		void setSatSteam_p(const Pressure &p);	// p Pressure [MPa]
		void setSatWater_p(const Pressure &p);	// p Pressure [MPa]
		void setSatWater_T(const Temperature &T);	// T Temperature [K]
		void setSatSteam_T(const Temperature &T);	// T Temperature [K]

		void setB23_T(const Temperature &T);
		void setB23_p(const Pressure &p);

		void setRegion1_pT(const Pressure &p, const Temperature &T);
		void setRegion2_pT(const Pressure &p, const Temperature &T);
		void setRegion4_Tx(const Temperature &T, const Num &x);
		void setRegion3_rhoT(const Density &rho, const Temperature &T);

		bool isSet(void) const;

		// Methods to return properties and state

		int whichRegion(void) const;
		const SteamCalculator &getGasPart() const;
		const SteamCalculator &getLiquidPart() const;

		SteamStateCode whichState(void);
		const char *whichStateStr(void);

		Temperature temp() const;               // Temperature
		Pressure pres() const;                  // Pressure
		Density dens() const;                   // Density

		SpecificVolume specvol() const;         // Specific volume
		SpecificEnergy specienergy() const;     // Specific internal energy
		SpecificEntropy specentropy() const;    // Specific entropy
		SpecificEnergy specenthalpy() const;    // Specific entropy
		SpecHeatCap speccp() const;             // Specific isobaric heat capacity  [kJ/kgK]
		SpecHeatCap speccv() const;             // Specific isochoric heat capacity [kJ/kgK]
		Num quality() const;                    // Steam quality
		DynamicViscosity dynvisc() const;       // Dynamic viscosity, mu            [Pa.s]
		ThermalConductivity conductivity() const;// Conductivity
};


template<class FirstProp,class SecondProp,int FirstPropAlt=0, int SecondPropAlt=0>
class Solver2{
	public:
		Solver2();
		SteamCalculator solve(const FirstProp &fp, const SecondProp &sp);
};

%template(solver_ph) Solver2<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY>;
%template(solver_uv) Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0>;
%template(solver_Ts) Solver2<Temperature,SpecificEntropy,0,SOLVE_ENTROPY>;
%template(solver_ps) Solver2<Pressure,SpecificEntropy,0,SOLVE_ENTROPY>;
