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
#define FREESTEAM_BUILDING_LIB
#include "solver2.h"

#include "region1.h"
#include "region2.h"
#include "region3.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

SteamState solver2_region3(char X, char Y, double x, double y, SteamState guess, int *status){

	int maxiter = 20;

	SteamState S = guess;
	if(guess.region!=3){
		*status = 1;
		fprintf(stderr,"%s: first guess is not region 3!\n",__func__);
		return S;
	}

	SteamState petT, petrho;
	double T,dT;
	double rho,drho;
	double f,Df, DfT, Dfrho; /* first prop */
	double s,Ds,DsT, Dsrho; /* second prop and derivs */
	double p;

	// cerr << endl << "Solver2::solveRegion3: Starting iterations";

	int niter=0;
	while(1){
		T = freesteam_T(S);
		rho = 1./freesteam_v(S);
		p = freesteam_p(S);

		f = freesteam_prop(X,S);
		s = freesteam_prop(Y,S);

		Df = x - f;
		Ds = y - s;
		
#if 0
	/* FIXME need to implement convergence test somehow */

		// In this template it's hard to know the units of the convergence test, so make it as a new, separate, template:
		if(
			ConvergenceTest<FirstProp,FirstPropAlt>::test(Df,p,T)
			&& ConvergenceTest<SecondProp,SecondPropAlt>::test(Ds,p,T)
		){
			// cerr << endl << "     ... SOLUTION OK (" << niter << " iterations)" << endl;
			return guess;
		}
#endif

		dT = T * 0.001;

		if(T + dT > REGION2_TMAX){
			dT = -dT;
		}

		petT = freesteam_region3_set_rhoT(rho,T+dT);
		if(petT.region!=3){
			fprintf(stderr,"%s (%s:%d): region is not 3 as expected\n",__func__,__FILE__,__LINE__);
			exit(1);
		}

		// ensure we don't go over rho limits
		drho = rho * 0.001;
		if(rho + drho > 50.0){
			drho = -drho;
		}

		petrho = freesteam_region3_set_rhoT(rho+drho, T);
		if(petrho.region!=3){
			fprintf(stderr,"%s (%s:%d): region is not 3 as expected\n",__func__,__FILE__,__LINE__);
			exit(1);
		}

		
		DfT = freesteam_prop(X,petT) - f;
		DsT = freesteam_prop(Y,petT) - s;

		Dfrho = freesteam_prop(X,petrho) - f;
		Dsrho = freesteam_prop(Y,petrho) - s;

		ASSERT(DfT * Dsrho != DsT * Dfrho);

		// Solve new peturbations
		dT = dT * (Df*Dsrho - Ds*Dfrho) / (DfT*Dsrho - DsT*Dfrho);
		drho = drho * (DfT*Ds - DsT*Df) / (DfT*Dsrho - DsT*Dfrho);

		ASSERT(!isnan(dT));
		ASSERT(!isnan(drho));

		// Regulate maximum change in temperature

		double dTMax = 0.1 * T;
		double dTAbs = fabs(dT);
		if(dTAbs > dTMax){
			//cerr << endl << "      ... limiting dT, too great";
			dT = dT * dTMax/dTAbs;
		}

		// Regulate max change in pressure

		double drhoMax = 0.4 * rho;
		double drhoAbs = fabs(drho);
		if(drhoAbs > drhoMax){
			//cerr << endl << "      ... limiting drho, too great";
			drho = drho * drhoMax/drhoAbs;
		}

		//cerr << endl << "     ... calculated dT = " << dT << ", drho = " << drho;
		T = T + dT;
		rho = rho + drho;

		if(T > REGION2_TMAX){
			// Strangely, this test never seems to be used...
			//cerr << endl << "     .... Applying T_MAX limit";
			T = REGION2_TMAX;
		}

		if(T < REGION1_TMAX){
			//cerr << endl << "     .... Applying T_REG1_REG3 limit";
			T = REGION1_TMAX;
		}

		/* FIXME: do we really need this bit? */
#if 0 
		double rho_b134;
		SteamState S;
		S.setSatWater_T(T_REG1_REG3);
		rho_b134 = S.dens();
		if(rho < rho_b134 && T < T_CRIT){
			if(rho < RHO_CRIT){
				double rho_g = Boundaries::getSatDensSteam_T(T);
				if(rho > rho_g){
					//cerr << endl << "     ... Applying rho_g limit";
					rho = rho_g;
				}
			}else if(rho > RHO_CRIT){
				double rho_f = Boundaries::getSatDensWater_T(T);
				if(rho <  rho_f){
					//cerr << endl << "     ... Applying rho_f limit";
					rho = rho_f;
				}
			}

		}

		B23Curve<Density,Pressure> B23;
		double rho_b23 = B23.solve(T);
		if(rho < rho_b23){
			rho = rho_b23;
		}

		S.setRegion3_rhoT(rho,T);
		int pmaxiter = 0;
		if(S.pres() > P_MAX){
			do{
				//cerr << endl << "    ... found p = " << S.pres() / MPa << " MPa";

				drho *= 0.5001;
				//dT *= 0.5001;
				rho -= drho;
				//T -= dT;
				S.setRegion3_rhoT(rho,T);
				//cerr << endl << "     ... Applying P_MAX limit at T = " << T << ": rho = " << rho;
				if(++pmaxiter > 20){
					throw std::runtime_error("Solver2::solveRegion3: Failed to find rho of P_MAX");
				}

			}while(S.pres() > P_MAX);
			//cerr << endl << "     ... pressure capped to " << S.pres();
		}
#endif

		S = freesteam_region3_set_rhoT(rho,T);

		niter++;

		if(niter > maxiter){
			*status = 2;
			fprintf(stderr,"%s (%s:%d): exceeded maximum iterations\n",__func__,__FILE__,__LINE__);
			return S;
		}
	}
}

#if 0
int solver2_region4(char X, char Y, double x, double y, SteamState *S){
	try{
		// Just like region 1, except for it's T,x

		//cerr << endl << "Solver2::solveRegion4: ";

		SteamState guess = firstguess;

		//cerr << endl << "Solver2::solveRegion4: firstguess copied to guess OK";

		if(firstguess.whichRegion()!=4){
			throw std::runtime_error("Solver2::solveRegion4: First guess is not region 4");
		}

		SteamState petT, petx;
		double T,dT;
		Num x,dx;
		double f,Df, DfT, Dfx;
		double s,Ds,DsT, Dsx;
		double p;

		//cerr << endl << "Solver2::solveRegion4: Starting iterations";

		int niter=0;
		while(1){

			T = guess.temp();
			x = guess.quality();
			p = guess.pres();

			//cerr << endl << "Iter " << niter << ": x = " << x << ", T = " << T;

			f = SteamProperty<FirstProp,FirstPropAlt>::get(guess);
			s = SteamProperty<SecondProp,SecondPropAlt>::get(guess);

			//cerr << ": " << SteamProperty<FirstProp,FirstPropAlt>::name() << " = " << f;
			//cerr << ", " << SteamProperty<SecondProp,SecondPropAlt>::name() << " = " << s;

			Df = f1 - f;
			Ds = s1 - s;

			// In this template it's hard to know the units of the convergence test, so make it as a new, separate, template:
			if(
				ConvergenceTest<FirstProp,FirstPropAlt>::test(Df,p,T)
				&& ConvergenceTest<SecondProp,SecondPropAlt>::test(Ds,p,T)
			){
				//cerr << endl << "     ... SOLUTION OK" << endl;
				return guess;
			}

			dT = -T * 0.001;
			// ensure we don't go over the T limits with our peturbation
			if(T + dT < T_TRIPLE){
				dT = -dT;
			}

			Boundaries::isSat_Tx(T+dT,x, true);

			petT.setRegion4_Tx(T + dT, x);
			ASSERT(petT.whichRegion()==4);

			// ensure we don't go over x limits
			dx = 0.0001;
			if(x + dx > 1.0){
				dx = -dx;
			}

			Boundaries::isSat_Tx(T, x+dx, true);

			petx.setRegion4_Tx(T, x + dx);
			ASSERT(petx.whichRegion()==4);

			DfT = SteamProperty<FirstProp,FirstPropAlt>::get(petT) - f;
			DsT = SteamProperty<SecondProp,SecondPropAlt>::get(petT) - s;

			Dfx = SteamProperty<FirstProp,FirstPropAlt>::get(petx) - f;
			Dsx = SteamProperty<SecondProp,SecondPropAlt>::get(petx) - s;

			//ASSERT(!(DfT == 0.0 * FirstProp() && Dfx == 0.0 * FirstProp()));
			//ASSERT(!(DsT == 0.0 * SecondProp() && Dsx == 0.0 * SecondProp()));
			//ASSERT(!(DsT == 0.0 * SecondProp() && DfT == 0.0 * FirstProp()));
			//ASSERT(!(Dsx == 0.0 * SecondProp() && Dfx == 0.0 * FirstProp()));
			ASSERT(DfT * Dsx != DsT * Dfx);

			//ASSERT(0.0 * FirstProp() * SecondProp() != DfT * Dsx - DsT *Dfx);

			//ASSERT(!((DfT==0.0 * FirstProp() || Dsx==0.0 * SecondProp()) && (Dfx==0.0 * FirstProp() || DsT==0.0 * SecondProp())));


			// Solve new peturbations
			dT = dT * (Df*Dsx - Ds*Dfx) / (DfT*Dsx - DsT*Dfx);
			dx = dx * (DfT*Ds - DsT*Df) / (DfT*Dsx - DsT*Dfx);

			ASSERT(!isnan(dT));
			ASSERT(!isnan(dx));

			// Regulate maximum change in temperature

			double dTMax = 0.1 * T;
			double dTAbs = fabs(dT);
			if(dTAbs > dTMax){
				//cerr << endl << "      ... limiting dT, too great";
				dT = dT * dTMax/dTAbs;
			}

			// Regulate max change in pressure

			Num dxMax = 0.2;
			Num dxAbs = fabs(dx);
			if(dxAbs > dxMax){
				//cerr << endl << "      ... limiting dx, too great";
				dx = dx * dxMax/dxAbs;
			}

			//cerr << endl << "     ... calculated dT = " << dT << ", dx = " << dx;
			T = T + dT;
			x = x + dx;

			// Keep new temperature in limits
			if(T > T_CRIT){
				//cerr << endl << "     ... applying T_CRIT limit";
				T = T_CRIT;
			}else if(T < T_TRIPLE){
				//cerr << endl << "     ... applying T_TRIPLE limit";
				T = T_TRIPLE;
			}

			// Keep pressure in limits
			if(x > 1){
				//cerr << endl << "     ... applying upper x limit";
				x = 1;
			}else if(x < 0){
				//cerr << endl << "     ... applying lower x limit";
				x = 0;
			}

			Boundaries::isSat_Tx(T, x, true);
			guess.setRegion4_Tx(T,x);

			niter++;

			if(niter > maxiter){
				throw std::runtime_error("Solver2::solveRegion4: Exceeded maximum iterations");
			}
		}
	}catch(std::exception &E){
		std::stringstream s;
		s<< "Solver2::solveRegion4: " << E.what();
		throw std::runtime_error(s.str());
	}
}

int solver2_region1(char X, char Y, double x, double y, SteamState *S);

	SteamState guess = firstguess;

	if(firstguess.whichRegion()!=1){
		throw std::runtime_error("Solver2::solveRegion1: First guess is not region 1");
	}

	SteamState petT, petp;
	double T,dT;
	double p,dp;
	double f,Df, DfT, Dfp;
	double s,Ds,DsT, Dsp;

	int niter=0;
	// If we are in region 1, then we will be iterating with pressure and temperature

	while(1){

		T = guess.temp();
		p = guess.pres();

		//cerr << endl << "Iter " << niter << ": p = " << p << ", T = " << T;

		f = SteamProperty<FirstProp,FirstPropAlt>::get(guess);
		s = SteamProperty<SecondProp,SecondPropAlt>::get(guess);

		//cerr << ": " << SteamProperty<FirstProp,FirstPropAlt>::name() << " = " << f;
		//cerr << ", " << SteamProperty<SecondProp,SecondPropAlt>::name() << " = " << s;

		Df = f1 - f;
		Ds = s1 - s;

		// In this template it's hard to know the units of the convergence test, so make it as a new, separate, template:
		if(
			ConvergenceTest<FirstProp,FirstPropAlt>::test(Df,p,T)
			&& ConvergenceTest<SecondProp,SecondPropAlt>::test(Ds,p,T)
		){
			//cerr << endl << "     ... SOLUTION OK" << endl;
			return guess;
		}

		dT = -T * 0.001;
		// ensure we don't go over the T limits with our peturbation
		if(T + dT < T_TRIPLE){
			dT = -dT;
		}

		petT.set_pT(p,T + dT);
		ASSERT(petT.whichRegion()==1);

		// ensure we don't go over p limits
		dp = p * 0.001;
		if(p > 99.0 * MPa){
			dp = -dp;
		}

		petp.set_pT(p + dp, T);
		ASSERT(petp.whichRegion()==1);

		DfT = SteamProperty<FirstProp,FirstPropAlt>::get(petT) - f;
		DsT = SteamProperty<SecondProp,SecondPropAlt>::get(petT) - s;

		Dfp = SteamProperty<FirstProp,FirstPropAlt>::get(petp) - f;
		Dsp = SteamProperty<SecondProp,SecondPropAlt>::get(petp) - s;

		// Solve new peturbations
		dT = dT * (Df*Dsp - Ds*Dfp) / (DfT*Dsp - DsT*Dfp);
		dp = dp * (DfT*Ds - DsT*Df) / (DfT*Dsp - DsT*Dfp);

		// Regulate maximum change in temperature

		double dTMax = 0.1 * T;
		double dTAbs = fabs(dT);
		if(dTAbs > dTMax){
			//cerr << endl << "      ... limiting dT, too great";
			dT = dT * dTMax/dTAbs;
		}

		// Regulate max change in pressure

		double dpMax = 0.4 * p;
		double dpAbs = fabs(dp);
		if(dpAbs > dpMax){
			//cerr << endl << "      ... limiting dp, too great";
			dp = dp * dpMax/dpAbs;
		}

		//cerr << endl << "     ... calculated dT = " << dT << ", dp = " << dp;
		T = T + dT;
		p = p + dp;

		// Keep new temperature in limits
		if(T > T_REG1_REG3){
			//cerr << endl << "     ... applying upper T limit";
			T = T_REG1_REG3;
		}else if(T < T_TRIPLE){
			//cerr << endl << "     ... applying lower T limit";
			T = T_TRIPLE;
		}

		// Keep pressure in limits
		if(p > P_MAX){
			//cerr << endl << "     ... applying upper p limit";
			p = P_MAX;
		}else if(p < P_TRIPLE){
			//cerr << endl << "     ... applying lower p_triple limit";
			p = P_TRIPLE;
		}else if(p < PB_LOW){
			double p_sat = Boundaries::getSatPres_T(T);
			if(p < p_sat){
				//cerr << endl << "     ... applying saturation limit to p";
				p = p_sat;
			}
		}

		ASSERT(guess.whichRegion()==1);
		ASSERT(Boundaries::isRegion1_pT(p,T));

		guess.setRegion1_pT(p,T);

		niter++;

		if(niter > maxiter){
			throw std::runtime_error("Solver2::solveRegion1: Exceeded maximum iterations");
		}
			}
}

int solver2_region2(char X, char Y, double x, double y, SteamState *S);

	// Most of this is the same as for solveRegion1:

	#ifdef SOLVER2_DEBUG
		if(SOLVER2BASE_DEBUG){
			std::cerr << std::endl << "---------------------------------" << std::endl << "SOLVE REGION 2";
		}
	#endif

	SteamState guess = firstguess;

	if(firstguess.whichRegion()!=2){
		throw std::runtime_error("Solver2::solveRegion2: First guess is not region 2");
	}

	SteamState petT, petp;
	double T,dT;
	double p,dp;
	double f,Df, DfT, Dfp;
	double s,Ds,DsT, Dsp;

	int niter=0;
	// If we are in region 1, then we will be iterating with pressure and temperature

	while(1){

		T = guess.temp();
		p = guess.pres();

		#ifdef SOLVER2_DEBUG
			if(SOLVER2BASE_DEBUG){
				std::cerr << std::endl << "Iter " << niter << ": p = " << p / MPa << " MPa, T = " << T << " (" << tocelsius(T) << "°C)";
			}
		#endif

		f = SteamProperty<FirstProp,FirstPropAlt>::get(guess);
		s = SteamProperty<SecondProp,SecondPropAlt>::get(guess);

		#ifdef SOLVER2_DEBUG
			if(SOLVER2BASE_DEBUG){
				std::cerr << ": " << SteamProperty<FirstProp,FirstPropAlt>::name() << " = " << f;
				std::cerr << ", " << SteamProperty<SecondProp,SecondPropAlt>::name() << " = " << s;
			}
		#endif

		Df = f1 - f;
		Ds = s1 - s;

		if(
			ConvergenceTest<FirstProp,FirstPropAlt>::test(Df,p,T)
			&& ConvergenceTest<SecondProp,SecondPropAlt>::test(Ds,p,T)
		){
			//cerr << endl << "     ... SOLUTION OK" << endl;
			return guess;
		}

		// Peturb T but keep inside T_MAX
		dT = T * 0.001;
		if(T + dT > T_MAX){
			dT = -dT;
		}

		petT.set_pT(p,T+ dT);
		ASSERT(petT.whichRegion()==2);

		// Peturb p but keep above P_TRIPLE
		dp = -p * 0.001;
		if(p + dp < P_TRIPLE){
			dp = -dp;
		}
		petp.set_pT(p + dp, T);
		ASSERT(petp.whichRegion()==2);

		DfT = SteamProperty<FirstProp,FirstPropAlt>::get(petT) - f;
		DsT = SteamProperty<SecondProp,SecondPropAlt>::get(petT) - s;

		Dfp = SteamProperty<FirstProp,FirstPropAlt>::get(petp) - f;
		Dsp = SteamProperty<SecondProp,SecondPropAlt>::get(petp) - s;

		// Solve new peturbations
		dT = dT * (Df*Dsp - Ds*Dfp) / (DfT*Dsp - DsT*Dfp);
		dp = dp * (DfT*Ds - DsT*Df) / (DfT*Dsp - DsT*Dfp);

		// Regulate maximum change in temperature

		double dTMax = 0.2 * T;
		double dTAbs = fabs(dT);
		if(dTAbs > dTMax){
			//cerr << endl << "      ... limiting dT, too great";
			dT = dT * dTMax/dTAbs;
		}

		// Regulate max change in pressure

		double dpMax = 0.5 * p;
		double dpAbs = fabs(dp);
		if(dpAbs > dpMax){
			//cerr << endl << "      ... limiting dp, too great";
			dp = dp * dpMax/dpAbs;
		}

		//cerr << endl << "     ... calculated dT = " << dT << ", dp = " << dp;
		T = T + dT;
		p = p + dp;

		// Keep new temperature in limits
		if(T > T_MAX){
			//cerr << endl << "     ... applying T_MAXlimit";
			T = T_MAX;
		}else if(T < T_TRIPLE){
			//cerr << endl << "     ... applying T_TRIPLE limit";
			T = T_TRIPLE;
		}

		// Keep pressure in limits
		if(p < P_TRIPLE){
			//cerr << endl << "     ... applying P_TRIPLE limit";
			p = P_TRIPLE;
		}else if(T > TB_LOW){
			double pbound = Boundaries::getpbound_T(T);
			if(p > pbound){
				//cerr << endl << "     ... applying pbound limit";
				p = pbound;
			}
		}else{
			double psat = Boundaries::getSatPres_T(T);
			if(p > psat){
				//cerr << endl << "     ... applying psat limit";
				p = psat;
			}
		}

		ASSERT(guess.whichRegion()==2);
		ASSERT(Boundaries::isRegion2_pT(p,T));

		guess.setRegion2_pT(p,T);

		niter++;

		if(niter > maxiter){
			throw std::runtime_error("Solver2::solveRegion2: Exceeded maximum iterations");
		}
	}

}
#endif

