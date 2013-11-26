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
 * define Excel error #NUM!
 */
#define ERROR_EXCEL_TYPE_NUM CellValue::error_type(36)

/*
 * Function pointer types
 */
typedef double (*fstm_PtrType_S)(SteamState) ;
typedef double (*fstm_PtrType_double)(double) ;
typedef double (*fstm_PtrType_double_double)(double, double) ;

/*
 * return freesteam_region as a double for fstm_PtrType_S compatibility
 */
double freesteam_region_double(SteamState S) ;

#endif
