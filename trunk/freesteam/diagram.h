#ifndef DIAGRAM_H
#define DIAGRAM_H

#include "steamcalculator.h"
#include "steamproperty.h"
#include "satcurve.h"
#include "b23curve.h"
#include "b13curve.h"

#include <sstream>
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

			@todo: fix this, it doesn't seem to be working...
		*/
		static const std::string getMatlabPlotType();

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
			s << std::endl << getMatlabPlotType() << "(TMIN(:,1),TMIN(:,2),'w:');";
			s << std::endl << getMatlabPlotType() << "(TMAX(:,1),TMAX(:,2),'w-.');";

			s << std::endl << "legend('Sat Steam','Sat Water','B23','B13','Pmin','Pmax','Tmin','Tmax');";

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
			std::stringstream s;
			s << std::endl << "TMIN = [";

			const Temperature &Tref = T_MIN;

			const Pressure &pmin = P_MIN;
			const Pressure &pmax = P_MAX;
			Pressure pstep = (pmax - pmin) / double(200);
			for(Pressure p = pmin; p <= pmax; p += pstep){
				SteamCalculator S;
				S.set_pT(p,Tref);
				s << std::endl << "\t" << SteamProperty<Abscissa,AbsAlt>::get(S) << "\t" << SteamProperty<Ordinate,OrdAlt>::get(S);
			}
			s << std::endl << "];" << std::endl;
			return s.str();
		}

		std::string plotTmax(){
			std::stringstream s;
			s << std::endl << "TMAX = [";

			const Temperature &Tref = T_MAX;

			const Pressure &pmin = P_MIN;
			const Pressure &pmax = P_MAX;
			Pressure pstep = (pmax - pmin) / double(200);
			for(Pressure p = pmin; p <= pmax; p += pstep){
				SteamCalculator S;
				S.set_pT(p,Tref);
				s << std::endl << "\t" << SteamProperty<Abscissa,AbsAlt>::get(S) << "\t" << SteamProperty<Ordinate,OrdAlt>::get(S);
			}
			s << std::endl << "];" << std::endl;
			return s.str();
		}

		std::string plotpmin(){
			std::stringstream s;
			s << std::endl << "PMIN = [";

			const Pressure &pref = P_MIN;

			const Temperature &Tmin = T_MIN;
			const Temperature &Tmax = T_MAX;
			Temperature Tstep = (Tmax - Tmin) / double(200);
			for(Temperature T = Tmin; T <= Tmax; T += Tstep){
				SteamCalculator S;
				S.set_pT(pref,T);
				s << std::endl << "\t" << SteamProperty<Abscissa,AbsAlt>::get(S) << "\t" << SteamProperty<Ordinate,OrdAlt>::get(S);
			}
			s << std::endl << "];" << std::endl;
			return s.str();
		}

		std::string plotpmax(){
			std::stringstream s;
			s << std::endl << "PMAX = [";

			const Pressure &pref = P_MAX;

			const Temperature &Tmin = T_MIN;
			const Temperature &Tmax = T_MAX;
			Temperature Tstep = (Tmax - Tmin) / double(200);
			for(Temperature T = Tmin; T <= Tmax; T += Tstep){
				SteamCalculator S;
				S.set_pT(pref,T);
				s << std::endl << "\t" << SteamProperty<Abscissa,AbsAlt>::get(S) << "\t" << SteamProperty<Ordinate,OrdAlt>::get(S);
			}
			s << std::endl << "];" << std::endl;
			return s.str();
		}
};

#endif
