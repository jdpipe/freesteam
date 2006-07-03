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

extern "C"{
#include <utilities/ascConfig.h>
#include <utilities/error.h>

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
}

/* return 0 on success */
int phmu_uv_calc(struct Slv_Interp *slv_interp,
		int ninputs, int noutputs,
		double *inputs, double *outputs,
		double *jacobian
){
	Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0> SS;

	(void)jacobian; // not used
	(void)slv_interp; // not used

	ASSERT(ninputs==2);
	ASSERT(noutputs==3);

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
		outputs[2] = S.dynvisc() / (Pascal*second);
	
		return 0; /* success */
	}catch(std::exception &e){
		// report error message using the ASCEND error reporting system
		ERROR_REPORTER_HERE(ASC_PROG_ERR,e.what());
		return 1; /* failure */
	}
	return 0;
}


extern "C"{ // start of C-accessible portion

	extern ASC_EXPORT(int) freesteam_register(){
		// help string
		const char *help = "Evaluate pressure, specific enthalpy"
			" and dynamic viscosity in terms of specific volume and specific"
			" internal energy, using the IAPWS-IF97 steam table correlations"
			" (http://freesteam.sf.net).";
		int result = 0;

		ERROR_REPORTER_HERE(ASC_PROG_NOTE,"Initialising freesteam...\n");

		result += CreateUserFunctionBlackBox("iapws97_phmu_rhou"
			, NULL /* alloc */
			, phmu_uv_calc /* value */
			, (ExtBBoxFunc*)NULL /* deriv */
			, (ExtBBoxFunc*)NULL /* deriv2 */
			, NULL /* free */
			, 2,3 /* inputs, outputs */
			, help
		);

		//ERROR_REPORTER_HERE(ASC_PROG_NOTE,"CreateUserFunction result = %d\n",result);
		return result;
	}

} // end of C-accessible portion 
