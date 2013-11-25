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
CellMatrix fstm_Ts_Result(double Temperature, double Entropy,
                          fstm_PtrType_S freesteamFunction, double scale)
{
    double T = Temperature + 273.15 ;
    double s = Entropy * 1000.0 ;
    SteamState S = freesteam_set_Ts(T, s) ;
    double p = freesteam_p(S) ;
    CellMatrix result(1,1) ;

    if ( ! ( ( p >= 0.0 ) && (p <= IAPWS97_PMAX ) && ( T >= IAPWS97_TMIN) && ( T <= IAPWS97_TMAX) ) )
    {
        result(0,0) = ERROR_EXCEL_TYPE_NUM ;
    }
    else
    {
        result(0,0) = freesteamFunction(S) * scale ;
    }

    return result ;
}

/*
 * Enthalpy (kJ/kg)
 */
CellMatrix fstm_h_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_h, 0.001) ;
}

/*
 * Pressure (bar)
 */
CellMatrix fstm_p_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_p, 1.0e-5) ;
}

/*
 * Isobaric heat capacity (kJ/kg.K)
 */
CellMatrix fstm_cp_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_cp, 0.001) ;
}

/*
 * Isochoric heat capacity (kJ/kg.K)
 */
CellMatrix fstm_cv_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_cv, 0.001) ;
}

/*
 * Specific volume (m3/kg)
 */
CellMatrix //Specific volume (m3/kg)
fstm_v_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_v, 1.0) ;
}

/*
 * Density (kg/m3)
 */
CellMatrix fstm_rho_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_rho, 1.0) ;
}

/*
 * Internal energy (kJ/kg)
 */
CellMatrix fstm_u_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_u, 0.001) ;
}

/*
 * Thermal conductivity (W/m.K)
 */
CellMatrix //Thermal conductivity (W/m.K)
fstm_k_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_k, 1.0) ;
}

/*
 * Dynamic viscosity (Pa.s)
 */
CellMatrix fstm_mu_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_mu, 1.0) ;
}

/*
 * Speed of sound (m/s)
 */
CellMatrix fstm_w_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_w, 1.0) ;
}

/*
 * Region of IAPWS-IF97
 */
CellMatrix fstm_region_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_region_double, 1.0) ;
}

/*
 * Steam quality (kg/kg)
 */
CellMatrix fstm_x_Ts(double Temperature, double Entropy)
{
    return fstm_Ts_Result(Temperature, Entropy, &freesteam_x, 1.0) ;
}
