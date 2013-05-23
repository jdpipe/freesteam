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
 * Saturation temperature ( C)
 */
double //Saturation temperature ( C)
fstm_Tsat_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
)
{
    double p = fstm_psat_P(Pressure) ;
    return ( freesteam_region4_Tsat_p( p ) - 273.15 )  ;
}

/*
 * Saturation pressure (bar)
 */
double //Saturation pressure (bar)
fstm_psat_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
)
{
    double T = fstm_Tsat_T(Temperature) ;
    return ( freesteam_region4_psat_T( T ) * 1.0e-5 )  ;
}

/*
 * Liquid Enthalpy at saturation (kJ/kg)
 */
double //Liquid Enthalpy at saturation (kJ/kg)
fstm_hf_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
)
{
    double T = fstm_Tsat_T(Temperature) ;
    return ( freesteam_region4_h_Tx( T, 0.0 ) * 0.001 )  ;
}

/*
 * Vapour Enthalpy at saturation (kJ/kg)
 */
double //Vapour Enthalpy at saturation (kJ/kg)
fstm_hg_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
)
{
    double T = fstm_Tsat_T(Temperature) ;
    return ( freesteam_region4_h_Tx( T, 1.0 ) * 0.001 )  ;
}

/*
 * Liquid Enthalpy at saturation (kJ/kg)
 */
double //Liquid Enthalpy at saturation (kJ/kg)
fstm_hf_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    return ( freesteam_region4_h_Tx( T, 0.0 ) * 0.001 )  ;
}

/*
 * Vapour Enthalpy at saturation (kJ/kg)
 */
double //Vapour Enthalpy at saturation (kJ/kg)
fstm_hg_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    return ( freesteam_region4_h_Tx( T, 1.0 ) * 0.001 )  ;
}

/*
 * Enthalpy at saturation (kJ/kg)
 */
double //Enthalpy at saturation (kJ/kg)
fstm_h_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_h_Tx( T, SteamQuality ) * 0.001 )  ;
}

/*
 * Enthalpy at saturation (kJ/kg)
 */
double //Enthalpy at saturation (kJ/kg)
fstm_h_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double T = fstm_Tsat_T(Temperature) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_h_Tx( T, SteamQuality ) * 0.001 )  ;
}

/*
 * Internal energy at saturation (kJ/kg)
 */
double //Internal energy at saturation (kJ/kg)
fstm_u_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_u_Tx( T, SteamQuality ) * 0.001 )  ;
}

/*
 * Internal energy at saturation (kJ/kg)
 */
double //Internal energy at saturation (kJ/kg)
fstm_u_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double T = fstm_Tsat_T(Temperature) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_u_Tx( T, SteamQuality ) * 0.001 )  ;
}

/*
 * Specific volume at saturation (m3/kg)
 */
double //Specific volume at saturation (m3/kg)
fstm_v_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_v_Tx( T, SteamQuality ) )  ;
}

/*
 * Specific volume at saturation (m3/kg)
 */
double //Specific volume at saturation (m3/kg)
fstm_v_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double T = fstm_Tsat_T(Temperature) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_v_Tx( T, SteamQuality ) )  ;
}

/*
 * Liquid Density at saturation (kg/m3)
 */
double //Liquid Density at saturation (kg/m3)
fstm_rhof_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
)
{
    double T = fstm_Tsat_T(Temperature) ;
    return ( freesteam_region4_rhof_T( T ) )  ;
}

/*
 * Vapour Density at saturation (kg/m3)
 */
double //Vapour Density at saturation (kg/m3)
fstm_rhog_T(
    double Temperature//0 C <= Temperature <= Tcrit(373.946 C)
)
{
    double T = fstm_Tsat_T(Temperature) ;
    return ( freesteam_region4_rhog_T( T ) )  ;
}

/*
 * Entropy at saturation (kJ/kg.K)
 */
double //Entropy at saturation (kJ/kg.K)
fstm_s_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double T = fstm_Tsat_T(Temperature) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_s_Tx( T, SteamQuality ) * 0.001 )  ;
}

/*
 * Isobaric heat capacity at saturation (kJ/kg.K)
 */
double //Isobaric heat capacity at saturation (kJ/kg.K)
fstm_cp_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double T = fstm_Tsat_T(Temperature) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_cp_Tx( T, SteamQuality ) * 0.001 )  ;
}

/*
 * Isochoric heat capacity at saturation (kJ/kg.K)
 */
double //Isochoric heat capacity at saturation (kJ/kg.K)
fstm_cv_Tx(
    double Temperature, //0 C <= Temperature <= Tcrit(373.946 C)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double T = fstm_Tsat_T(Temperature) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_cv_Tx( T, SteamQuality ) * 0.001 )  ;
}

/*
 * //Liquid Density at saturation (kg/m3)
 */
double //Liquid Density at saturation (kg/m3)
fstm_rhof_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    return ( freesteam_region4_rhof_T( T ) )  ;
}

/*
 * Vapour Density at saturation (kg/m3)
 */
double //Vapour Density at saturation (kg/m3)
fstm_rhog_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    return ( freesteam_region4_rhog_T( T ) )  ;
}

/*
 * Entropy at saturation (kJ/kg.K)
 */
double //Entropy at saturation (kJ/kg.K)
fstm_s_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_s_Tx( T,SteamQuality ) * 0.001 )  ;
}

/*
 * /sobaric heat capacity at saturation (kJ/kg.K)
 */
double //Isobaric heat capacity at saturation (kJ/kg.K)
fstm_cp_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_cp_Tx( T , SteamQuality ) * 0.001 ) ;
}

/*
 * Isochoric heat capacity at saturation (kJ/kg.K)
 */
double //Isochoric heat capacity at saturation (kJ/kg.K)
fstm_cv_px(
    double Pressure, //0 bar <= Pressure <= Pcrit(220.64 bar)
    double SteamQuality // 0 <= Steam Quality <= 1 kg/kg
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    fstm_x_check(SteamQuality) ;
    return ( freesteam_region4_cv_Tx( T, SteamQuality ) * 0.001 ) ;
}

/*
 * Surface tension between water phase and vapour phase at saturation (N/m)
 */
double //Surface tension between water phase and vapour phase at saturation (N/m)
fstm_surftens_T(
    double Temperature //0 C <= Temperature <= Tcrit(373.946 C)
)
{
    double T = fstm_Tsat_T(Temperature) ;
    return ( freesteam_surftens_T( T )  )  ;
}

/*
 * Surface tension between water phase and vapour phase at saturation (N/m)
 */
double //Surface tension between water phase and vapour phase at saturation (N/m)
fstm_surftens_p(
    double Pressure //0 bar <= Pressure <= Pcrit(220.64 bar)
)
{
    double p = fstm_psat_P(Pressure) ;
    double T = freesteam_region4_Tsat_p( p ) ;
    return ( freesteam_surftens_T( T ) )  ;
}


/*
 * Convert pressure and test bounds
 */
double fstm_psat_P(double Pressure)
{
    double p = Pressure * 1.0e5 ;

    if ( p < 0.0 )
    {
        throw("#ERROR Pressure below 0 bar") ;
    }
    else if ( p > IAPWS97_PCRIT )
    {
        throw("#ERROR Pressure above Pcrit(220.64 bar)") ;
    }
    return ( p ) ;
}

/*
 * Convert Temperature and test bounds
 */
double fstm_Tsat_T(double Temperature)
{
    double T = Temperature + 273.15 ;

    if ( T < IAPWS97_TMIN )
    {
        throw("#ERROR Temperature below 0 C") ;
    }
    else if ( T > IAPWS97_TCRIT )
    {
        throw("#ERROR Temperature above Tcrit(373.946 C)") ;
    }
    return ( T ) ;
}

/*
 * Check Quality Bounds
 */
void fstm_x_check(double x)
{
    if ( !( ( x >= 0.0) && ( x <= 1.0) ) )
        throw("#ERROR Steam quality out of bounds") ;

}
