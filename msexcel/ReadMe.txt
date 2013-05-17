FREESTEAM XLL
=============

The freesteam add-in provides natives Excel functions for calculating
the properties of water and steam using the IAPWS-IF97 industry-standard
steam properties correlations.

32 bits version : freesteam.xll
64 bits version : freesteam-w64.xll

Using xll
-------------------
Follow instructions from the link below to load the xll add-in in Excel.
http://office.microsoft.com/en-us/excel-help/load-or-unload-add-in-programs-HP010096834.aspx#Bmexceladdin

To use the freesteam functions, click on the fx button on the formula
bar, select the Freesteam category, select a function then enter your
data. Functions description and examples of usage are given in the
freesteam-list-of-functions.xls Excel sheet.

Available Functions
-------------------
fstm_h_pT	Enthalpy (kJ/kg)
fstm_s_pT	Entropy (kJ/kg.K)
fstm_cp_pT	Isobaric heat capacity (kJ/kg.K)
fstm_cv_pT	Isochoric heat capacity (kJ/kg.K)
fstm_v_pT	Specific volume (m3/kg)
fstm_rho_pT	Density (kg/m3)
fstm_u_pT	Internal energy (kJ/kg)
fstm_k_pT	Thermal conductivity (W/m.K)
fstm_mu_pT	Dynamic viscosity (Pa.s)
fstm_w_pT	Speed of sound (m/s)
fstm_region_pT	Region of IAPWS-IF97
	
fstm_psat_T	Saturation pressure (bar)
fstm_hf_T	Liquid Enthalpy at saturation (kJ/kg)
fstm_hg_T	Vapour Enthalpy at saturation (kJ/kg)
	
fstm_Tsat_p	Saturation temperature (°C)
fstm_hf_p	Liquid Enthalpy at saturation (kJ/kg)
fstm_hg_p	Vapour Enthalpy at saturation (kJ/kg)

fstm_h_Tx	Enthalpy at saturation (kJ/kg)
fstm_u_Tx	Internal energy at saturation (kJ/kg)
fstm_v_Tx	Specific volume at saturation (m3/kg)

fstm_h_px	Enthalpy at saturation (kJ/kg)
fstm_u_px	Internal energy at saturation (kJ/kg)
fstm_v_px	Specific volume at saturation (m3/kg)

Building freesteam.xll
----------------------

Requirements:
  - xlw 5 http://xlw.sourceforge.net/index.shtml
  - For 64 bits MinGW-w64 http://mingw-w64.sourceforge.net/
  - For 32 bits MinGW http://www.mingw.org/

I recommend:
 - To use the tdm installer for MinGW-w64 http://tdm-gcc.tdragon.net/
 - To install MinGW, MinGW-w64 and xlw in paths without spaces
   (i.e. not in C:\Program Files)

It is possible to install both MinGW and MinGW-w64
e.g. in C:\mingw32 and C:\mingw64 respectively. If so:
 - It is useless to install mingw32-make with MinGW-w64 
   (it is included with MinGW 32 bits)
 - Because both C:\mingw32\bin and C:\mingw64\bin are defined 
   in %PATH%, use non ambiguous compilers names like mingw32-gcc.exe 
   and x86_64-w64-mingw32-gcc.exe

If required, create the config.h file: 
copy ../config.h.in to ../config.h then replace @VERSION@ by your version number

Finally Use the MakeRelease-winXX.bat batch files to build xlls

