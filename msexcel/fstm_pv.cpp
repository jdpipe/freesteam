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
 * Enthalpy (kJ/kg)
 */
double //Enthalpy (kJ/kg)
fstm_h_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_h(S) * 0.001 )  ;
}

/*
 * Entropy (kJ/kg.K)
 */
double //Entropy (kJ/kg.K)
fstm_s_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_s(S) * 0.001 )  ;
}

/*
 * Isobaric heat capacity (kJ/kg.K)
 */
double //Isobaric heat capacity (kJ/kg.K)
fstm_cp_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_cp(S) * 0.001 )  ;
}

/*
 * Isochoric heat capacity (kJ/kg.K)
 */
double //Isochoric heat capacity (kJ/kg.K)
fstm_cv_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_cv(S) * 0.001 )  ;
}

/*
 * Temperature (C)
 */
double //Temperature (C)
fstm_T_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_T(S) ) - 273.15  ;
}

/*
 * Density (kg/m3)
 */
double //Density (kg/m3)
fstm_rho_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_rho(S) )  ;
}

/*
 * Internal energy (kJ/kg)
 */
double //Internal energy (kJ/kg)
fstm_u_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_u(S) * 0.001)  ;
}

/*
 * Thermal conductivity (W/m.K)
 */
double //Thermal conductivity (W/m.K)
fstm_k_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_k(S) )  ;
}

/*
 * Dynamic viscosity (Pa.s)
 */
double //Dynamic viscosity (Pa.s)
fstm_mu_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_mu(S) )  ;
}

/*
 * Speed of sound (m/s)
 */
double //Speed of sound (m/s)
fstm_w_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_w(S)  ) ;
}

/*
 * Region of IAPWS-IF97
 */
int //Region of IAPWS-IF97
fstm_region_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_region(S) ) ;
}

/*
 * Steam quality (kg/kg)
 */
double //Steam quality (kg/kg)
fstm_x_pv(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double SpecificVolume //Specific volume (m3/kg)
)
{
    SteamState S = fstm_set_pv(Pressure, SpecificVolume) ;
    return ( freesteam_x(S) ) ;
}

/*
 * Set steam state, test bounds and send error if any
 */
SteamState fstm_set_pv(double Pressure, double SpecificVolume)
{
    double p = Pressure * 1.0e5 ;

    SteamState S = freesteam_set_pv(p, SpecificVolume) ;

    double T = freesteam_T(S) + 273.15 ;

    if ( ! ( ( p >= 0.0 ) && (p <= IAPWS97_PMAX ) && ( T >= IAPWS97_TMIN) && ( T <= IAPWS97_TMAX) ) )
    {
        throw("#ERROR Pressure/Specific volume out of bounds") ;
    }
    return ( S ) ;
}
