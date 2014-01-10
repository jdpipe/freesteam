FREESTEAM XLL
=============

The freesteam add-in for Microsoft Excel provides fast machine-level functions
for calculating the properties of water and steam using the industry-standard 
IAPWS-IF97 steam property correlations.

The XLL was primarily developed by Ben Besses, with further input from John Pye.


Using freeseteam.xll
--------------------
Follow instructions from the link below to load the xll add-in in Excel.
http://office.microsoft.com/en-us/excel-help/load-or-unload-add-in-programs-HP010096834.aspx

To use the freesteam functions, click on the 'fx' button on the formula bar, 
select the Freesteam category, select a function then enter your data. Function
descriptions and examples of usage are given in the 'examples.xls' file 
provided.

IMPORTANT NOTE: freesteam.xll currently does NOT use base SI units in the way 
that the freesteam C and Python libraries do. You need to check the function 
description and make sure you are providing values in the units requested. For
example, temperatures are in kelvin and pressures are in bar.


Building freesteam.xll
----------------------

If you are running 64-bit Microsoft Office, then you need a 64-bit MinGW-w64
environment. If you are running 32-bit Microsoft Office, you need 32-bit 
MinGW32. We recommend that you set up your build environment as specified at 
the following link. Other configurations are possible, but it's harder to help
you if you strike difficulties.

http://ascend4.org/Setting_up_a_MinGW-w64_build_environment

The freesteam.xll file can be built as part of the overall freesteam build
process. Once you have GSL built/installed, you should be able to just
run 'scons' in your ~/freesteam folder, and everything including the XLL will
be built for you.

Note that we have included an embedded copy of the just the needed parts of 
XLW (the Excel wrapper tool, http://xlw.sf.net), version 5.0. We found that it
was much simpler to provide and build this tool on the fly compared with using
a pre-compiled version.

Note also that the recommended TDM-GCC compiler does NOT provide support for the
-static-libstdc++ compiler flag. If you use a different version of GCC, you 
may need to add this compiler flag in order to avoid generating undesired DLL
runtime dependencies (it's much easier if the XLL is statically linked to
all the stuff it needs).

-- 
John Pye and Ben Besses
Jan 2014

