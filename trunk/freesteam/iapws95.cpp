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

#include "iapws95.h"
#include <cmath>

namespace 
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

    inline void get_delta_tau(const Num & T, const Num & rho,
                                Num & delta, Num & tau)
    {
        delta = rho/rhoc_;
        tau   = Tc_/T;
    }

    Num phi0(const Num & delta, const Num & tau)
    {
        Num sum = 0;
        for (int i = eGamma1; i < eGamma2; i++)
        {
            sum += n0[i]*log(1-exp(-tau*gamma0[i]));
        }
        sum += log(delta) + n0[1] + n0[2]*tau + n0[3]*log(tau);
        return sum;
    }

    Num phi0delta(const Num & delta, const Num & tau)
    {
        return 1.0/delta;
    }

    Num phi0deltadelta(const Num & delta, const Num & tau)
    {
        return -1.0/(delta*delta);
    }

    Num phi0tau(const Num & delta, const Num & tau)
    {
        Num sum = n0[2] + n0[3]/tau;
        for (int i = eGamma1; i < eGamma2; i++)
        {
            sum += n0[i]*gamma0[i]*(1/(1-exp(-tau*gamma0[i])) - 1);
        }
        return sum;
    }

    Num phi0tautau(const Num & delta, const Num & tau)
    {
        Num sum = -n0[3]/(tau*tau);
        for (int i = eGamma1; i < eGamma2; i++)
        {
            Num a = exp(-tau*gamma0[i]);
            Num b = 1-a;
            sum -= n0[i]*gamma0[i]*gamma0[i]*a/(b*b);
        }
        return sum;
    }

    Num phi0deltatau(const Num & delta, const Num & tau)
    {
        return 0.0;
    }

    Num phir(const Num & delta, const Num & tau)
    {
        Num sum = 0;
        Num d1 = delta - 1;
        Num t1 = tau - 1;
        for (int i = ePhir1; i < ePhir2; i++)
        {
            sum += n[i]*pow(delta, d[i])*pow(tau, t[i]);
        }
        for (int i= ePhir2; i < ePhir3; i++)
        {
            sum += n[i]*pow(delta, d[i])*pow(tau, t[i])*
                    exp(-pow(delta, c[i]));
        }
        for (int i= ePhir3; i < ePhir4; i++)
        {
            Num a1 = -alpha[i]*d1*d1 - beta[i]*
                (tau - gamma[i])*(tau - gamma[i]);
            sum += n[i]*pow(delta, d[i])*pow(tau, t[i])*exp(a1);
        }
        for (int i= ePhir4; i < ePhir5; i++)
        {
            Num psi = exp(-C[i]*d1*d1 - D[i]*t1*t1);
            Num theta = 1 - tau + A[i]*pow(d1*d1, 1/(2*beta[i]));
            Num Delta = theta*theta + B[i]*pow(d1*d1, a[i]);
            sum += n[i]*pow(Delta, b[i])*delta*psi;
        }
        return sum;
    }

    Num phirdelta(const Num & delta, const Num & tau)
    {
        Num sum = 0;
        Num d1 = delta - 1;
        Num t1 = tau - 1;
        for (int i = ePhir1; i < ePhir2; i++)
        {
            sum += n[i]*d[i]*pow(delta, (d[i] - 1))*pow(tau, t[i]);
        }
        for (int i= ePhir2; i < ePhir3; i++)
        {
            sum += n[i]*exp(-pow(delta, c[i]))*(pow(delta, (d[i]-1))*
                    pow(tau, t[i])*(d[i] - c[i]*pow(delta, c[i])));
        }
        for (int i= ePhir3; i < ePhir4; i++)
        {
            Num a1 = tau - gamma[i];
            Num b1 = -alpha[i]*d1*d1 - beta[i]*a1*a1;
            sum += n[i]*pow(delta, d[i])*pow(tau, t[i])*exp(b1)*
                    (d[i]/delta - 2*alpha[i]*d1);
        }
        for (int i= ePhir4; i < ePhir5; i++)
        {
            Num psi = exp(-C[i]*d1*d1 - D[i]*t1*t1);
            Num theta = 1 - tau + A[i]*pow((d1*d1), (1/(2*beta[i])));
            Num Delta = theta*theta + B[i]*pow((d1*d1), (a[i]));
            Num dpsiddelta = -2*C[i]*d1*psi;
            Num dDeltaddelta = d1*(A[i]*theta*2/beta[i]*
                pow((d1*d1), (1/(2*beta[i]) - 1)) + 2*B[i]*a[i]*
                pow((d1*d1), (a[i] - 1)));
            Num dDeltabiddelta = b[i]*pow(Delta, (b[i]-1))*dDeltaddelta;
            sum += n[i]*(pow(Delta, b[i])*(psi + delta*dpsiddelta) +
                    dDeltabiddelta*delta*psi);
        }
        return sum;
    }

    Num phirdeltadelta(const Num & delta, const Num & tau)
    {
        Num sum = 0;
        Num d1 = delta - 1;
        Num t1 = tau - 1;
        for (int i = ePhir1; i < ePhir2; i++)
        {
            sum += n[i]*d[i]*(d[i] - 1)*
                    pow(delta, (d[i]-2))*pow(tau, t[i]);
        }
        for (int i= ePhir2; i < ePhir3; i++)
        {
            sum += n[i]*exp(-pow(delta, c[i]))*(pow(delta, (d[i]-2))*
                    pow(tau, t[i])*((d[i] - c[i]*pow(delta, c[i]))*
                    (d[i]-1-c[i]*pow(delta, c[i])) -
                    c[i]*c[i]*pow(delta, c[i])));
        }
        for (int i= ePhir3; i < ePhir4; i++)
        {
            Num a1 = tau - gamma[i];
            Num b1 = -alpha[i]*d1*d1 - beta[i]*a1*a1;
            sum += n[i]*pow(tau, t[i])*exp(b1)*
                    (-2*alpha[i]*pow(delta, d[i]) +
                    4*alpha[i]*alpha[i]*pow(delta, d[i])*d1*d1 -
                    4*d[i]*alpha[i]*pow(delta, (d[i]-1))*d1 +
                    d[i]*(d[i]-1)*pow(delta, (d[i]-1)));
        }
        for (int i= ePhir4; i < ePhir5; i++)
        {
            Num psi = exp(-C[i]*d1*d1 - D[i]*t1*t1);
            Num theta = 1 - tau + A[i]*pow((d1*d1), (1/(2*beta[i])));
            Num Delta = theta*theta + B[i]*pow((d1*d1), (a[i]));
            Num dpsiddelta = -2*C[i]*d1*psi;
            Num dDeltaddelta = d1*(A[i]*theta*2/beta[i]*
                pow((d1*d1), (1/(2*beta[i]) - 1)) + 2*B[i]*a[i]*
                pow((d1*d1), (a[i] - 1)));
            Num dDeltabiddelta = b[i]*pow(Delta, (b[i]-1))*dDeltaddelta;
            Num d2psiddelta2 = (2*C[i]*d1*d1 - 1)*2*C[i]*psi;
            Num a1 = pow(d1*d1, 1/(2*beta[i])-1);
            Num d2Deltaddelta2 = 1/d1*dDeltaddelta + d1*d1*(4*B[i]*a[i]* 
                (a[i]-1)*pow(d1*d1, a[i]-2) + 2*A[i]*A[i]*
                (1/(beta[i]*beta[i]))*a1*a1) +
                A[i]*theta*4/beta[i]*(1/(2*beta[i]) - 1)*
                pow(d1*d1, 1/(2*beta[i]) - 2);
            Num d2Deltabiddelta2 = b[i]*(pow(Delta, (b[i]-1)) *
                d2Deltaddelta2 + (b[i]-1)*pow(Delta, (b[i]-2))*
                dDeltaddelta*dDeltaddelta);
            sum += n[i]*(pow(Delta, b[i])*(2*dpsiddelta +
                    delta*d2psiddelta2) + 2*dDeltabiddelta*(psi +
                    delta*dpsiddelta) + d2Deltabiddelta2*delta*psi);
        }
        return sum;
    }

    Num phirtau(const Num & delta, const Num & tau)
    {
        Num sum = 0;
        Num d1 = delta - 1;
        Num t1 = tau - 1;
        for (int i = ePhir1; i < ePhir2; i++)
        {
            sum += n[i]*t[i]*pow(delta, d[i])*pow(tau, (t[i]-1));
        }
        for (int i= ePhir2; i < ePhir3; i++)
        {
            sum += n[i]*t[i]*pow(delta, d[i])*
                pow(tau, (t[i]-1))*exp(-pow(delta, c[i]));
        }
        for (int i= ePhir3; i < ePhir4; i++)
        {
            Num a1 = tau - gamma[i];
            Num b1 = -alpha[i]*d1*d1 - beta[i]*a1*a1;
            sum += n[i]*pow(delta, d[i])*pow(tau, t[i])*exp(b1)*
                    (t[i]/tau - 2*beta[i]*(tau - gamma[i]));
        }
        for (int i= ePhir4; i < ePhir5; i++)
        {
            Num psi = exp(-C[i]*d1*d1 - D[i]*t1*t1);
            Num theta = 1 - tau + A[i]*pow(d1*d1, (1/(2*beta[i])));
            Num Delta = theta*theta + B[i]*pow(d1*d1, (a[i]));
            Num dDeltabidtau = -2*theta*b[i]*pow(Delta, (b[i]-1));
            Num dpsidtau = -2*D[i]*t1*psi;
            sum += n[i]*delta*(dDeltabidtau*psi +
                    pow(Delta, b[i])*dpsidtau);
        }
        return sum;
    }


    Num phirtautau(const Num & delta, const Num & tau)
    {
        Num sum = 0;
        Num d1 = delta - 1;
        Num t1 = tau - 1;
        for (int i = ePhir1; i < ePhir2; i++)
        {
            sum += n[i]*t[i]*(t[i]-1)*pow(delta, d[i])*
                    pow(tau, (t[i]-2));
        }
        for (int i= ePhir2; i < ePhir3; i++)
        {
            sum += n[i]*t[i]*(t[i]-1)*pow(delta, d[i])*
                    pow(tau, (t[i]-2))*exp(-pow(delta, c[i]));
        }
        for (int i= ePhir3; i < ePhir4; i++)
        {
            Num a1 = tau - gamma[i];
            Num b1 = -alpha[i]*d1*d1 - beta[i]*a1*a1;
            sum += n[i]*pow(delta, d[i])*pow(tau, t[i])*exp(b1)*
                    (pow((t[i]/tau - 2*beta[i]*a1), 2) -
                    t[i]/(tau*tau) - 2*beta[i]);
        }
        for (int i= ePhir4; i < ePhir5; i++)
        {
            Num psi = exp(-C[i]*d1*d1 - D[i]*t1*t1);
            Num theta = 1 - tau + A[i]*pow(d1*d1, (1/(2*beta[i])));
            Num Delta = theta*theta + B[i]*pow(d1*d1, (a[i]));
            Num dDeltabidtau = -2*theta*b[i]*pow(Delta, (b[i]-1));
            Num d2Deltabidtau2 = 2*b[i]*pow(Delta, (b[i]-1)) +
                4*theta*theta*b[i]*(b[i]-1)*pow(Delta, (b[i]-2));
            Num dpsidtau = -2*D[i]*t1*psi;
            Num d2psidtau2 = (2*D[i]*t1*t1 - 1)*2*D[i]*psi;
            sum += n[i]*delta*(d2Deltabidtau2*psi +
                    2*dDeltabidtau*dpsidtau + pow(Delta, b[i])*d2psidtau2);
        }
        return sum;
    }

    Num phirdeltatau(const Num & delta, const Num & tau)
    {
        Num sum = 0;
        Num d1 = delta - 1;
        Num t1 = tau - 1;
        for (int i = ePhir1; i < ePhir2; i++)
        {
            sum += n[i]*d[i]*t[i]*pow(delta, (d[i]-1))*
                    pow(tau, (t[i]-1));
        }
        for (int i= ePhir2; i < ePhir3; i++)
        {
            sum += n[i]*t[i]*pow(delta, (d[i]-1))*
                    pow(tau, (t[i]-1))*(d[i] - c[i]*
                    pow(delta, c[i]))*exp(-pow(delta, c[i]));
        }
        for (int i= ePhir3; i < ePhir4; i++)
        {
            Num a1 = tau - gamma[i];
            Num b1 = -alpha[i]*d1*d1 - beta[i]*a1*a1;
            sum += n[i]*pow(delta, d[i])*pow(tau, t[i])*exp(b1)*
                    (d[i]/delta - 2*alpha[i]*d1)*
                    (t[i]/tau - 2*beta[i]*a1);
        }
        for (int i= ePhir4; i < ePhir5; i++)
        {
            Num psi = exp(-C[i]*d1*d1 - D[i]*t1*t1);
            Num theta = 1 - tau + A[i]*pow(d1*d1, (1/(2*beta[i])));
            Num Delta = theta*theta + B[i]*pow(d1*d1, (a[i]));
            Num dDeltabidtau = -2*theta*b[i]*pow(Delta, (b[i]-1));
            Num dpsidtau = -2*D[i]*t1*psi;
            Num dDeltaddelta = d1*(A[i]*theta*2/beta[i]*
                pow(d1*d1, (1/(2*beta[i]) - 1)) + 2*B[i]*a[i]*
                pow(d1*d1, (a[i] - 1)));
            Num d2psiddeltadtau = 4*C[i]*D[i]*d1*t1*psi;
            Num dDeltabiddelta = b[i]*pow(Delta, (b[i]-1))*dDeltaddelta;
            Num dpsiddelta = -2*C[i]*d1*psi;
            Num d2Deltabiddeltadtau = -A[i]*b[i]*2/beta[i]*
                pow(Delta, (b[i]-1))*d1*pow(d1*d1, (1/(2*beta[i])-1)) -
                2*theta*b[i]*(b[i]-1)*pow(Delta, (b[i]-2))*
                dDeltaddelta;
            sum += n[i]*(pow(Delta, b[i])*(dpsidtau + delta*
                    d2psiddeltadtau) + delta*dDeltabiddelta*dpsidtau +
                    dDeltabidtau*(psi + delta*dpsiddelta) +
                    d2Deltabiddeltadtau*delta*psi);
        }
        return sum;
    }

} // End of anonymous namespace


Water95::Water95()
    :
    R(R_),
    Tc(Tc_),
    rhoc(rhoc_)
{
    // Intentionally empty
}


Num Water95::p(const Num & T, const Num & rho)
{
    Num delta;
    Num tau;
    get_delta_tau(T, rho, delta, tau);
    return rho*R*T*(1 + delta*phirdelta(delta, tau));
}

Num Water95::u(const Num & T, const Num & rho)
{
    Num delta;
    Num tau;
    get_delta_tau(T, rho, delta, tau);
    return R*T*tau*(phi0tau(delta, tau) + 
            phirtau(delta, tau));
}

Num Water95::h(const Num & T, const Num & rho)
{
    Num delta;
    Num tau;
    get_delta_tau(T, rho, delta, tau);
    return R*T*(1 + tau*(phi0tau(delta, tau) + 
            phirtau(delta, tau)) + delta*phirdelta(delta, tau));
}

Num Water95::s(const Num & T, const Num & rho)
{
    Num delta;
    Num tau;
    get_delta_tau(T, rho, delta, tau);
    return R*(tau*(phi0tau(delta, tau) + 
            phirtau(delta, tau)) - 
            phi0(delta, tau) - phir(delta, tau));
}

Num Water95::cv(const Num & T, const Num & rho)
{
    Num delta;
    Num tau;
    get_delta_tau(T, rho, delta, tau);
    return -R*tau*tau*(phi0tautau(delta, tau) + 
            phirtautau(delta, tau));
}

Num Water95::cp(const Num & T, const Num & rho)
{
    Num delta;
    Num tau;
    get_delta_tau(T, rho, delta, tau);
    Num a1 = 1 + delta*phirdelta(delta, tau) - 
        delta*tau*phirdeltatau(delta, tau);
    Num b1 = 1 + 2*delta*phirdelta(delta, tau) + 
        delta*delta*phirdeltadelta(delta, tau);
    return R*(-tau*tau*(phi0tautau(delta, tau) + 
            phirtautau(delta, tau)) + a1*a1/b1);
}

Num Water95::w(const Num & T, const Num & rho)
{
    Num delta;
    Num tau;
    get_delta_tau(T, rho, delta, tau);
    Num a1 = 1 + delta*phirdelta(delta, tau) - 
        delta*tau*phirdeltatau(delta, tau);
    Num b1 = tau*tau*(phi0tautau(delta, tau) + 
        phirtautau(delta, tau));
    Num w = 1 + 2*delta*phirdelta(delta, tau) + 
        delta*delta*phirdeltadelta(delta, tau) - a1*a1/b1;
    return sqrt(R*T*w*1000);
}

// ----------------------------------------------------------------------

#ifdef TESTMAIN

// Unit tests for Water95 class.  Returns 0 if successful.

#include <iostream>
using namespace std;
#include <stdio.h>
#include <cassert>


Num PctDiff(const Num & a, const Num & b)
{
    if (a == 0 and b == 0)
        return 0.0;
    return fabs(100.0*(a-b)/b);
}

void TestIdealGasPart()
{
    // Test that we match Table 6 values.  Note the phi0tautau
    // expected value is negative, as it has to be, although the
    // table gives it as positive.
    Num eps = 0.0001;
    Num T = 500;       // K
    Num rho = 838.025; // kg/m^3
    Water95 w;
    assert(PctDiff(phi0(          rho/w.rhoc, w.Tc/T),  0.204797734e+1) < eps);
    assert(PctDiff(phi0delta(     rho/w.rhoc, w.Tc/T),  0.384236747e+0) < eps);
    assert(PctDiff(phi0deltadelta(rho/w.rhoc, w.Tc/T), -0.147637878e+0) < eps);
    assert(PctDiff(phi0tau(       rho/w.rhoc, w.Tc/T),  0.904611106e+1) < eps);
    assert(PctDiff(phi0tautau(    rho/w.rhoc, w.Tc/T), -0.193249185e+1) < eps);
    assert(PctDiff(phi0deltatau(  rho/w.rhoc, w.Tc/T),  0.000000000e+0) < eps);
}

void TestRealGasPart()
{
    // Test that we match Table 6 values
    Num eps = 0.0001;
    Num T = 500;       // K
    Num rho = 838.025; // kg/m^3
    Water95 w;
    assert(PctDiff(phir(          rho/w.rhoc, w.Tc/T), -0.342693206e+1) < eps);
    assert(PctDiff(phirdelta(     rho/w.rhoc, w.Tc/T), -0.364366650e+0) < eps);
    assert(PctDiff(phirdeltadelta(rho/w.rhoc, w.Tc/T),  0.856063701e+0) < eps);
    assert(PctDiff(phirtau(       rho/w.rhoc, w.Tc/T), -0.581403435e+1) < eps);
    assert(PctDiff(phirtautau(    rho/w.rhoc, w.Tc/T), -0.223440737e+1) < eps);
    assert(PctDiff(phirdeltatau(  rho/w.rhoc, w.Tc/T), -0.112176915e+1) < eps);
}

void TestSinglePhaseRegion()
{
    // Values from Table 7
    Num values[11][6] = {
        //Temp   Density       Pressure     Isochoric Sp. Ht  Spd of Sound       Entropy
        // K     kg/m^3           MPa          kJ/(kg*K)          m/s           kJ/(kg*K)
        // T       rho             p              cv               w                s
        {300.0, 0.9965560e+3, 0.99242e-1    , 0.413018111e+1, 0.150151914e+4, 0.393062642e+0},
        {300.0, 0.1005308e+4, 0.200022514e+2, 0.406798347e+1, 0.153492501e+4, 0.387405401e+0},
        {300.0, 0.1188202e+4, 0.700004704e+3, 0.346135580e+1, 0.244357992e+4, 0.132609616e+0},
        {500.0, 0.4350000e+0, 0.999679423e-1, 0.150817541e+1, 0.548314253e+3, 0.794488271e+1},
        {500.0, 0.4532000e+1, 0.999938125e+0, 0.166991025e+1, 0.535739001e+3, 0.682502725e+1},
        {500.0, 0.8380250e+3, 0.100003858e+2, 0.322106219e+1, 0.127128441e+4, 0.256690918e+1},
        {500.0, 0.1084564e+4, 0.700000405e+3, 0.307437693e+1, 0.241200877e+4, 0.203237509e+1},
        {647.0, 0.3580000e+3, 0.220384756e+2, 0.618315728e+1, 0.252145078e+3, 0.432092307e+1},
        {900.0, 0.2410000e+0, 0.100062559e+0, 0.175890657e+1, 0.724027147e+3, 0.916653194e+1},
        {900.0, 0.5261500e+2, 0.200000690e+2, 0.193510526e+1, 0.698445674e+3, 0.659070225e+1},
        {900.0, 0.8707690e+3, 0.700000006e+3, 0.266422350e+1, 0.201933608e+4, 0.417223802e+1}
    };
    for (int i = 0; i < 11; i++)
    {
        Water95 w;
        Num T, rho, p_e, cv_e, w_e, s_e, eps;
        T    = values[i][0];
        rho  = values[i][1];
        p_e  = values[i][2];
        cv_e = values[i][3];
        w_e  = values[i][4];
        s_e  = values[i][5];
        if (T == 647.0)
            eps = 0.2; // Precision at the critical point is much less
        else
            eps = 0.0002;
        assert(PctDiff(w.p(T, rho)/1000, p_e) < eps);
        assert(PctDiff(w.cv(T, rho), cv_e) < eps);
        assert(PctDiff(w.w(T, rho), w_e) < eps);
        assert(PctDiff(w.s(T, rho), s_e) < eps);
    }
    // Tests for u and h came from NIST webpage at
    // http://webbook.nist.gov/chemistry/fluid/
    Num values1[6][5] = {
        // T    rho         u       h        cp
        {350, 973.70,     321.75, 321.79,  4.1946},  // Liquid
        {400, 937.49,     532.69, 532.95,  4.2555},  // Liquid
        {440, 900.65,     704.50, 705.31,  4.3571},  // Liquid
        {350,   0.26029, 2477.6, 2637.7,   2.0033},  // Vapor
        {400,   1.3694,  2536.2, 2715.7,   2.2183},  // Vapor
        {500,   4.5320,  2670.6, 2891.2,   2.2795}   // Vapor
    };
    for (int i = 0; i < 6; i++)
    {
        Water95 w;
        Num T, rho, u_e, h_e, cp_e;
        Num eps = 0.0025;
        T    = values1[i][0];
        rho  = values1[i][1];
        u_e  = values1[i][2];
        h_e  = values1[i][3];
        cp_e = values1[i][4];
        assert(PctDiff(w.u(T, rho), u_e) < eps);
        assert(PctDiff(w.h(T, rho), h_e) < eps);
        assert(PctDiff(w.cp(T, rho), cp_e) < eps);
    }
}

int main(void)
{
    TestIdealGasPart();
    TestRealGasPart();
    TestSinglePhaseRegion();
    cout << "Tests passed" << endl;
    return 0;
}

#endif
