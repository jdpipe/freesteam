#ifndef SOLVER2_H
#define SOLVER2_H

#include "steamproperty.h"
#include "units.h"
#include "satcurve.h"
#include "exception.h"
#include "convergencetest.h"

/**
	Base class for all property solvers
*/
template<class FirstProp,class SecondProp,int FirstPropAlt=0, int SecondPropAlt=0>
class Solver2Base{

	protected:
	
		virtual int whichRegion(const FirstProp &fp,const SecondProp &sp) const{
			throw new Exception("Solver2Base::whichRegion() not implemented!");
		}
		
		virtual SteamCalculator solve(const FirstProp &fp, const SecondProp &sp){
			throw new Exception("Solver2Base::solve must be over-ridden");
		}
	
		FirstProp getFirstProp(SteamCalculator &S){
			return SteamProperty<FirstProp,FirstPropAlt>::get(S);
		}
		
		SecondProp getSecondProp(SteamCalculator &S){
			return SteamProperty<SecondProp,SecondPropAlt>::get(S);
		}

};

/**
	Solver class for the steam tables
	
	This is intended to be a general purpose way of defining steam state in terms of any combination of properties, eg to find the pressure at which rho = 1.1 kg/m3 and u = 2500 kJ/kg, use:
	
		Solver2<Density,SpecificEnergy> SS;
		SteamCalculator S = SS.solve(1.1 * kg_m3, 2500.0 * kJ_kg);
		cerr << S->pres() << endl;
		
	Likewise, even with one of the properties being a correlation property, just use
	
		Solver2<Temperature,SpecificEnergy> SS;
		SteamCalculator S = SS.solve(450.0 * Kelvin, 2500.0 * kJ_kg);
		cerr << S->pres() << endl;

	Or even,
	
		Solver2<Temperature,Pressure> SS;
		SteamCalculator S = SS.solve(450.0 * Kelvin, 10.0 * bar);
		cerr << S->dens() << endl;
	
*/		
template<class FirstProp,class SecondProp,int FirstPropAlt=0, int SecondPropAlt=0>
class Solver2 
	: public Solver2Base<FirstProp,SecondProp,FirstPropAlt,SecondPropAlt>{
	
	private:
		static const int maxiter=30;
		
	public:
		
		Solver2(){
			//cerr << endl <<"Solver2<" << SteamProperty<FirstProp,FirstPropAlt>::name() << "," << SteamProperty<SecondProp,SecondPropAlt>::name() << ">::Solver2()";
		}
		
		int whichRegion(const FirstProp &fp, const SecondProp &sp);
		
		/// Solve with no first guess provided
		/**
			@param fp Value of FirstProp property
			@param sp Value of SecondProp property
		*/
		SteamCalculator solve(const FirstProp &fp, const SecondProp &sp){
			int region = 0;
			SteamCalculator S;
			
			try{
				//cerr << endl << "Solver2: solving for " << SteamProperty<FirstProp,FirstPropAlt>::name() << " = " << fp << ", " << SteamProperty<SecondProp,SecondPropAlt>::name() << " = " <<  sp;

				region = whichRegion(fp,sp);		
				
				//cerr << endl << "Solver2: solving in region " << region;
				switch(region){
					case 1:
						S.set_pT(10.0 * bar, fromcelsius(100));
						return solveRegion1(fp,sp,S);
					case 2:
						S.set_pT(6.0 * MPa, fromcelsius(400));
						return solveRegion2(fp,sp,S);
					case 3:
						S.set_rhoT(1 / 0.00317 * kg_m3, fromcelsius(400));
						return solveRegion3(fp,sp,S);
					case 4:
						S.setSatSteam_T(fromcelsius(263.9));
						return solveRegion4(fp,sp,S);
				}
			}catch(Exception *E){
				stringstream s;
				s << "Solver2::solve (with region=" << region << "): " << E->what();
				throw new Exception(s.str());
			}
		}
		
		/// Solve with a first guess provided
		/**
			@param fp Value of FirstProp property
			@param sp Value of SecondProp property
			@param firstguess SteamCalculator initialised with state to be used for first guess
		*/
		SteamCalculator solve(const FirstProp &fp, const SecondProp &sp, const SteamCalculator &firstguess){
			try{
				int region = whichRegion(fp,sp);
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
		
		SteamCalculator solveRegion3(const FirstProp &f1, const SecondProp &s1,const SteamCalculator &firstguess){
			throw new Exception("solveRegion3 not implemented");
		}

		SteamCalculator solveRegion4(const FirstProp &f1, const SecondProp &s1,const SteamCalculator &firstguess){
			throw new Exception("solveRegion4 not implemented");
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
				
				Pressure dpMax = 0.2 * p;
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
