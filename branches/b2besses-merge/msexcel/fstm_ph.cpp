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
 * Convert units, set steam state, test bounds and send error if any
 */
CellMatrix fstm_ph_Result(double Pressure, double Enthalpy,
                          fstm_PtrType_S freesteamFunction, double scale, double offset)
{
    double p = Pressure * 1.0e5 ;
    double h = Enthalpy * 1000.0 ;
    SteamState S = freesteam_set_ph(p, h) ;
    double T = freesteam_T(S) ;
    CellMatrix result(1,1) ;

    if ( ! ( ( p >= 0.0 ) && (p <= IAPWS97_PMAX ) && ( T >= IAPWS97_TMIN) && ( T <= IAPWS97_TMAX) ) )
    {
        result(0,0) = ERROR_EXCEL_TYPE_NUM ;
    }
    else
    {
        result(0,0) = freesteamFunction(S) * scale + offset ;
    }

    return result ;
}

/*
 * Temperature (C)
 */
CellMatrix fstm_T_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_T, 1.0, -273.15) ;
}

/*
 * Entropy (kJ/kg.K)
 */
CellMatrix fstm_s_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_s, 0.001, 0.0) ;
}

/*
 * Isobaric heat capacity (kJ/kg.K)
 */
CellMatrix fstm_cp_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_cp, 0.001, 0.0) ;
}

/*
 * Isochoric heat capacity (kJ/kg.K)
 */
CellMatrix fstm_cv_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_cv, 0.001, 0.0) ;
}

/*
 * Specific volume (m3/kg)
 */
CellMatrix fstm_v_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_v, 1.0 , 0.0) ;
}

/*
 * Density (kg/m3)
 */
CellMatrix fstm_rho_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_rho, 1.0 , 0.0) ;
}

/*
 * Internal energy (kJ/kg)
 */
CellMatrix fstm_u_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_u, 0.001, 0.0) ;
}

/*
 * Thermal conductivity (W/m.K)
 */
CellMatrix fstm_k_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_k, 1.0 , 0.0) ;
}

/*
 * Dynamic viscosity (Pa.s)
 */
CellMatrix fstm_mu_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_mu, 1.0 , 0.0) ;
}

/*
 * Speed of sound (m/s)
 */
CellMatrix fstm_w_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_w, 1.0 , 0.0) ;
}

/*
 * Region of IAPWS-IF97
 */
CellMatrix fstm_region_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_region_double, 1.0 , 0.0) ;
}

/*
 * Steam quality (kg/kg)
 */
CellMatrix fstm_x_ph(double Pressure, double Enthalpy)
{
    return fstm_ph_Result(Pressure, Enthalpy, &freesteam_x, 1.0 , 0.0) ;
}
