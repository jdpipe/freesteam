#ifndef SOLVER2_H
#define SOLVER2_H

#include "steamproperty.h"
#include "units.h"
#include "satcurve.h"
#include "exception.h"
#include "convergencetest.h"

/// Base class for all two-property solvers
/**
	This class is abstract. Use it when creating template Solver2 classes for particular combinations of variables, so that all Solver2 classes will have common methods whichRegion, solve, getFirstProp, etc, defined.

	@param FirstProp (Units of) first property to be solved for
	@param SecondProp (Units of) second property to be solved for
	@param FirstPropAlt First property alternative, used to distinguish two properties having the same units
	@param SecondPropAlt Second property alternative, used to distinguish two properties having the same units
*/
template<class FirstProp,class SecondProp,int FirstPropAlt=0, int SecondPropAlt=0>
class Solver2Base{

	protected:

		Solver2Base(){}
		virtual ~Solver2Base(){}

		virtual int whichRegion(const FirstProp &fp,const SecondProp &sp) = 0;

		virtual SteamCalculator solve(const FirstProp &fp, const SecondProp &sp) = 0;

		FirstProp getFirstProp(SteamCalculator &S){
			return SteamProperty<FirstProp,FirstPropAlt>::get(S);
		}

		SecondProp getSecondProp(SteamCalculator &S){
			return SteamProperty<SecondProp,SecondPropAlt>::get(S);
		}

};

///	Solver class for the steam tables
/**
	This is intended to be a general purpose way of defining steam state in terms of any combination of properties, eg to find the pressure at which rho = 1.1 kg/m3 and u = 2500 kJ/kg, use:

		@code
			Solver2<Density,SpecificEnergy> SS;
			SteamCalculator S = SS.solve(1.1 * kg_m3, 2500.0 * kJ_kg);
			cerr << S->pres() << endl;
		@endcode

	Likewise, even with one of the properties being a correlation property, just use

		@code
			Solver2<Temperature,SpecificEnergy> SS;
			SteamCalculator S = SS.solve(450.0 * Kelvin, 2500.0 * kJ_kg);
			cerr << S->pres() << endl;
		@endcode

	Or even,

		@code
			Solver2<Temperature,Pressure> SS;
			SteamCalculator S = SS.solve(450.0 * Kelvin, 10.0 * bar);
			cerr << S->dens() << endl;
		@endcode

	@see Solver (solving for one un-correlated property)
*/
template<class FirstProp,class SecondProp,int FirstPropAlt=0, int SecondPropAlt=0>
class Solver2
	: public Solver2Base<FirstProp,SecondProp,FirstPropAlt,SecondPropAlt>{

	private:
		static const int maxiter=30;

	public:

		Solver2() : Solver2Base<FirstProp,SecondProp,FirstPropAlt,SecondPropAlt>(){
			//cerr << endl <<"Solver2<" << SteamProperty<FirstProp,FirstPropAlt>::name() << "," << SteamProperty<SecondProp,SecondPropAlt>::name() << ">::Solver2()";
		}

		/// Give the IAPWS-IF97 region number given (any combination of) property values
		/**
			Used in the same way as Solver2::solve but the solution is not done, only the region is found.

			@param fp Value of FirstProp property
			@param sp Value of SecondProp property

			@example
			To find the region for steam with u = 1500 kJ/kg and v = 0.02 m³/kg, use the following:
@code
Solver2<SpecificEnergy,SpecificVolume> SS;
cerr << SS.whichRegion(1500. * kJ_kg, 0.02 * m3_kg);
@endcode
		*/
		int whichRegion(const FirstProp &fp, const SecondProp &sp);

		/// Solve with no first guess provided
		/**
			@param fp Value of FirstProp property
			@param sp Value of SecondProp property
		*/
		SteamCalculator solve(const FirstProp &fp, const SecondProp &sp){
			int region = 0;
			SteamCalculator S,S2;

			try{
				//cerr << endl << "Solver2: solving for " << SteamProperty<FirstProp,FirstPropAlt>::name() << " = " << fp << ", " << SteamProperty<SecondProp,SecondPropAlt>::name() << " = " <<  sp;

				region = whichRegion(fp,sp);

				//cerr << endl << "Solver2: solving in region " << region;
				switch(region){
					case 1:
						S.set_pT(10.0 * bar, fromcelsius(100));
						S2 = solveRegion1(fp,sp,S);
						break;
					case 2:
						S.set_pT(6.0 * MPa, fromcelsius(400));
						S2 = solveRegion2(fp,sp,S);
						break;
					case 3:
						S.setRegion3_rhoT(1 / 0.00317 * kg_m3, fromcelsius(400));
						S2 = solveRegion3(fp,sp,S);
						break;
					case 4:
						S.setRegion4_Tx(fromcelsius(263.9),0.5);
						S2 = solveRegion4(fp,sp,S);
						break;
				}
			}catch(Exception *E){
				stringstream s;
				s << "Solver2::solve (with region=" << region << "): " << E->what();
				throw new Exception(s.str());
			}

			return S2;
		}

		/// Solve with a first guess provided
		/**
			@param fp Value of FirstProp property
			@param sp Value of SecondProp property
			@param firstguess SteamCalculator initialised with state to be used for first guess
		*/
		SteamCalculator solve(const FirstProp &fp, const SecondProp &sp, const SteamCalculator &firstguess){
			int region=0;
			try{
				region = whichRegion(fp,sp);
				switch(region){
					case 1:
						return solveRegion1(fp,sp,firstguess);
					case 2:
						return solveRegion2(fp,sp,firstguess);
					case 3:
						return solveRegion3(fp,sp,firstguess);
					case 4:
						return solveRegion4(fp,sp,firstguess);
				}
			}catch(Exception *E){
				stringstream s;
				s << "Solver2::solve (with region="<< region<<"): " << E->what();
				throw new Exception(s.str());
			}
		}

	private:

		SteamCalculator solveRegion3(const FirstProp &f1, const SecondProp &s1,const SteamCalculator &firstguess){
			throw new Exception("solveRegion3 not implemented");
		}

		SteamCalculator solveRegion4(const FirstProp &f1, const SecondProp &s1,const SteamCalculator &firstguess){

			try{
				// Just like region 1, except for it's T,x

				//cerr << endl << "Solver2::solveRegion4: ";

				SteamCalculator guess = firstguess;

				//cerr << endl << "Solver2::solveRegion4: firstguess copied to guess OK";

				if(firstguess.whichRegion()!=4){
					throw new Exception("Solver2::solveRegion4: First guess is not region 4");
				}

				SteamCalculator petT, petx;
				Temperature T,dT;
				Num x,dx;
				FirstProp f,Df, DfT, Dfx;
				SecondProp s,Ds,DsT, Dsx;
				Pressure p;

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
					dx = 0.001;
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

					Temperature dTMax = 0.1 * T;
					Temperature dTAbs = fabs(dT);
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
						throw new Exception("Solver2::solveRegion4: Exceeded maximum iterations");
					}
				}
			}catch(Exception *E){
				stringstream s;
				s<< "Solver2::solveRegion4: " << E->what();
				throw new Exception(s.str());
			}
		}

		SteamCalculator solveRegion1(const FirstProp &f1, const SecondProp &s1,const SteamCalculator &firstguess){

			SteamCalculator guess = firstguess;

			if(firstguess.whichRegion()!=1){
				throw new Exception("Solver2::solveRegion1: First guess is not region 1");
			}

			SteamCalculator petT, petp;
			Temperature T,dT;
			Pressure p,dp;
			FirstProp f,Df, DfT, Dfp;
			SecondProp s,Ds,DsT, Dsp;

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

				Temperature dTMax = 0.1 * T;
				Temperature dTAbs = fabs(dT);
				if(dTAbs > dTMax){
					//cerr << endl << "      ... limiting dT, too great";
					dT = dT * dTMax/dTAbs;
				}

				// Regulate max change in pressure

				Pressure dpMax = 0.4 * p;
				Pressure dpAbs = fabs(dp);
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
					Pressure p_sat = Boundaries::getSatPres_T(T);
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
					throw new Exception("Solver2::solveRegion1: Exceeded maximum iterations");
				}
			}
		}

		SteamCalculator solveRegion2(const FirstProp &f1, const SecondProp &s1,const SteamCalculator &firstguess){

			// Most of this is the same as for solveRegion1:

			//cerr << endl << "---------------------------------" << endl << "SOLVE REGION 2";
			SteamCalculator guess = firstguess;

			if(firstguess.whichRegion()!=2){
				throw new Exception("Solver2::solveRegion2: First guess is not region 2");
			}

			SteamCalculator petT, petp;
			Temperature T,dT;
			Pressure p,dp;
			FirstProp f,Df, DfT, Dfp;
			SecondProp s,Ds,DsT, Dsp;

			int niter=0;
			// If we are in region 1, then we will be iterating with pressure and temperature

			while(1){

				T = guess.temp();
				p = guess.pres();

				//cerr << endl << "Iter " << niter << ": p = " << p / MPa << " MPa, T = " << T << " (" << tocelsius(T) << "°C)";

				f = SteamProperty<FirstProp,FirstPropAlt>::get(guess);
				s = SteamProperty<SecondProp,SecondPropAlt>::get(guess);

				//cerr << ": " << SteamProperty<FirstProp,FirstPropAlt>::name() << " = " << f;
				//cerr << ", " << SteamProperty<SecondProp,SecondPropAlt>::name() << " = " << s;

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

				Temperature dTMax = 0.1 * T;
				Temperature dTAbs = fabs(dT);
				if(dTAbs > dTMax){
					//cerr << endl << "      ... limiting dT, too great";
					dT = dT * dTMax/dTAbs;
				}

				// Regulate max change in pressure

				Pressure dpMax = 0.5 * p;
				Pressure dpAbs = fabs(dp);
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
					Pressure pbound = Boundaries::getpbound_T(T);
					if(p > pbound){
						//cerr << endl << "     ... applying pbound limit";
						p = pbound;
					}
				}else{
					Pressure psat = Boundaries::getSatPres_T(T);
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
					throw new Exception("Solver2::solveRegion1: Exceeded maximum iterations");
				}
			}

		}
};


#endif
