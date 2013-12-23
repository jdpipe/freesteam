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

#ifndef CPPINTERFACE_TS_H
#define CPPINTERFACE_TS_H

#include <xlw/MyContainers.h>
#include <xlw/CellMatrix.h>
#include <xlw/ArgList.h>

using namespace xlw;

//<xlw:libraryname=Freesteam 2.x (T,s)

CellMatrix //Enthalpy (kJ/kg)
//<xlw:threadsafe
fstm_h_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Pressure (bar)
//<xlw:threadsafe
fstm_p_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Isobaric heat capacity (kJ/kg.K)
//<xlw:threadsafe
fstm_cp_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Isochoric heat capacity (kJ/kg.K)
//<xlw:threadsafe
fstm_cv_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Specific volume (m3/kg)
//<xlw:threadsafe
fstm_v_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Density (kg/m3)
//<xlw:threadsafe
fstm_rho_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Internal energy (kJ/kg)
//<xlw:threadsafe
fstm_u_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Thermal conductivity (W/m.K)
//<xlw:threadsafe
fstm_k_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Dynamic viscosity (Pa.s)
//<xlw:threadsafe
fstm_mu_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Speed of sound (m/s)
//<xlw:threadsafe
fstm_w_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Region of IAPWS-IF97
//<xlw:threadsafe
fstm_region_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

CellMatrix //Steam Quality (kg/kg)
//<xlw:threadsafe
fstm_x_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

#endif
