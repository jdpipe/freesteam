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

namespace IAPWS95
{

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

using namespace IAPWS95;

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
    return rho*R*T*(1 + delta*phirdelta(delta, tau))/1000;
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
