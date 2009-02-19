/*

freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2005  John Pye

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

#ifndef DIAGRAM_H
#define DIAGRAM_H

#include "../steamcalculator.h"
#include "../steamproperty.h"
#include "../satcurve.h"
#include "../b23curve.h"
#include "../b13curve.h"

#include <sstream>
#include <iostream>
#include <string>

/// Steam tables diagrams to plot with MATLAB
/**
	This class produces output which can be piped into an 'M-file' for MATLAB. It will generate plots on arbitrary sets of axes, showing the steam region boundaries for B13, B23, saturated steam line, saturated water line, as well as the limits of pressure and temperature according to IAPWS-IF97 release.

	See diagram.cli.cpp for example usage.
*/
template<class Abscissa, class Ordinate, int AbsAlt=0, int OrdAlt=0>
class Diagram{

	public:

		///Constructor
		Diagram(){
			// Intentionally empty
		}

		/// Get the type for the matlab plot
		/**
			Some combinations of variables need to be placed on log-log plots, etc.
			Use this method to specify the types of axes to be used by Matlab.
		*/
		static const std::string getMatlabPlotType(){
			return "plot";
		}

		/// Generate the M-file text and return it as a string
		/**
			This public method calls all the plot data methods and formats the graph with standard MATLAB commands
		*/
		std::string plot(){
			std::stringstream s;

			s << plotSatWater();
			s << plotSatSteam();
			s << plotB23();
			s << plotB13();
			s << plotTmin();
			s << plotTmax();
			s << plotpmin();
			s << plotpmax();

			s << std::endl << getMatlabPlotType() << "(SW(:,1),SW(:,2),'b-');";
			s << std::endl << "hold on;";

			s << std::endl << getMatlabPlotType() << "(SS(:,1),SS(:,2),'r-');";
			s << std::endl << getMatlabPlotType() << "(B23(:,1),B23(:,2),'g-');";
			s << std::endl << getMatlabPlotType() << "(B13(:,1),B13(:,2),'y-');";
			s << std::endl << getMatlabPlotType() << "(PMIN(:,1),PMIN(:,2),'c:');";
			s << std::endl << getMatlabPlotType() << "(PMAX(:,1),PMAX(:,2),'c-.');";
			s << std::endl << getMatlabPlotType() << "(TMIN(:,1),TMIN(:,2),'m:');";
			s << std::endl << getMatlabPlotType() << "(TMAX(:,1),TMAX(:,2),'m-.');";

			s << std::endl << "legend('Sat Water','Sat Steam','B23','B13','Ptriple','Pmax','Ttriple','Tmax');";

			s << std::endl << "xlabel('" << SteamProperty<Abscissa,AbsAlt>::name() << "')";
			s << std::endl << "ylabel('" << SteamProperty<Ordinate,OrdAlt>::name() << "')";
			s << std::endl << "title('Steam Property Boundaries: " << SteamProperty<Ordinate,OrdAlt>::name() << " vs " << SteamProperty<Abscissa,AbsAlt>::name() << "')";

			s << std::endl;

			return s.str();
		}

	private:

		std::string plotSatWater(){
			std::stringstream s;
			s << std::endl << "SW = [";

			Temperature Tstep = (T_CRIT - T_TRIPLE) / double(500);
			for(Temperature T = T_TRIPLE; T <= T_CRIT; T += Tstep){
				SteamCalculator S;
				S.setSatWater_T(T);
				s << std::endl << "\t" << SteamProperty<Abscissa,AbsAlt>::get(S) << "\t" << SteamProperty<Ordinate,OrdAlt>::get(S);
			}
			s << std::endl << "];" << std::endl;
			return s.str();

		}

		std::string plotSatSteam(){
			std::stringstream s;
			s << std::endl << "SS = [";

			Temperature Tstep = (T_CRIT - T_TRIPLE) / double(500);
			for(Temperature T=T_TRIPLE; T <= T_CRIT; T += Tstep){
				SteamCalculator S;
				S.setSatSteam_T(T);
				s << std::endl << "\t" << SteamProperty<Abscissa,AbsAlt>::get(S) << "\t" << SteamProperty<Ordinate,OrdAlt>::get(S);
			}
			s << std::endl << "];" << std::endl;
			return s.str();
		}

		std::string plotB23(){
			std::stringstream s;
			s << std::endl << "B23 = [";

			Temperature Tstep = (TB_HIGH - T_REG1_REG3) / double(200);
			for(Temperature T = T_REG1_REG3; T <= TB_HIGH; T += Tstep){
				SteamCalculator S;
				S.setB23_T(T);
				s << std::endl << "\t" << SteamProperty<Abscissa,AbsAlt>::get(S) << "\t" << SteamProperty<Ordinate,OrdAlt>::get(S);
			}
			s << std::endl << "];" << std::endl;
			return s.str();
		}

		std::string plotB13(){
			std::stringstream s;
			s << std::endl << "B13 = [";

			SteamCalculator S;
			S.setSatWater_T(T_REG1_REG3);
			Pressure p13 = S.pres();

			Pressure pstep = (P_MAX - p13) / double(200);
			for(Pressure p = p13; p <= P_MAX; p += pstep){
				S.set_pT(p,T_REG1_REG3,0.0);
				s << std::endl << "\t" << SteamProperty<Abscissa,AbsAlt>::get(S) << "\t" << SteamProperty<Ordinate,OrdAlt>::get(S);
			}
			s << std::endl << "];" << std::endl;
			return s.str();
		}

		std::string plotTmin(){
			return loopAlong<Temperature,Pressure>(T_TRIPLE,P_MIN,P_MAX,"TMIN");
		}

		std::string plotTmax(){
			return loopAlong<Temperature,Pressure>(T_MAX,P_MIN,P_MAX,"TMAX");
		}

		std::string plotpmin(){
			return loopAlong<Pressure,Temperature>(P_TRIPLE,T_MIN,T_MAX,"PMIN");
		}

		std::string plotpmax(){
			return loopAlong<Pressure,Temperature>(P_MAX,T_MIN,T_MAX,"PMAX");
		}

		template<class F, class S>
		std::string
		loopAlong(const F &fref,const S &smin, const S &smax, const string &title){
			std::stringstream ss;
			S sstep = (smax - smin) / double(200);

			ss << std::endl << title << " = [";
			for(S s=smin; s<= smax; s += sstep){
				SteamCalculator SC;
				SC.set<F,S>(fref,s);

				ss << std::endl << "\t";

				try{
					Abscissa a = SteamProperty<Abscissa,AbsAlt>::get(SC);
					ss << a;
				}catch(Exception *E){
					ss << "1.0e+1025";
					delete E;
				}

				ss << "\t";

				try{
					Ordinate o = SteamProperty<Ordinate,OrdAlt>::get(SC);
					ss << o;
				}catch(Exception *E){
					ss << "1.0e+1025";
					std::cerr << E->what();
					delete E;
				}
			}
			ss << std::endl << "];" << std::endl;
			return ss.str();
		}
};

template<>
const std::string Diagram<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0>::getMatlabPlotType();

template<>
const std::string Diagram<SpecificEnergy,Pressure,SOLVE_ENTHALPY,0>::getMatlabPlotType();

template<>
const std::string Diagram<SpecificEntropy,Pressure,SOLVE_ENTROPY,0>::getMatlabPlotType();

template<>
const std::string Diagram<Pressure,SpecificEntropy,0,SOLVE_ENTROPY>::getMatlabPlotType();

#endif
