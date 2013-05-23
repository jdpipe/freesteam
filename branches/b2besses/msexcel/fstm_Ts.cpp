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
fstm_h_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_h(S) * 0.001 )  ;
}

/*
 * Pressure (bar)
 */
double //Pressure (bar)
fstm_p_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_p(S) * 1e-5 )  ;
}

/*
 * Isobaric heat capacity (kJ/kg.K)
 */
double //Isobaric heat capacity (kJ/kg.K)
fstm_cp_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_cp(S) * 0.001 )  ;
}

/*
 * Isochoric heat capacity (kJ/kg.K)
 */
double //Isochoric heat capacity (kJ/kg.K)
fstm_cv_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_cv(S) * 0.001 )  ;
}

/*
 * Specific volume (m3/kg)
 */
double //Specific volume (m3/kg)
fstm_v_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_v(S) )  ;
}

/*
 * Density (kg/m3)
 */
double //Density (kg/m3)
fstm_rho_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_rho(S) )  ;
}

/*
 * Internal energy (kJ/kg)
 */
double //Internal energy (kJ/kg)
fstm_u_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_u(S) * 0.001)  ;
}

/*
 * Thermal conductivity (W/m.K)
 */
double //Thermal conductivity (W/m.K)
fstm_k_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_k(S) )  ;
}

/*
 * Dynamic viscosity (Pa.s)
 */
double //Dynamic viscosity (Pa.s)
fstm_mu_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_mu(S) )  ;
}

/*
 * Speed of sound (m/s)
 */
double //Speed of sound (m/s)
fstm_w_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_w(S)  ) ;
}

/*
 * Region of IAPWS-IF97
 */
int //Region of IAPWS-IF97
fstm_region_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_region(S) ) ;
}

/*
 * Steam quality (kg/kg)
 */
double //Steam quality (kg/kg)
fstm_x_Ts(
    double Temperature, //0 C <= Temperature <= 800 C
    double Entropy //Entropy (kJ/kg.K)
)
{
    SteamState S = fstm_set_Ts(Temperature, Entropy) ;
    return ( freesteam_x(S) ) ;
}

/*
 * Set steam state, test bounds and send error if any
 */
SteamState fstm_set_Ts(double Temperature, double Entropy)
{
    double T = Temperature + 273.15 ;
    double s = Entropy * 1000.0 ;

    SteamState S = freesteam_set_Ts(T, s) ;

    double p = freesteam_p( S );

    if ( ! ( ( p >= 0.0 ) && (p <= IAPWS97_PMAX ) && ( T >= IAPWS97_TMIN) && ( T <= IAPWS97_TMAX) ) )
    {
        throw("#ERROR Temperature/Entropy out of bounds") ;
    }
    return ( S ) ;
}
