
--------------------------------------------------------------------------
freesteam
--------------------------------------------------------------------------

freesteam is an open source implementation of international-standard steam tables from the International Association for the Properties of Water and Steam (IAPWS):

  . IAPWS-IF97 Industrial formulation (high speed, but complex code)
  . IAPWS-95 Scientific formulation (low speed, but simple code)

freesteam is currently under active development, please contact us via the details below if you are interested in using freesteam or want to contribute.

CONTENTS

    1. What can I do with it?
    2. Quick start
    3. Other compilers
    4. License
    5. Future directions
    6. Links
    7. Contact details

--------------------------------------------------------------------------
1. What can I do with it?
--------------------------------------------------------------------------

You can use freesteam to calculate the properties of steam and water given its state. The IAPWS-IF97 equations, in their present form, allow you to calculate the following

    Enthalpy                  h(p,T)
    Entropy                   s(p,T)
    Isobaric heat capacity    cp(p,T)
    Isochoric heat capcity    cv(p,T)
    Specific volume           v(p,T)
    Density                   rho(p,T)
    Internal energy           u(p,T)
    Saturation pressure       psat(T)
    Saturation temperature    Tsat(p)
   
The IAPWS-95 equations, in their present form, allow you to calculate the above results but in terms of *density* and *temperature*, rather than pressure and temperature.

We are currently working on enhancing the freesteam code so that you will be able to calculate these parameters in terms of more different combinations of specified variables. This involves enhancing the rootsolving capabilities already packaged with the code.

--------------------------------------------------------------------------
2. Quick start
--------------------------------------------------------------------------

If you haven't already, get the code from http://sourceforge.net/projects/freesteam

Download the source files then type 'make'. You might need to configure the location of your compiler etc. at the top of the Makefile.

If you want to run the test methods, you'll need to get a copy of CppUnit. You can get this at
http://sourceforge.net/projects/cppunit. You will then be able to type 'make test' and you should see a set of compiler-time and run-time test results.

The above steps will result in a file 'libsteam.a' which you can link to in your own code, along with steamcalculator.h.

If you have trouble building CppUnit, you may possibly be able to use a binary version. Binary versions for cygwin are available via the cygwin installer.

--------------------------------------------------------------------------
3. Other compilers
--------------------------------------------------------------------------

freesteam is known to compile and complete all tests correctly with 
  . GCC 3.3.3 (cygwin special)
  
We intend to get freesteam working with other compilers, in particular GCC/mingw, VC++ and the Intel C++ compiler. It might even work with those compilers already. It is anticipated that the code will not work in present form with VC++ 6.0 because of the complexity of the template methods used in the units-of-measurement class, 'units.h'.

We are going to be regularly testing under MinGW as well as Cygwin, stay tuned for this.

--------------------------------------------------------------------------
4. License
--------------------------------------------------------------------------

freesteam is released un the GNU GPL, which is available online at
http://www.gnu.org/copyleft/gpl.html

It is also distributed with the source code distribution for freesteam, as LICENSE.txt.

--------------------------------------------------------------------------
5. Future directions
--------------------------------------------------------------------------

* We will shortly be releasing extra test cases that confirm the 
  consistency of values returned from the two correlations.

* We will be developing full solvers to allow any pair of steam properties 
  to be used to specify state, for example density and internal energy, 
  p(rho,u), etc.

* We plan to put in place hooks that will allow freesteam to be used from 
  within python, possibly using SWIG.

* An interactive commandline steam tables program will be developed for 
  easy demonstration/interactive testing.

--------------------------------------------------------------------------
6. Links
--------------------------------------------------------------------------

IAPWS-IF97 steam tables
http://www.iapws.org/
http://www.iapws.org/relguide/visc.pdf
http://www.iapws.org/relguide/thcond.pdf
http://www.iapws.org/relguide/IF97.pdf

Bernhard Spang's summary of IF97
http://www.cheresources.com/iapwsif97.shtml

IAPWS-95 scientific formulation
http://www.iapws.org/relguide/IAPWS95.pdf

CppUnit
http://cppunit.sourceforge.net/cgi-bin/moin.cgi
http://sourceforge.net/projects/cppunit

Rettig's units-of-measurement stuff
http://www.embedded.com/shared/printableArticle.jhtml?articleID=9900094
http://www.embedded.com/code/2001code.htm

Brent solver
http://www.library.cornell.edu/nr/bookcpdf.html

--------------------------------------------------------------------------
7. contact details
--------------------------------------------------------------------------

http://pye.dyndns.org/freesteam/
http://sourceforge.net/projects/freesteam/

John Pye       jdpipe?users_sourceforge_net
Don Peterson   someonesdad?users_sourceforge_net

Monday, 29 November 2004