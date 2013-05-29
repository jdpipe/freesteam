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

//<xlw:libraryname=Freesteam 2.x

/*
 * fstm_pT ******************************************************************
 */

double //Enthalpy (kJ/kg)
fstm_h_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

double //Entropy (kJ/kg.K)
fstm_s_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

double //Isobaric heat capacity (kJ/kg.K)
fstm_cp_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

double //Isochoric heat capacity (kJ/kg.K)
fstm_cv_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

double //Specific volume (m3/kg)
fstm_v_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

double //Density (kg/m3)
fstm_rho_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

double //Internal energy (kJ/kg)
fstm_u_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

double //Thermal conductivity (W/m.K)
fstm_k_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

double //Dynamic viscosity (Pa.s)
fstm_mu_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

double //Speed of sound (m/s)
fstm_w_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

int //Region of IAPWS-IF97
fstm_region_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0 C <= Temperature <= 800 C
);

/*
 * fstm_sat ******************************************************************
 */

double //Saturation temperature ( C)
fstm_Tsat_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

double //Saturation pressure (bar)
fstm_psat_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
);

double //Liquid Enthalpy at saturation (kJ/kg)
fstm_hf_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
);

double //Vapour Enthalpy at saturation (kJ/kg)
fstm_hg_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
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
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Enthalpy at saturation (kJ/kg)
fstm_h_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Internal energy at saturation (kJ/kg)
fstm_u_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Internal energy at saturation (kJ/kg)
fstm_u_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Specific volume at saturation (m3/kg)
fstm_v_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Specific volume at saturation (m3/kg)
fstm_v_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Liquid Density at saturation (kg/m3)
fstm_rhof_T(
            double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
);

double //Vapour Density at saturation (kg/m3)
fstm_rhog_T(
    double Temperature//0 C <= Temperature <= Tcrit(373.946 C)
);

double //Entropy at saturation (kJ/kg.K)
fstm_s_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Isobaric heat capacity at saturation (kJ/kg.K)
fstm_cp_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Isochoric heat capacity at saturation (kJ/kg.K)
fstm_cv_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Liquid Density at saturation (kg/m3)
fstm_rhof_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

double //Vapour Density at saturation (kg/m3)
fstm_rhog_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

double //Entropy at saturation (kJ/kg.K)
fstm_s_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Isobaric heat capacity at saturation (kJ/kg.K)
fstm_cp_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Isochoric heat capacity at saturation (kJ/kg.K)
fstm_cv_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
);

double //Surface tension between water phase and vapour phase at saturation (N/m)
fstm_surftens_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
);

double //Surface tension between water phase and vapour phase at saturation (N/m)
fstm_surftens_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
);

/*
 * fstm_ps ******************************************************************
 */

double //Enthalpy (kJ/kg)
fstm_h_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
) ;

double //Temperature (C)
fstm_T_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

double //Isobaric heat capacity (kJ/kg.K)
fstm_cp_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

double //Isochoric heat capacity (kJ/kg.K)
fstm_cv_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

double //Specific volume (m3/kg)
fstm_v_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

double //Density (kg/m3)
fstm_rho_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

double //Internal energy (kJ/kg)
fstm_u_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

double //Thermal conductivity (W/m.K)
fstm_k_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

double //Dynamic viscosity (Pa.s)
fstm_mu_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

double //Speed of sound (m/s)
fstm_w_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

int //Region of IAPWS-IF97
fstm_region_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

double //Steam Quality (kg/kg)
fstm_x_ps(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Entropy //Entropy (kJ/kg.K)
);

/*
 * fstm_ph ******************************************************************
 */

double //Temperature (C)
fstm_T_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

double //Entropy (kJ/kg.K)
fstm_s_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

double //Isobaric heat capacity (kJ/kg.K)
fstm_cp_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

double //Isochoric heat capacity (kJ/kg.K)
fstm_cv_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

double //Specific volume (m3/kg)
fstm_v_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

double //Density (kg/m3)
fstm_rho_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

double //Internal energy (kJ/kg)
fstm_u_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

double //Thermal conductivity (W/m.K)
fstm_k_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

double //Dynamic viscosity (Pa.s)
fstm_mu_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

double //Speed of sound (m/s)
fstm_w_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

int //Region of IAPWS-IF97
fstm_region_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

double //Steam Quality (kg/kg)
fstm_x_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
);

/*
 * fstm_Ts ******************************************************************
 */

double //Enthalpy (kJ/kg)
fstm_h_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

double //Pressure (bar)
fstm_p_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

double //Isobaric heat capacity (kJ/kg.K)
fstm_cp_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

double //Isochoric heat capacity (kJ/kg.K)
fstm_cv_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

double //Specific volume (m3/kg)
fstm_v_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

double //Density (kg/m3)
fstm_rho_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

double //Internal energy (kJ/kg)
fstm_u_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

double //Thermal conductivity (W/m.K)
fstm_k_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

double //Dynamic viscosity (Pa.s)
fstm_mu_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

double //Speed of sound (m/s)
fstm_w_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

int //Region of IAPWS-IF97
fstm_region_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

double //Steam Quality (kg/kg)
fstm_x_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
);

 /*
 * fstm_pv ******************************************************************
 */

 double //Enthalpy (kJ/kg)
fstm_h_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

double //Entropy (kJ/kg.K)
fstm_s_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

double //Isobaric heat capacity (kJ/kg.K)
fstm_cp_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

double //Isochoric heat capacity (kJ/kg.K)
fstm_cv_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

double //Temperature (C)
fstm_T_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

double //Density (kg/m3)
fstm_rho_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

double //Internal energy (kJ/kg)
fstm_u_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

double //Thermal conductivity (W/m.K)
fstm_k_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

double //Dynamic viscosity (Pa.s)
fstm_mu_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

double //Speed of sound (m/s)
fstm_w_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

int //Region of IAPWS-IF97
fstm_region_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

double //Steam Quality (kg/kg)
fstm_x_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
);

#endif
