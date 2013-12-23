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

#ifndef CPPINTERFACE_PT_H
#define CPPINTERFACE_PT_H

#include <xlw/MyContainers.h>
#include <xlw/CellMatrix.h>
#include <xlw/ArgList.h>

using namespace xlw;

//<xlw:libraryname=Freesteam 2.x (p,T)

CellMatrix //Enthalpy (kJ/kg)
//<xlw:threadsafe
fstm_h_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

CellMatrix //Entropy (kJ/kg.K)
//<xlw:threadsafe
fstm_s_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

CellMatrix //Isobaric heat capacity (kJ/kg.K)
//<xlw:threadsafe
fstm_cp_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

CellMatrix //Isochoric heat capacity (kJ/kg.K)
//<xlw:threadsafe
fstm_cv_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

CellMatrix //Specific volume (m3/kg)
//<xlw:threadsafe
fstm_v_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

CellMatrix //Density (kg/m3)
//<xlw:threadsafe
fstm_rho_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

CellMatrix //Internal energy (kJ/kg)
//<xlw:threadsafe
fstm_u_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

CellMatrix //Thermal conductivity (W/m.K)
//<xlw:threadsafe
fstm_k_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

CellMatrix //Dynamic viscosity (Pa.s)
//<xlw:threadsafe
fstm_mu_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

CellMatrix //Speed of sound (m/s)
//<xlw:threadsafe
fstm_w_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

CellMatrix //Region of IAPWS-IF97
//<xlw:threadsafe
fstm_region_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

#endif
