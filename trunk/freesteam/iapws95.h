/*************************************************************************

The thermodynamic properties of water calculated with the 
IAPWS95 equations.  Variables and units:
    
    T       Temperature, K
    rho     Density, kg/m^3
    p       Pressure, MPa
    u       Specific internal energy, kJ/kg
    h       Specific enthalpy, kJ/kg
    s       Specific entropy, kJ/(kg*K)
    cv      Isochoric specific heat, kJ/(kg*K)
    cp      Isobaric specific heat, kJ/(kg*K)
    w       Speed of sound, m/s

References:

    [1] The International Association for the Properties of
        Water and Steam, "Release on the IAPWS Formulation 1995
        for the Thermodynamic Properties of Ordinary Water
        Substance for General and Scientific Use", dated
        September 1996, Fredericia, Denmark.  See the "Releases"
        section of the website http://www.iapws.org/.

    [2] NIST Chemistry Webbook:  
        http://webbook.nist.gov/chemistry/fluid/

----------------------------------------------------------------------

Copyright (C) Don Peterson 2004

This program is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330,
Boston, MA 02111-1307  USA

*************************************************************************/

#ifndef IAPWS95_H
#define IAPWS95_H

#ifndef NUM_DEFINED
typedef double Num;
#endif

class Water95
{
    public:
        Water95();
        ~Water95() {}
        Num  p(const Num & T, const Num & rho);
        Num  u(const Num & T, const Num & rho);
        Num  h(const Num & T, const Num & rho);
        Num  s(const Num & T, const Num & rho);
        Num cp(const Num & T, const Num & rho);
        Num cv(const Num & T, const Num & rho);
        Num  w(const Num & T, const Num & rho);

        // Constants
        const Num & R;     // Universal gas constant, kJ/(kg*K)
        const Num & Tc;    // Water critical temperature, K
        const Num & rhoc;  // Water critical density, kg/m^3
};

#endif
