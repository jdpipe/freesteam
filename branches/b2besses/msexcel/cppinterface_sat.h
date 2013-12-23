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

#ifndef CPPINTERFACE_SAT_H
#define CPPINTERFACE_SAT_H

#include <xlw/MyContainers.h>
#include <xlw/CellMatrix.h>
#include <xlw/ArgList.h>

using namespace xlw;

//<xlw:libraryname=Freesteam 2.x (sat.)

CellMatrix //Saturation Temperature ( C)
//<xlw:threadsafe
fstm_Tsat_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

CellMatrix //Saturation Pressure (bar)
//<xlw:threadsafe
fstm_psat_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
);

CellMatrix //Liquid Enthalpy at saturation (kJ/kg)
//<xlw:threadsafe
fstm_hf_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
);

CellMatrix //Vapour Enthalpy at saturation (kJ/kg)
//<xlw:threadsafe
fstm_hg_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
);

CellMatrix //Liquid Enthalpy at saturation (kJ/kg)
//<xlw:threadsafe
fstm_hf_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

CellMatrix //Vapour Enthalpy at saturation (kJ/kg)
//<xlw:threadsafe
fstm_hg_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

CellMatrix //Enthalpy at saturation (kJ/kg)
//<xlw:threadsafe
fstm_h_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Enthalpy at saturation (kJ/kg)
//<xlw:threadsafe
fstm_h_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Internal energy at saturation (kJ/kg)
//<xlw:threadsafe
fstm_u_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Internal energy at saturation (kJ/kg)
//<xlw:threadsafe
fstm_u_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Specific volume at saturation (m3/kg)
//<xlw:threadsafe
fstm_v_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Specific volume at saturation (m3/kg)
//<xlw:threadsafe
fstm_v_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Liquid Density at saturation (kg/m3)
//<xlw:threadsafe
fstm_rhof_T(
            double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
);

CellMatrix //Vapour Density at saturation (kg/m3)
//<xlw:threadsafe
fstm_rhog_T(
    double Temperature//0 C <= Temperature <= Tcrit(373.946 C)
);

CellMatrix //Entropy at saturation (kJ/kg.K)
//<xlw:threadsafe
fstm_s_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Isobaric heat capacity at saturation (kJ/kg.K)
//<xlw:threadsafe
fstm_cp_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Isochoric heat capacity at saturation (kJ/kg.K)
//<xlw:threadsafe
fstm_cv_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Liquid Density at saturation (kg/m3)
//<xlw:threadsafe
fstm_rhof_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

CellMatrix //Vapour Density at saturation (kg/m3)
//<xlw:threadsafe
fstm_rhog_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

CellMatrix //Entropy at saturation (kJ/kg.K)
//<xlw:threadsafe
fstm_s_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Isobaric heat capacity at saturation (kJ/kg.K)
//<xlw:threadsafe
fstm_cp_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Isochoric heat capacity at saturation (kJ/kg.K)
//<xlw:threadsafe
fstm_cv_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

CellMatrix //Surface tension between water phase and vapour phase at saturation (N/m)
//<xlw:threadsafe
fstm_surftens_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
);

CellMatrix //Surface tension between water phase and vapour phase at saturation (N/m)
//<xlw:threadsafe
fstm_surftens_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

#endif
