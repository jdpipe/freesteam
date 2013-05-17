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

#ifndef CPPINTERFACE_H
#define CPPINTERFACE_H

#include <xlw/MyContainers.h>
#include <xlw/CellMatrix.h>
#include <xlw/DoubleOrNothing.h>
#include <xlw/ArgList.h>

using namespace xlw;

//<xlw:libraryname=Freesteam2.x

double //Enthalpy (kJ/kg)
fstm_h_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

double //Entropy (kJ/kg.K)
fstm_s_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

double //Isobaric heat capacity (kJ/kg.K)
fstm_cp_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

double //Isochoric heat capacity (kJ/kg.K)
fstm_cv_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

double //Specific volume (m3/kg)
fstm_v_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

double //Density (kg/m3)
fstm_rho_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

double //Internal energy (kJ/kg)
fstm_u_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

double //Thermal conductivity (W/m.K)
fstm_k_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

double //Dynamic viscosity (Pa.s)
fstm_mu_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

double //Speed of sound (m/s)
fstm_w_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

int //Region of IAPWS-IF97
fstm_region_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
);

double //Saturation temperature (°C)
fstm_Tsat_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

double //Saturation pressure (bar)
fstm_psat_T(
    double Temperature //0°C <= Temperature <= Tcrit(373.946°C)
);

double //Liquid Enthalpy at saturation (kJ/kg)
fstm_hf_T(
    double Temperature //0°C <= Temperature <= Tcrit(373.946°C)
);

double //Vapour Enthalpy at saturation (kJ/kg)
fstm_hg_T(
    double Temperature //0°C <= Temperature <= Tcrit(373.946°C)
);

double //Liquid Enthalpy at saturation (kJ/kg)
fstm_hf_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

double //Vapour Enthalpy at saturation (kJ/kg)
fstm_hg_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

double //Enthalpy at saturation (kJ/kg)
fstm_h_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double x //Steam Quality (kg/kg), 0 <= x <= 1
);

double //Enthalpy at saturation (kJ/kg)
fstm_h_Tx(
    double Temperature, //0°C <= Temperature <= Tcrit(373.946°C)
    double x //Steam Quality (kg/kg), 0 <= x <= 1
);

double //Internal energy at saturation (kJ/kg)
fstm_u_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double x //Steam Quality (kg/kg), 0 <= x <= 1
);

double //Internal energy at saturation (kJ/kg)
fstm_u_Tx(
    double Temperature, //0°C <= Temperature <= Tcrit(373.946°C)
    double x //Steam Quality (kg/kg), 0 <= x <= 1
);

double //Specific volume at saturation (m3/kg)
fstm_v_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double x //Steam Quality (kg/kg), 0 <= x <= 1
);

double //Specific volume at saturation (m3/kg)
fstm_v_Tx(
    double Temperature, //0°C <= Temperature <= Tcrit(373.946°C)
    double x //Steam Quality (kg/kg), 0 <= x <= 1
);

#endif
