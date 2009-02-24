/*
freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2009  John Pye

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
#include "../steam_ph.h"

#include <utilities/ascConfig.h>
#include <utilities/error.h>
#include <compiler/extfunc.h>

/**
	ASCEND external evaluation function
	Outputs: T
	Inputs: p, h
	@return 0 on success 
*/
int T_ph_calc(struct BBoxInterp *bbox,
		int ninputs, int noutputs,
		double *inputs, double *outputs,
		double *jacobian
){
	(void)bbox; (void)jacobian; // not used
	(void)ninputs; (void)noutputs; // not used currently

#if 0
	ASSERT(ninputs==2);
	ASSERT(noutputs==1);
#endif

	// convert inputs to freesteam dimensionful values
	double p = inputs[0]; /* ASCEND uses SI units, so no conversion needed */
	double h = inputs[1]; /* ASCEND uses SI units, so no conversion needed */
	
	ERROR_REPORTER_HERE(ASC_USER_NOTE,
		"Evaluating with p = %f bar, h = %f kJ/kg"
		,p,h
	);

	SteamState S;
	S = freesteam_set_ph(p,h);
	double T, dTdh_p, dTdp_h;

	switch(bbox->task){
	case bb_func_eval:
		T = freesteam_T(S);

		ERROR_REPORTER_HERE(ASC_USER_NOTE,
			"Got result T = %f K"
			,T
		);

		outputs[0] = T;
		/* TODO add error checks here, surely? */
		return 0;
	case bb_deriv_eval:
		dTdp_h = freesteam_deriv(S,'T','p','h');
		dTdh_p = freesteam_deriv(S,'T','h','p');
		ERROR_REPORTER_HERE(ASC_USER_NOTE,
			"Got result (∂T/∂p)h = %g, (∂T/∂h)p = %g K/Pa",dTdp_h,dTdh_p
		);
		jacobian[0] = dTdp_h;
		jacobian[1] = dTdh_p;
		return 0;
	default:
		ERROR_REPORTER_HERE(ASC_PROG_ERR,"Invalid call, unknown bbox->task");
		return 1;
	}
}

FREESTEAM_EXPORT int freesteam_register(){
		int result = 0;

		CONSOLE_DEBUG("Initialising freesteam...");

		result += CreateUserFunctionBlackBox("freesteam_T_ph"
			, NULL /* alloc */
			, T_ph_calc /* value */
			, /*T_ph_calc*/ NULL /* deriv */
			, NULL /* deriv2 */
			, NULL /* free */
			, 2,1 /* inputs, outputs */
			, "[T] = freesteam_T_ph(p,h) (see http://freesteam.sf.net)"
			, 0.0
		);

		return result;
}
