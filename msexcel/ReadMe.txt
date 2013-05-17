FREESTEAM XLL
=============

The freesteam add-in provides natives Excel function for calculating
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
 - Because both C:\mingw32\bin and C:\mingw64\bin should be defined 
   in %PATH%, use non ambiguous compilers names like mingw32-gcc.exe 
   and x86_64-w64-mingw32-gcc.exe

If required, create the config.h file: 
copy ../config.h.in to ../config.h then replace @VERSION@ by your version number

Finally Use the MakeRelease-winXX.bat batch files to build xlls

