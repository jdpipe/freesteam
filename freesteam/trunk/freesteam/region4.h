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
*/

#ifndef FREESTEAM_REGION4_H
#define FREESTEAM_REGION4_H

double freesteam_region4_Tsat_p(double p);
double freesteam_region4_psat_T(double T);

double freesteam_region4_v_px(double T, double x);
double freesteam_region4_u_px(double T, double x);
double freesteam_region4_h_px(double T, double x);
double freesteam_region4_s_px(double T, double x);



#endif

