FREESTEAM XLL
=============

The freesteam add-in provides natives Excel function for calculating
the properties of water and steam using the IAPWS-IF97 industry-standard
steam properties correlations.

32 bits version : freesteam.xll
64 bits version : freesteam-w64.xll

Using xll
-------------------
Follow instructions from the link below to load the freesteam.xll add-in
in Excel.
http://office.microsoft.com/en-us/excel-help/load-or-unload-add-in-programs-HP010096834.aspx#Bmexceladdin

To use the freesteam functions, click on the fx button on the formula
bar, select the Freesteam category, select a function then enter your
data. Functions description and examples of usage are given in the
freesteam-list-of-functions.xls Excel sheet.


Building freesteam.xll
----------------------

Requirements:
  - xlw 5 http://xlw.sourceforge.net/index.shtml
  - MinGW-w64 http://mingw-w64.sourceforge.net/

I recommend to use the tdm installer for MinGW-w64 http://tdm-gcc.tdragon.net/
I recommend to install MinGW-w64 and xlw in paths without spaces (i.e. not in C:\Program Files)

If required, create the config.h file: 
copy ../config.h.in to ../config.h then replace @VERSION@ by your version number

Finally use the MakeRelease.bat batch file to generate 32 and 64 bits xlls

