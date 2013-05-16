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

/*
 * C function declaration
 */
extern "C"
{
#include <steam.h>
#include <region4.h>
#include <steam_pT.h>
#include <steam_Tx.h>
#include <viscosity.h>
#include <thcond.h>
}

SteamState fstm_set_pT(double Pressure, double Temperature) ;

double fstm_psat_P(double Pressure) ;

double fstm_Tsat_T(double Temperature) ;

void fstm_x_check(double x) ;

#endif
