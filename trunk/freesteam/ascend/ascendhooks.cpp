/*	freesteam - IAPWS-IF97 steam tables
	Copyright (C) 2006 John Pye

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2, or (at your option)
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330,
	Boston, MA 02111-1307, USA.
*/

#include <stdexcept>

#include "../units.h"
#include "../steamcalculator.h"
#include "../solver2.h"
#include "../surfacetension.h"

extern "C"{
#include <utilities/ascConfig.h>
#include <utilities/error.h>

/*
#include <compiler/fractions.h>
#include <compiler/compiler.h>
#include <compiler/dimen.h>
#include <compiler/child.h>
#include <general/list.h>
#include <compiler/module.h>
#include <compiler/childinfo.h>
#include <compiler/slist.h>
#include <compiler/type_desc.h>
#include <compiler/packages.h>
*/
#include <compiler/extfunc.h>
}

/**
	ASCEND external evaluation function
	Inputs: p, h, T, s, mu 
	Outputs: u, v
	@return 0 on success 
*/
int phTsmu_uv_calc(struct BBoxInterp *slv_interp,
		int ninputs, int noutputs,
		double *inputs, double *outputs,
		double *jacobian
){
	Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> SS;

	(void)slv_interp; (void)jacobian; // not used

	ASSERT(ninputs==2);
	ASSERT(noutputs==5);

	// convert inputs to freesteam dimensionful values
	SpecificEnergy u = inputs[0] * J_kg;
	SpecificVolume v = inputs[1] * m3_kg;

	ERROR_REPORTER_HERE(ASC_USER_NOTE,
		"Evaluating with u = %f kJ/kg, v = %f m^3/kg"
		,double(u/kJ_kg),double(v/m3_kg)
	);

	try{
		// solve for the steam state specified
		SteamCalculator S = SS.solve(u,v);

		// evaluate and return properties in MKS units
		outputs[0] = S.pres() / Pascal;
		outputs[1] = S.specenthalpy() / J_kg;
		outputs[2] = S.temp() / Kelvin;
		outputs[3] = S.specentropy() / J_kgK;
		outputs[4] = S.dynvisc() / (Pascal*second);
	
		return 0; /* success */
	}catch(std::exception &e){
		// report error message using the ASCEND error reporting system
		ERROR_REPORTER_HERE(ASC_PROG_ERR,e.what());
		return 1; /* failure */
	}
	return 0;
}

/**
	ASCEND external evaluation function
	Inputs: p, h, T, s, mu 
	Outputs: u, v
	@return 0 on success 
*/
int phmu_uv_calc(struct BBoxInterp *slv_interp,
		int ninputs, int noutputs,
		double *inputs, double *outputs,
		double *jacobian
){
	Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> SS;

	(void)slv_interp; (void)jacobian; // not used

	ASSERT(ninputs==2);
	ASSERT(noutputs==3);

	// convert inputs to freesteam dimensionful values
	SpecificEnergy u = inputs[0] * J_kg;
	SpecificVolume v = inputs[1] * m3_kg;

	try{
		// solve for the steam state specified
		SteamCalculator S = SS.solve(u,v);

		// evaluate and return properties in MKS units
		outputs[0] = S.pres() / Pascal;
		outputs[1] = S.specenthalpy() / J_kg;
		outputs[1] = S.dynvisc() / (Pascal*second);
	
		return 0; /* success */
	}catch(std::exception &e){
		// report error message using the ASCEND error reporting system
		ERROR_REPORTER_HERE(ASC_PROG_ERR,e.what());
		return 1; /* failure */
	}
	return 0;
}


/**
	ASCEND external evaluation function
	Inputs: p, h
	Outputs: u, v, mu 
	@return 0 on success 
*/
int uvmux_ph_calc(struct BBoxInterp *slv_interp,
		int ninputs, int noutputs,
		double *inputs, double *outputs,
		double *jacobian
){
	Solver2<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY> SS;

	(void)slv_interp; (void)jacobian; // not used

	ASSERT(ninputs==2);
	ASSERT(noutputs==4);

	// convert inputs to freesteam dimensionful values
	Pressure p = inputs[0] * Pascal;
	SpecificEnergy h = inputs[1] * J_kg;

	try{
		// solve for the steam state specified
		SteamCalculator S = SS.solve(p,h);

		// evaluate and return properties in MKS units
		SpecificEnergy u = S.specienergy();
		SpecificVolume v = S.specvol();
		DynamicViscosity mu = S.dynvisc();
		double x = S.quality();

		outputs[0] = u / J_kg;
		outputs[1] = v / m3_kg;
		outputs[2] = mu  / (Pascal*second);
		outputs[3] = x;

		CONSOLE_DEBUG("p = %f bar, h = %f kJ/kg --> u = %f kJ/kg, v = %f m^3/kg, x = %f"
			, double(p / bar), double(h / kJ_kg)
			, double(u / kJ_kg), double(v / m3_kg)
			, x
		);
	
		return 0; /* success */
	}catch(std::exception &e){
		// report error message using the ASCEND error reporting system
		ERROR_REPORTER_HERE(ASC_PROG_ERR,e.what());
		return 1; /* failure */
	}
	return 0;
}


/**
	ASCEND external evaluation function
	Inputs: p, h
	Outputs: u, v, mu 
	@return 0 on success 
*/
int Ts_ph_calc(struct BBoxInterp *slv_interp,
		int ninputs, int noutputs,
		double *inputs, double *outputs,
		double *jacobian
){
	Solver2<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY> SS;

	(void)slv_interp; (void)jacobian; // not used

	ASSERT(ninputs==2);
	ASSERT(noutputs==2);

	// convert inputs to freesteam dimensionful values
	Pressure p = inputs[0] * Pascal;
	SpecificEnergy h = inputs[1] * J_kg;

	try{
		// solve for the steam state specified
		SteamCalculator S = SS.solve(p,h);

		// evaluate and return properties in MKS units
		Temperature T = S.temp();
		SpecificEntropy s = S.specentropy();

		outputs[0] = T / Kelvin;
		outputs[1] = s / J_kgK;

		CONSOLE_DEBUG("p = %f bar, h = %f kJ/kg --> T = %f K, s = %f kJ/kgK"
			, double(p / bar), double(h / kJ_kg)
			, double(T / Kelvin), double(s / kJ_kgK)
		);
	
		return 0; /* success */
	}catch(std::exception &e){
		// report error message using the ASCEND error reporting system
		ERROR_REPORTER_HERE(ASC_PROG_ERR,e.what());
		return 1; /* failure */
	}
	return 0;
}

/**
	ASCEND external evaluation function
	Inputs: p, h
	Outputs: u, v, mu 
	@return 0 on success 
*/
int ph_Ts_calc(struct BBoxInterp *slv_interp,
		int ninputs, int noutputs,
		double *inputs, double *outputs,
		double *jacobian
){
	Solver2<Temperature,SpecificEntropy,0,SOLVE_ENTROPY> SS;

	(void)slv_interp; (void)jacobian; // not used

	ASSERT(ninputs==2);
	ASSERT(noutputs==2);

	// convert inputs to freesteam dimensionful values
	Temperature T = inputs[0] * Kelvin;
	SpecificEntropy s = inputs[1] * J_kgK;

	try{
		// solve for the steam state specified
		SteamCalculator S = SS.solve(T,s);

		// evaluate and return properties in MKS units
		Pressure p = S.pres();
		SpecificEnergy h = S.specenthalpy();

		outputs[0] = p / Pascal;
		outputs[1] = h / J_kg;

		CONSOLE_DEBUG("T = %f K, s = %f kJ/kgK --> p = %f bar, h = %f kJ/kg"
			, double(T / Kelvin), double(s / kJ_kgK)
			, double(p / bar), double(h / kJ_kg)
		);
	
		return 0; /* success */
	}catch(std::exception &e){
		// report error message using the ASCEND error reporting system
		ERROR_REPORTER_HERE(ASC_PROG_ERR,e.what());
		return 1; /* failure */
	}
	return 0;
}

/**
	ASCEND external evaluation function
	Inputs: p, h
	Outputs: u, v, mu 
	@return 0 on success 
*/
int uvTxmu_ph_calc(struct BBoxInterp *slv_interp,
		int ninputs, int noutputs,
		double *inputs, double *outputs,
		double *jacobian
){
	Solver2<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY> SS;

	(void)slv_interp; (void)jacobian; // not used

	ASSERT(ninputs==2);
	ASSERT(noutputs==5);

	// convert inputs to freesteam dimensionful values
	Pressure p = inputs[0] * Pascal;
	SpecificEnergy h = inputs[1] * J_kg;

	try{
		// solve for the steam state specified
		SteamCalculator S = SS.solve(p,h);

		// evaluate and return properties in MKS units
		SpecificEnergy u = S.specienergy();
		SpecificVolume v = S.specvol();
		DynamicViscosity mu = S.dynvisc();
		Temperature T = S.temp();
		double x = S.quality();

		outputs[0] = u / J_kg;
		outputs[1] = v / m3_kg;
		outputs[2] = T / Kelvin;
		outputs[3] = x;
		outputs[4] = mu  / (Pascal*second);

		CONSOLE_DEBUG("p = %f bar, h = %f kJ/kg --> u = %f kJ/kg, v = %f m^3/kg, x = %f"
			, double(p / bar), double(h / kJ_kg)
			, double(u / kJ_kg), double(v / m3_kg)
			, x
		);
	
		return 0; /* success */
	}catch(std::exception &e){
		// report error message using the ASCEND error reporting system
		ERROR_REPORTER_HERE(ASC_PROG_ERR,e.what());
		return 1; /* failure */
	}
	return 0;
}


/**
	Surface tension
	ASCEND external evaluation function
	Inputs: T
	Outputs: sigma
	@return 0 on success 
*/
int sigma_T_calc(struct BBoxInterp *slv_interp,
		int ninputs, int noutputs,
		double *inputs, double *outputs,
		double *jacobian
){
	(void)slv_interp; (void)jacobian; // not used

	ASSERT(ninputs==1);
	ASSERT(noutputs==1);

	// convert inputs to freesteam dimensionful values
	Temperature T = inputs[0] * Kelvin;

	try{
		// evaluate and return properties in MKS units
		SurfaceTension sigma = surfaceTension(T);

		outputs[0] = sigma / (Newton/metre);

		return 0; /* success */
	}catch(std::exception &e){
		// report error message using the ASCEND error reporting system
		ERROR_REPORTER_HERE(ASC_PROG_ERR,e.what());
		return 1; /* failure */
	}
	return 0;
}

/**
	Region 4 steam properties
	ASCEND external evaluation function
	Inputs: T
	Outputs: sigma
	@return 0 on success 
*/
int iapws97_region4_calc(struct BBoxInterp *slv_interp,
		int ninputs, int noutputs,
		double *inputs, double *outputs,
		double *jacobian
){
	(void)slv_interp; (void)jacobian; // not used

	ASSERT(ninputs==2);
	ASSERT(noutputs==6);

	// convert inputs to freesteam dimensionful values
	Temperature T = inputs[0] * Kelvin;
	double x = inputs[1];

	try{
		// evaluate and return properties in MKS units
		SteamCalculator S;
		S.setRegion4_Tx(T,x);

		const SteamCalculator &liq = S.getLiquidPart();
		const SteamCalculator &gas = S.getGasPart();

		outputs[0] = S.pres() / Pascal;
		outputs[1] = S.specenthalpy() / J_kg;
		outputs[2] = liq.dens() / kg_m3;
		outputs[3] = gas.dens() / kg_m3;
		outputs[4] = liq.dynvisc() / (Pascal*second);
		outputs[5] = gas.dynvisc() / (Pascal*second);

		return 0; /* success */
	}catch(std::exception &e){
		// report error message using the ASCEND error reporting system
		ERROR_REPORTER_HERE(ASC_PROG_ERR,"%s",e.what());
		return 1; /* failure */
	}catch(...){
		ERROR_REPORTER_HERE(ASC_PROG_ERR,"unknown exception caught");
		return 1;
	}
	return 0;
}

extern "C"{ // start of C-accessible portion

	extern ASC_EXPORT(int) freesteam_register(){
		int result = 0;

		CONSOLE_DEBUG("Initialising freesteam...");

		result += CreateUserFunctionBlackBox("iapws97_phTsmu_uv"
			, NULL /* alloc */
			, phTsmu_uv_calc /* value */
			, NULL /* deriv */
			, NULL /* deriv2 */
			, NULL /* free */
			, 2,5 /* inputs, outputs */
			, "[p,h,T,s,mu] = iapws97_phTsmu_uv(u,v) (see http://freesteam.sf.net)"
			, 0.0
		);


		result += CreateUserFunctionBlackBox("iapws97_phmu_uv"
			, NULL /* alloc */
			, phmu_uv_calc /* value */
			, NULL /* deriv */
			, NULL /* deriv2 */
			, NULL /* free */
			, 2,3 /* inputs, outputs */
			, "[p,h,mu] = iapws97_phmu_uv(u,v) (see http://freesteam.sf.net)"
			, 0.0
		);

		result += CreateUserFunctionBlackBox("iapws97_uvmux_ph"
			, NULL /* alloc */
			, uvmux_ph_calc /* value */
			, NULL /* deriv */
			, NULL /* deriv2 */
			, NULL /* free */
			, 2,4 /* inputs, outputs */
			, "[u,v,mu,x] = iapws97_uvmu_ph(p,h) (see http://freesteam.sf.net)"
			, 0.0
		);

		result += CreateUserFunctionBlackBox("iapws97_Ts_ph"
			, NULL /* alloc */
			, Ts_ph_calc /* value */
			, NULL /* deriv */
			, NULL /* deriv2 */
			, NULL /* free */
			, 2,2 /* inputs, outputs */
			, "[T,s] = iapws97_Ts_ph(p,h) (see http://freesteam.sf.net)"
			, 0.0
		);

		result += CreateUserFunctionBlackBox("iapws97_ph_Ts"
			, NULL /* alloc */
			, ph_Ts_calc /* value */
			, NULL /* deriv */
			, NULL /* deriv2 */
			, NULL /* free */
			, 2,2 /* inputs, outputs */
			, "[p,h] = iapws97_ph_Ts(T,s) (see http://freesteam.sf.net)"
			, 0.0
		);

		result += CreateUserFunctionBlackBox("iapws97_uvTxmu_ph"
			, NULL /* alloc */
			, uvTxmu_ph_calc /* value */
			, NULL /* deriv */
			, NULL /* deriv2 */
			, NULL /* free */
			, 2,5 /* inputs, outputs */
			, "[u,v,T,x,mu] = iapws97_uvTxmu_ph(p,h) (see http://freesteam.sf.net)"
			, 0.0
		);

		result += CreateUserFunctionBlackBox("iapws_sigma_T"
			, NULL /* alloc */
			, sigma_T_calc /* value */
			, NULL /* deriv */
			, NULL /* deriv2 */
			, NULL /* free */
			, 1,1 /* inputs, outputs */
			, "[sigma] = iapws_sigma_T(T) (surface tension, see http://freesteam.sf.net)"
			, 0.0
		);			

		result += CreateUserFunctionBlackBox("iapws97_region4"
			, NULL /* alloc */
			, iapws97_region4_calc /* value */
			, NULL /* deriv */
			, NULL /* deriv2 */
			, NULL /* free */
			, 2,6 /* inputs, outputs */
			, "[p, h, rhof, rhog, muf, mug] = iapws97_region4(T,x)"
				"\n\tRegion 4 (saturated) steam properties"
				"\n\t(see http://freesteam.sf.net)"
			, 0.0
		);			

		return result;
	}

} // end of C-accessible portion 
