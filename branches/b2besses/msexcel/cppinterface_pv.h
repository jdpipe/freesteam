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

#ifndef CPPINTERFACE_PV_H
#define CPPINTERFACE_PV_H

#include <xlw/MyContainers.h>
#include <xlw/CellMatrix.h>
#include <xlw/ArgList.h>

using namespace xlw;

//<xlw:libraryname=Freesteam 2.x (p,v)

CellMatrix //Enthalpy (kJ/kg)
//<xlw:threadsafe
fstm_h_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Entropy (kJ/kg.K)
//<xlw:threadsafe
fstm_s_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Isobaric heat capacity (kJ/kg.K)
//<xlw:threadsafe
fstm_cp_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Isochoric heat capacity (kJ/kg.K)
//<xlw:threadsafe
fstm_cv_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Temperature (C)
//<xlw:threadsafe
fstm_T_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Density (kg/m3)
//<xlw:threadsafe
fstm_rho_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Internal energy (kJ/kg)
//<xlw:threadsafe
fstm_u_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Thermal conductivity (W/m.K)
//<xlw:threadsafe
fstm_k_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Dynamic viscosity (Pa.s)
//<xlw:threadsafe
fstm_mu_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Speed of sound (m/s)
//<xlw:threadsafe
fstm_w_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Region of IAPWS-IF97
//<xlw:threadsafe
fstm_region_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

CellMatrix //Steam Quality (kg/kg)
//<xlw:threadsafe
fstm_x_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

#endif
