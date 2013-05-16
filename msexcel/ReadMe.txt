FREESTEAM XLL
=============

The freesteam.xll add-in provides natives Excel function for calculating
the properties of water and steam using the IAPWS-IF97 industry-standard
steam properties correlations.

Using freesteam.xll
-------------------
Follow instructions from the link below to load the freesteam.xll add-in
in Excel.
http://office.microsoft.com/en-us/excel-help/load-or-unload-add-in-programs-HP010096834.aspx#Bmexceladdin

To use the freesteam functions, click on the fx button on the formula
bar, select the Freesteam category, select a function then enter your
data. Functions description and examples of usage are given in the
FreesteamFunction Excel sheet.


Building freesteam.xll
----------------------

Requirements:
    XLW (xlw-4.0.0f0.exe) from https://sourceforge.net/projects/xlw/
    MinGW from https://sourceforge.net/projects/mingw/

Use the MakeRelease.bat batch file or type mingw32-make

(Alternatively, you can use the FreeSteamXLL.cbp with Code::Blocks)

