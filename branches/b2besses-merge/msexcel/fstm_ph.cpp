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
 * Temperature (C)
 */
double //Temperature (C)
fstm_T_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_T(S) - 273.15 )  ;
}

/*
 * Entropy (kJ/kg.K)
 */
double //Entropy (kJ/kg.K)
fstm_s_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_s(S) * 0.001 )  ;
}

/*
 * Isobaric heat capacity (kJ/kg.K)
 */
double //Isobaric heat capacity (kJ/kg.K)
fstm_cp_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_cp(S) * 0.001 )  ;
}

/*
 * Isochoric heat capacity (kJ/kg.K)
 */
double //Isochoric heat capacity (kJ/kg.K)
fstm_cv_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_cv(S) * 0.001 )  ;
}

/*
 * Specific volume (m3/kg)
 */
double //Specific volume (m3/kg)
fstm_v_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_v(S) )  ;
}

/*
 * Density (kg/m3)
 */
double //Density (kg/m3)
fstm_rho_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_rho(S) )  ;
}

/*
 * Internal energy (kJ/kg)
 */
double //Internal energy (kJ/kg)
fstm_u_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_u(S) * 0.001)  ;
}

/*
 * Thermal conductivity (W/m.K)
 */
double //Thermal conductivity (W/m.K)
fstm_k_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_k(S) )  ;
}

/*
 * Dynamic viscosity (Pa.s)
 */
double //Dynamic viscosity (Pa.s)
fstm_mu_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_mu(S) )  ;
}

/*
 * Speed of sound (m/s)
 */
double //Speed of sound (m/s)
fstm_w_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_w(S)  ) ;
}

/*
 * Region of IAPWS-IF97
 */
int //Region of IAPWS-IF97
fstm_region_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_region(S) ) ;
}

/*
 * Steam quality (kg/kg)
 */
double //Steam quality (kg/kg)
fstm_x_ph(
    double Pressure, //0 bar <= Pressure <= 1000 bar
    double Enthalpy //Enthalpy (kJ/kg.K)
)
{
    SteamState S = fstm_set_ph(Pressure, Enthalpy) ;
    return ( freesteam_x(S) ) ;
}

/*
 * Set steam state, test bounds and send error if any
 */
SteamState fstm_set_ph(double Pressure, double Enthalpy)
{
    double p = Pressure * 1.0e5 ;
    double h = Enthalpy * 1000.0 ;

    SteamState S = freesteam_set_ph(p, h) ;
    double T = freesteam_T(S) ;

    if ( ! ( ( p >= 0.0 ) && (p <= IAPWS97_PMAX ) && ( T >= IAPWS97_TMIN) && ( T <= IAPWS97_TMAX) ) )
    {
        throw("#ERROR Pressure/Enthalpy out of bounds") ;
    }

    return ( S ) ;
}
