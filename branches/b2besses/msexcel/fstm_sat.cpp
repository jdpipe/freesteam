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

#include "fstm.h"
#include "cppinterface.h"

/*
 * Set steam state, test bounds and send error if any
 */
CellMatrix fstm_T_Result(double Temperature, fstm_PtrType_double freesteamFunction, double scale)
{
    double T = Temperature + 273.15 ;
    CellMatrix result(1,1) ;

    if ( ! ( ( T >= IAPWS97_TMIN ) && (T <= IAPWS97_TCRIT ) ) )
    {
        result(0,0) = ERROR_EXCEL_TYPE_NUM ;
    }
    else
    {
        result(0,0) = freesteamFunction(T) * scale ;
    }

    return result ;
}

/*
 * Set steam state, test bounds and send error if any
 */
CellMatrix fstm_px_Result(double Pressure, double x, fstm_PtrType_double_double freesteamFunction,
                          double scale)
{
    double p = Pressure * 1.0e5 ;
    CellMatrix result(1,1) ;

    if ( ! ( ( p >= 0.0 ) && (p <= IAPWS97_PCRIT ) && ( x >= 0.0) && ( x <= 1.0) ) )
    {
        result(0,0) = ERROR_EXCEL_TYPE_NUM ;
    }
    else
    {
        result(0,0) = freesteamFunction(p, x) * scale ;
    }

    return result ;
}

/*
 * Set steam state, test bounds and send error if any
 */
CellMatrix fstm_px2Tx_Result(double Pressure, double x, fstm_PtrType_double_double freesteamFunction,
                             double scale)
{
    double p = Pressure * 1.0e5 ;
    CellMatrix result(1,1) ;

    if ( ! ( ( p >= 0.0 ) && (p <= IAPWS97_PCRIT ) && ( x >= 0.0) && ( x <= 1.0) ) )
    {
        result(0,0) = ERROR_EXCEL_TYPE_NUM ;
    }
    else
    {
        double T = freesteam_region4_Tsat_p(p) ;
        result(0,0) = freesteamFunction(T, x) * scale ;
    }

    return result ;
}

/*
 * Set steam state, test bounds and send error if any
 */
CellMatrix fstm_Tx_Result(double Temperature, double x, fstm_PtrType_double_double freesteamFunction,
                          double scale)
{
    double T = Temperature + 273.15 ;
    CellMatrix result(1,1) ;

    if ( ! ( ( T >= IAPWS97_TMIN ) && (T <= IAPWS97_TCRIT ) && ( x >= 0.0) && ( x <= 1.0) ) )
    {
        result(0,0) = ERROR_EXCEL_TYPE_NUM ;
    }
    else
    {
        result(0,0) = freesteamFunction(T, x) * scale;
    }

    return result ;
}

/*
 * Set steam state, test bounds and send error if any
 */
CellMatrix fstm_p2T_Result(double Pressure, fstm_PtrType_double freesteamFunction,
                           double scale)
{
    double p = Pressure * 1.0e5 ;
    CellMatrix result(1,1) ;

    if ( ! ( ( p >= 0.0 ) && (p <= IAPWS97_PCRIT ) ) )
    {
        result(0,0) = ERROR_EXCEL_TYPE_NUM ;
    }
    else
    {
        double T = freesteam_region4_Tsat_p(p) ;
        result(0,0) = freesteamFunction(T) * scale ;
    }

    return result ;
}

/*
 * Saturation temperature ( C)
 */
CellMatrix fstm_Tsat_p(double Pressure)
{
    double p = Pressure * 1.0e5 ;
    CellMatrix result(1,1) ;

    if ( ! ( ( p >= 0.0 ) && (p <= IAPWS97_PCRIT ) ) )
    {
        result(0,0) = ERROR_EXCEL_TYPE_NUM ;
    }
    else
    {
        result(0,0) = freesteam_region4_Tsat_p(p) - 273.15 ;
    }

    return result ;
}

/*
 * Saturation pressure (bar)
 */
CellMatrix fstm_psat_T(double Temperature)
{
    return fstm_T_Result(Temperature, &freesteam_region4_psat_T, 1.0e-5) ;
}

/*
 * Liquid Enthalpy at saturation (kJ/kg)
 */
CellMatrix fstm_hf_T(double Temperature)
{
    return fstm_Tx_Result(Temperature, 0.0, freesteam_region4_h_Tx, 0.001) ;
}

/*
 * Vapour Enthalpy at saturation (kJ/kg)
 */
CellMatrix fstm_hg_T(double Temperature)
{
    return fstm_Tx_Result(Temperature, 1.0, freesteam_region4_h_Tx, 0.001) ;
}

/*
 * Liquid Enthalpy at saturation (kJ/kg)
 */
CellMatrix fstm_hf_p(double Pressure)
{
    return fstm_px2Tx_Result(Pressure, 0.0, &freesteam_region4_h_Tx, 0.001) ;
}

/*
 * Vapour Enthalpy at saturation (kJ/kg)
 */
CellMatrix fstm_hg_p(double Pressure)
{
    return fstm_px2Tx_Result(Pressure, 1.0, &freesteam_region4_h_Tx, 0.001) ;
}

/*
 * Enthalpy at saturation (kJ/kg)
 */
CellMatrix fstm_h_px(double Pressure, double SteamQuality)
{
    return fstm_px2Tx_Result(Pressure, SteamQuality, &freesteam_region4_h_Tx, 0.001) ;
}

/*
 * Enthalpy at saturation (kJ/kg)
 */
CellMatrix fstm_h_Tx(double Temperature, double SteamQuality)
{
    return fstm_Tx_Result(Temperature, SteamQuality, freesteam_region4_h_Tx, 0.001) ;
}

/*
 * Internal energy at saturation (kJ/kg)
 */
CellMatrix fstm_u_px(double Pressure, double SteamQuality)
{
    return fstm_px2Tx_Result(Pressure, SteamQuality, &freesteam_region4_u_Tx, 0.001) ;
}

/*
 * Internal energy at saturation (kJ/kg)
 */
CellMatrix fstm_u_Tx(double Temperature, double SteamQuality)
{
    return fstm_Tx_Result(Temperature, SteamQuality, freesteam_region4_u_Tx, 0.001) ;
}

/*
 * Specific volume at saturation (m3/kg)
 */
CellMatrix fstm_v_px(double Pressure, double SteamQuality)
{
    return fstm_px2Tx_Result(Pressure, SteamQuality, &freesteam_region4_v_Tx, 1.0) ;
}

/*
 * Specific volume at saturation (m3/kg)
 */
CellMatrix fstm_v_Tx(double Temperature, double SteamQuality)
{
    return fstm_Tx_Result(Temperature, SteamQuality, freesteam_region4_v_Tx, 1.0) ;
}

/*
 * Liquid Density at saturation (kg/m3)
 */
CellMatrix fstm_rhof_T(double Temperature)
{
    return fstm_T_Result(Temperature, &freesteam_region4_rhof_T, 1.0) ;
}

/*
 * Vapour Density at saturation (kg/m3)
 */
CellMatrix fstm_rhog_T(double Temperature)
{
    return fstm_T_Result(Temperature, &freesteam_region4_rhog_T, 1.0) ;
}

/*
 * Entropy at saturation (kJ/kg.K)
 */
CellMatrix fstm_s_Tx(double Temperature, double SteamQuality)
{
    return fstm_Tx_Result(Temperature, SteamQuality, freesteam_region4_s_Tx, 0.001) ;
}

/*
 * Isobaric heat capacity at saturation (kJ/kg.K)
 */
CellMatrix fstm_cp_Tx(double Temperature, double SteamQuality)
{
    return fstm_Tx_Result(Temperature, SteamQuality, freesteam_region4_cp_Tx, 0.001) ;
}

/*
 * Isochoric heat capacity at saturation (kJ/kg.K)
 */
CellMatrix fstm_cv_Tx(double Temperature, double SteamQuality)
{
    return fstm_Tx_Result(Temperature, SteamQuality, freesteam_region4_cv_Tx, 0.001) ;
}

/*
 * Liquid Density at saturation (kg/m3)
 */
CellMatrix fstm_rhof_p(double Pressure)
{
    return fstm_p2T_Result(Pressure, &freesteam_region4_rhof_T, 1.0) ;
}

/*
 * Vapour Density at saturation (kg/m3)
 */
CellMatrix fstm_rhog_p(double Pressure)
{
    return fstm_p2T_Result(Pressure, &freesteam_region4_rhog_T, 1.0) ;
}

/*
 * Entropy at saturation (kJ/kg.K)
 */
CellMatrix fstm_s_px(double Pressure, double SteamQuality)
{
    return fstm_px2Tx_Result(Pressure, SteamQuality, &freesteam_region4_s_Tx, 0.001) ;
}

/*
 * /sobaric heat capacity at saturation (kJ/kg.K)
 */
CellMatrix fstm_cp_px(double Pressure, double SteamQuality)
{
    return fstm_px2Tx_Result(Pressure, SteamQuality, &freesteam_region4_cp_Tx, 0.001) ;
}

/*
 * Isochoric heat capacity at saturation (kJ/kg.K)
 */
CellMatrix fstm_cv_px(double Pressure, double SteamQuality)
{
    return fstm_px2Tx_Result(Pressure, SteamQuality, &freesteam_region4_cv_Tx, 0.001) ;
}

/*
 * Surface tension between water phase and vapour phase at saturation (N/m)
 */
CellMatrix fstm_surftens_T(double Temperature)
{
    return fstm_T_Result(Temperature, &freesteam_surftens_T, 1.0) ;
}

/*
 * Surface tension between water phase and vapour phase at saturation (N/m)
 */
CellMatrix fstm_surftens_p(double Pressure)
{
    return fstm_p2T_Result(Pressure, &freesteam_surftens_T, 1.0) ;
}
