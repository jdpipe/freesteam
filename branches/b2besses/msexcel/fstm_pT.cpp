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

/*
 * Enthalpy (kJ/kg)
 */
double //Enthalpy (kJ/kg)
fstm_h_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    SteamState S = fstm_set_pT(Pressure, Temperature) ;
    return ( freesteam_h(S) * 0.001 )  ;
}

/*
 * Entropy (kJ/kg.K)
 */
double //Entropy (kJ/kg.K)
fstm_s_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    SteamState S = fstm_set_pT(Pressure, Temperature) ;
    return ( freesteam_s(S) * 0.001 )  ;
}

/*
 * Isobaric heat capacity (kJ/kg.K)
 */
double //Isobaric heat capacity (kJ/kg.K)
fstm_cp_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    SteamState S = fstm_set_pT(Pressure, Temperature) ;
    return ( freesteam_cp(S) * 0.001 )  ;
}

/*
 * Isochoric heat capacity (kJ/kg.K)
 */
double //Isochoric heat capacity (kJ/kg.K)
fstm_cv_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    SteamState S = fstm_set_pT(Pressure, Temperature) ;
    return ( freesteam_cv(S) * 0.001 )  ;
}

/*
 * Specific volume (m3/kg)
 */
double //Specific volume (m3/kg)
fstm_v_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    SteamState S = fstm_set_pT(Pressure, Temperature) ;
    return ( freesteam_v(S) )  ;
}

/*
 * Density (kg/m3)
 */
double //Density (kg/m3)
fstm_rho_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    SteamState S = fstm_set_pT(Pressure, Temperature) ;
    return ( freesteam_rho(S) )  ;
}

/*
 * Internal energy (kJ/kg)
 */
double //Internal energy (kJ/kg)
fstm_u_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    SteamState S = fstm_set_pT(Pressure, Temperature) ;
    return ( freesteam_u(S) * 0.001)  ;
}

/*
 * Thermal conductivity (W/m.K)
 */
double //Thermal conductivity (W/m.K)
fstm_k_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    SteamState S = fstm_set_pT(Pressure, Temperature) ;
    return ( freesteam_k(S) )  ;
}

/*
 * Dynamic viscosity (Pa.s)
 */
double //Dynamic viscosity (Pa.s)
fstm_mu_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    SteamState S = fstm_set_pT(Pressure, Temperature) ;
    return ( freesteam_mu(S) )  ;
}

/*
 * Speed of sound (m/s)
 */
double //Speed of sound (m/s)
fstm_w_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    double w = 0.0 ;
    SteamState S = fstm_set_pT(Pressure, Temperature) ;

    if (freesteam_region(S) == 3)
    {
        w = 0.0 ;
        throw("#ERROR region 3 not implemented") ;
    }
    else
    {
        w = freesteam_w(S) ;
    }
    return ( w ) ;
}

/*
 * Region of IAPWS-IF97
 */
int //Region of IAPWS-IF97
fstm_region_pT(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Temperature //0°C <= Temperature <= 800°C
)
{
    SteamState S = fstm_set_pT(Pressure, Temperature) ;
    return ( freesteam_region(S) ) ;
}

/*
 * Set steam state, test bounds and send error if any
 */
SteamState fstm_set_pT(double Pressure, double Temperature)
{
    double p = Pressure * 1.0e5 ;
    double T = Temperature + 273.15 ;

    if ( ! ( ( p >= 0.0 ) && (p <= IAPWS97_PMAX ) && ( T >= IAPWS97_TMIN) && ( T <= IAPWS97_TMAX) ) )
    {
        throw("#ERROR Pressure/Temperature out of bounds") ;
    }
    return ( freesteam_set_pT(p, T) ) ;
}
