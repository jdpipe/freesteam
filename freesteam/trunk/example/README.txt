
Example program for freesteam
-----------------------------

This README is for the benefit of Windows users who have downloaded
and installed 'freesteam' using the freesteam-setup.exe program.

This directory contains a complete working example of code that
links to the freesteam library.

For both of these techiques you MUST use the GNU C++ Compiler ('g++').
Download and install MinGW and MSYS from the MinGW website
(http://www.mingw.org/). Check that it is working by typing 'g++ -v' at
the MSYS prompt.


Using SCons
-------------------

This is the preferred method. SCons (http://www.scons.org/) is a build tool 
that requires Python (http://www.python.org/) to be on your system. You should
ensure that you have got SCons version 0.96.93 or later installed on your
system.

Using your MSYS command prompt:
(you might need to change the first line)

  cp -R /c/Program\ Files/freesteam/example ~
  cd ~/example
  scons
  ./example

The 'scons' line should perform the necessary compilation steps for the 
example code, including locating the freesteam 'include' and 'lib' files.


Using GNU Make
--------------

You can also use GNU Make if you're a little reticent with SCons.

First you need to find the Makefile in the example code directory, and
open it and edit the line that starts with "FREESTEAM=" to ensure you
have the correct file location.

Then open your MSYS prompt and type

  cp -R /c/Program\ Files/freesteam/example .
  cd example
  make
  ./example

  
Expected output
---------------

In both cases, the expected output is

$ ./example.exe

Part (1) - density at 10 bar, 200°C
p1 = 5 bar
T1 = 473.15 K (200°C)
rho1 = 2.35275 kg/m³

Part (2) - isentropic compression to 10 bar
s1 = 7061.07 J/kgK
p2 = 10 bar
T2 = 556.418 K (283.268°C)

Part (3) - Finding p,T for v as above and u increased by 200 kJ_kg
v2 = 0.249638 m³/kg
u2 = 2.76613e+006 J/kg
u3 = 2.96613e+006 J/kg
p3 = 12.372 bar
T3 = 679.854 K (406.704°C)


Reporting problems
------------------

If you have any problems with the above instructions, please
let me know via the contact details at http://freesteam.sf.net/

-- 
John Pye
