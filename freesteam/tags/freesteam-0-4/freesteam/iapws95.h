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
Modified by John Pye 2004

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
// When integrating with the IF97 stuff, Num is already defined so don't redefine it...
typedef double Num;
#endif

namespace IAPWS95
{
    // Universal gas constant in kJ/(kg*K)
    const Num R_    = 0.46151805;

    // Critical temperature of water in K
    const Num Tc_   = 647.096;

    // Critical density of water in kg/m^3
    const Num rhoc_ = 322;

    const Num n0[] = {
        0.0,  
        -8.32044648201, 6.6832105268, 3.00632, 0.012436, 
        0.97315, 1.27950, 0.96956, 0.24873
    };

    const Num gamma0[] = {
        0.0, 0.0, 0.0, 0.0,
        1.28728967,
        3.53734222,
        7.74073708,
        9.24437796,
        27.5075105
    };

    const Num n[] = { 0,
        0.12533547935523e-1,  0.78957634722828e+1, -0.87803203303561e+1,
        0.31802509345418e+0, -0.26145533859358e+0, -0.78199751687981e-2,
        0.88089493102134e-2, -0.66856572307965e+0,  0.20433810950965e+0,
        -0.66212605039687e-4, -0.19232721156002e+0, -0.25709043003438e+0,
        0.16074868486251e+0, -0.40092828925807e-1,  0.39343422603254e-6,
        -0.75941377088144e-5,  0.56250979351888e-3, -0.15608652257135e-4,
        0.11537996422951e-8,  0.36582165144204e-6, -0.13251180074668e-11,
        -0.62639586912454e-9, -0.10793600908932e+0,  0.17611491008752e-1,
        0.22132295167546e+0, -0.40247669763528e+0,  0.58083399985759e+0,
        0.49969146990806e-2, -0.31358700712549e-1, -0.74315929710341e+0,
        0.47807329915480e+0,  0.20527940895948e-1, -0.13636435110343e+0,
        0.14180634400617e-1,  0.83326504880713e-2, -0.29052336009585e-1,
        0.38615085574206e-1, -0.20393486513704e-1, -0.16554050063734e-2,
        0.19955571979541e-2,  0.15870308324157e-3, -0.16388568342530e-4,
        0.43613615723811e-1,  0.34994005463765e-1, -0.76788197844621e-1,
        0.22446277332006e-1, -0.62689710414685e-4, -0.55711118565645e-9,
        -0.19905718354408e+0,  0.31777497330738e+0, -0.11841182425981e+0,
        -0.31306260323435e+2,  0.31546140237781e+2, -0.25213154341695e+4,
        -0.14874640856724e+0,  0.31806110878444e+0
    };

    const Num c[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 3, 3, 3, 3, 4, 6, 6, 6, 6
    };

    const Num d[] = { 
        0,  1,  1,  1,  2,  2,  3,  4,  1,  1,  1,  2,  2,  3,  4,  
        4,  5,  7,  9, 10, 11, 13, 15,  1,  2,  2,  2,  3,  4,  4,
        4,  5,  6,  6,  7,  9,  9,  9,  9,  9, 10, 10, 12,  3,  4,
        4,  5, 14,  3,  6,  6,  6,  3,  3,  3
    };

    const Num t[] = { 0,
        -0.5,  0.875,  1,  0.5,  0.75,  0.375,  1,  4,  6, 12,  1,
        5,  4,  2, 13,  9,  3,  4, 11,  4, 13,  1,  7,  1,  9, 10,
        10,  3,  7, 10, 10,  6, 10, 10,  1,  2,  3,  4,  8,  6,  9,
        8, 16, 22, 23, 23, 10, 50, 44, 46, 50,  0,  1,  4
    };

    // We need to make these arrays near the same size as n, so pad
    // them using macros.
    #define ARY10 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    #define ARY50 ARY10, ARY10, ARY10, ARY10, ARY10
    #define ARY52 ARY50, 0, 0
    #define ARY55 ARY52, 0, 0, 0

    const Num     a[] = { ARY55,   3.5,    3.5  };
    const Num     b[] = { ARY55,   0.85,   0.95 };
    const Num     A[] = { ARY55,   0.32,   0.32 };
    const Num     B[] = { ARY55,   0.2,    0.2  };
    const Num     C[] = { ARY55,  28,     32    };
    const Num     D[] = { ARY55, 700,    800    };
    
    // Note these are slightly different sizes
    const Num alpha[] = { ARY52, 20, 20, 20 };
    const Num  beta[] = { ARY52, 150, 150, 250, 0.3, 0.3 };
    const Num gamma[] = { ARY52, 1.21, 1.21, 1.25 };

    enum Limits
    {
        eGamma1 = 4,
        eGamma2 = 9,
        ePhir1 = 1,
        ePhir2 = 8,
        ePhir3 = 52,
        ePhir4 = 55,
        ePhir5 = 57
    };
	
	Num phi0(const Num &,const Num &);
	Num phi0delta(const Num &,const Num &);
	Num phi0deltadelta(const Num &,const Num &);
	Num phi0tau(const Num &,const Num &);
	Num phi0tautau(const Num &,const Num &);
	Num phi0deltatau(const Num &,const Num &);
	Num phir(const Num &,const Num &);
	Num phirdelta(const Num &,const Num &);
	Num phirdeltadelta(const Num &,const Num &);
	Num phirtau(const Num &,const Num &);
	Num phirtautau(const Num &,const Num &);
	Num phirdeltatau(const Num &,const Num &);
	Num phi0(const Num &,const Num &);
	Num phi0(const Num &,const Num &);
	Num phi0(const Num &,const Num &);
}

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
