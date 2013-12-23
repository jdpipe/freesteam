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
http://office.microsoft.com/en-us/excel-help/load-or-unload-add-in-programs-HP010096834.aspx

To use the freesteam functions, click on the fx button on the formula
bar, select one of the Freesteam sub-category on the table below,
then enter your data.

-------------------------------------------------------------
|  Category             | First variable | Second variable  |
-------------------------------------------------------------
|  Freesteam 2.x (p,h)  |    pressure    |    enthalpy      |
|  Freesteam 2.x (p,s)  |    pressure    |    entropy       |
|  Freesteam 2.x (p,T)  |    pressure    |  temperature     |
|  Freesteam 2.x (p,u)  |    pressure    |  internal energy |
|  Freesteam 2.x (p,v)  |    pressure    |  specific volume |
|  Freesteam 2.x (sat.) |     properties at saturation      |
|  Freesteam 2.x (T,s)  |  temperature   |    entropy       |
-------------------------------------------------------------

Functions description and examples of usage are given in
the examples.xls Excel sheet.

Building freesteam.xll
----------------------

Requirements:
  - For 64 bits MinGW-w64 http://mingw-w64.sourceforge.net/
  - For 32 bits MinGW http://www.mingw.org/
  - GSL library http://www.gnu.org/software/gsl/

I recommend:
 - To use the tdm installer for MinGW-w64 http://tdm-gcc.tdragon.net/
 - To install MinGW, MinGW-w64 and xlw in paths without spaces
   (i.e. not in C:\Program Files)
 - To use precompiled GSL library and headers from oscats. Both 32 bits and
   64 bits versions are available at http://code.google.com/p/oscats/downloads/list


It is possible to install both MinGW and MinGW-w64
e.g. in C:\mingw32 and C:\mingw64 respectively. If so:
 - It is useless to install mingw32-make with MinGW-w64
   (it is included with MinGW 32 bits)
 - Because both C:\mingw32\bin and C:\mingw64\bin are defined
   in %PATH%, use non ambiguous compilers names like mingw32-gcc.exe
   and x86_64-w64-mingw32-gcc.exe

Set up GSL library locations of the LIBDIR and INC variables in the Makefile.

If required, create the config.h file:
 - copy ../config.h.in to ../config.h
 - Replace @VERSION@ by your version number

Finally:
    Use the MakeRelease-win32.bat batch files to build the 32 bits xll
    Use the MakeRelease-win64.bat batch files to build the 64 bits xll

