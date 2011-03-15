/*
freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2005  John Pye

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
*//** @file
	Function to calculate surface tension as a function of
	temperature.
*/

#ifndef FREESTEAM_SURFTENS_H
#define FREESTEAM_SURFTENS_H

#include "common.h"

FREESTEAM_DLL double freesteam_surftens_T(double T);

#endif /* FREESTEAM_SURFTENS_H */

