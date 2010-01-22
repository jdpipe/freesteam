
FREESTEAM
=========

This is a package for calculating the properties of water and steam
using the IAPWS-IF97 industry-standard steam properties correlations.

freesteam includes a thorough set of test cases; you are encouraged to inspect
these for yourself to assure yourself that freesteam is accurate for the cases
you required.

freesteam is not a stand-alone program in its own right. Rather, it is a 'shared
library' that implements a range of functions that you can use to work out steam
properties in your own program that you write yourself.

This version 2.x release of freesteam is a complete re-write of freesteam in 
pure C. We have removed all the complicated C++ template code (excessively
complicated) as well as the units-of-measurement code (cute, but also
rather hard to maintain). This has the immediate advantage that the resulting
DLLs/libraries can be linked to from all C/C++ compilers, something which was 
not possible with the previous C++ version.

For more information, see
http://freesteam.sourceforge.net/


Installing freesteam
--------------------

If you're reading this file, it's possible that you may have already downloaded
and installed a binary version of freesteam on your machine. If not, please note
that we provide binary versions of freesteam for Windows and Ubuntu 9.10 users.
See the freesteam website for details.


Building freesteam
------------------

This software has been tested on Ubuntu Linux, Mac OS X and Windows XP. To build
it you will need GCC, Python, SCons and GSL installed on your machine.

http://www.python.org/
http://www.scons.org/
http://www.gnu.org/software/gsl/

In the case of Windows, you should use the MinGW/MSYS environment to build
freesteam. In the case of Mac OS X, you should install XCode (bundled with
your Mac), then download and build SCons (python setup.py install).

To build freesteam, just run 'scons' from the command-line. If there are any
error messages that you can't work out, please send an email the
freemsteam-users mailing list and enclose the 'config.log' file as well as the
SCons command-line output.

See also 'Special tips for Ubuntu 9.10 users' further below.


Accessing freesteam from Python
-------------------------------

freesteam includes language bindings that allow freesteam to be accessed from 
the Python scripting language. To build these language bindings you will 
additionally need to have SWIG available on your system.

http://www.swig.org/

If SWIG is detected and you have the necessary Python developer files (on
Ubuntu, you need to install the python-devel package), then the above call to 
'scons' should build the necessary extra files allowing you to type 'import
freesteam' from Python and then to be able to access the steam properties
functionality.

Usage of freesteam from Python is not yet properly documented, but there are
quite a few sample files in the 'python' subdirectory of the source code
distribution that can be used as templates to get you started.


Accessing freesteam from C and C++
----------------------------------

The various header (.h) files in the freesteam distribution define the API that
must be used to calculated steam properties using freesteam. The essential idea
of the C/C++ API is that first a 'steam state' must be specified, using one of
the freesteam_set_*(...) functions, eg

 SteamState S = freesteam_set_ph(p1, h1);

These functions return an object that can then be used for subsequent property
queries, eg 

 double v1 = freesteam_v(S);

There are a range of header files that define the various 'solver' functions
for different combinations of input variables, such as steam_ph.h, steam_Ts.h,
and so on.

It is also possible to define the state of steam within a particular region, if
you're sure you know which IAPWS-IF97 region you need. See the region*.h files
for this functionality.


Accessing freesteam from ASCEND
-------------------------------

Some preliminary bindings to allow freesteam calculations to be accessed from
the ASCEND modelling environment (http://ascend.cheme.cmu.edu) are provided.
To build these bindings, you must have ASCEND installed on your system, 
including the developer/header files, and you must have 'ascend-config' 
available on your PATH.

If ASCEND is successfully detected, you will be able to access a range of
'external relations' defining steam property relationships, which can then be
used to build up accurate models of steam equipment such as turbines, condensers
and so on. A model of a complete steam power station that uses
freesteam property evaluations is included with the ASCEND program, see
models/johnpye/rankine.a4c.


Accessing freesteam from Fortran
--------------------------------

The new C version of freesteam should be much easier to access from the Fortran
language than the previous C++ version. Some discussion on this topic can be
found in the online archive of the freesteam-users mailing list. Currently
no specific code is provided for this, but if there is sufficient interest it
could certainly be developed.


Accessing freesteam from Excel or OpenOffice
--------------------------------------------

freesteam is currently not accessible from spreadsheet programs. We do propose
to implement bindings for these environments, but it hasn't happened yet.

In the mean time, it is suggested that Visual Basic / OpenOffice Basic code
from the XSteam project will probably do what you need.
http://www.x-eng.com/XSteam_Information.htm


Automatic generation of property tables
---------------------------------------

Freesteam includes a script that will automatically produce a set of steam
tables similar to those presented in R. W. Haywood, 1990, "Thermodynamic Tables 
in SI (metric) Units", Cambridge University Press (out of print).

These tabulated steam tables include a Mollier h-s diagram which must first be 
generated. To produce these tables, compile freesteam, including the Python
language bindings, then:

cd ~/freesteam
  [or whatever the location of your working directory happens to be]
export LD_LIBRARY_PATH=~/freesteam
  [likewise]
cd python
python hsdiagram.py
  [save the resulting diagram as mollier.eps]
python tables.py
pslatex steamtable.tex
dvipdf steamtable.dvi
  [you should now have steamtable.pdf]

If you don't have the directory containing your libfreesteam.so in your
LD_LIBRARY_PATH (linux/mac) or PATH (windows), then the above won't work, so
make sure not the forget the first step.

Steam tables code was initially contributed to freesteam by Grant Ingram.


Special tips for Ubuntu 9.10 users
----------------------------------

On a clean Ubuntu 9.10 machine, a perfect development environment can be
established by installing the following packages on your machine:

  subversion
  swig
  python-dev
  scons
  texlive
  texlive-latex-extra
  python-matplotlib
  dvipng

With the above packages, you will be able to build and install freesteam,
as well as create the steam tables PDF and the related h-s diagram.

As of today (16 Dec 09), there seem to be some bugs in the generation of some
of the diagrams (python/*diagram.py) on this platform; stay tuned for fixes.


-- 
John Pye
Jan 2009

