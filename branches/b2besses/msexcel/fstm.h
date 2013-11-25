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

#ifndef FSTM_H
#define FSTM_H

#define FREESTEAM_BUILDING_LIB

#include <stdlib.h>

/*
 * C function declaration
 */
extern "C"
{
#include <steam.h>
#include <region4.h>
#include <steam_pT.h>
#include <steam_Tx.h>
#include <steam_ps.h>
#include <steam_ph.h>
#include <steam_Ts.h>
#include <steam_pv.h>
#include <steam_pu.h>
#include <steam_uv.h>
#include <viscosity.h>
#include <thcond.h>
#include <surftens.h>
}

/*
 * define Excel errors
 */
#define ERROR_EXCEL_TYPE_NULL CellValue::error_type(0)
#define ERROR_EXCEL_TYPE_DIV0 CellValue::error_type(7)
#define ERROR_EXCEL_TYPE_VALUE CellValue::error_type(15)
#define ERROR_EXCEL_TYPE_REF CellValue::error_type(23)
#define ERROR_EXCEL_TYPE_NAME CellValue::error_type(29)
#define ERROR_EXCEL_TYPE_NUM CellValue::error_type(36)
#define ERROR_EXCEL_TYPE_NA CellValue::error_type(42)

/*
 * freesteamFunction pointer type
 */
typedef double (*varPtrType)(SteamState) ;

double fstm_psat_P(double Pressure) ;

double fstm_Tsat_T(double Temperature) ;

void fstm_x_check(double x) ;

SteamState fstm_set_ps(double Pressure, double Entropy) ;

SteamState fstm_set_ph(double Pressure, double Enthalpy) ;

SteamState fstm_set_Ts(double Temperature, double Entropy) ;

SteamState fstm_set_pu(double Pressure, double InternalEnergy) ;

SteamState fstm_set_pv(double Pressure, double SpecificVolume) ;

#endif
