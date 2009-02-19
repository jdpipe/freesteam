freesteam/emso/README.txt

INTEGRATION OF FREESTEAM WITH EMSO
----------------------------------

A DLL which exposes freesteam methods to the system modelling
package EMSO is under development currently.

Please follow these exact steps when building the DLL (or
shared object, '.so' file in the case of linux) for EMSO.
That way we can diagnose any problems which arise.

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
IMPORTANT NOTE:
emsofreesteam does NOT build
correctly under cygwin at the
moment. You need to download
and install MinGW and MSYS
then type 'make emso' under
that environment
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

1. Build and test freesteam (you will need to have CppUnit
   installed, this is probably rather tricky on mingw, so you
   can skip this step if you want).

       make test

2. Install EMSO according to instructions given on the EMSO
   website,

       http://rps.eng.br/download.html

3. Put EMSO header files in the standard INCLUDE path so that
   they are accessible as <emso/emso_types.h> and
   <emso/external_object.h>

       On Windows, this would be
       c:\Windows\usr\local\include\emso

       On Linux, this would be /usr/local/include/emso

4. If using cygwin, modify the <emso/emso_types.h> file
   replacing all occurences of

       #ifdef WIN32

   with

       #if defined(WIN32) || defined(__CYGWIN__)

5. Build the emsofreesteam shared object / DLL

       cd emso
       make

   You should find a DLL/SO file generated as a result

       On Windows, this would be emsofreesteam.dll
       On Linux, this would be libemsofreesteam.so

   If you want to get some debug output from emsofreesteam,
   you can do this

        cd emso
        export EMSO_DEBUG=1
        make -e

   You should then see a warning during compilation,

        EMSO DEBUG MODE IS SET - START EMSO FROM A CONSOLE TO
        SEE DEBUG OUTPUT

   (start the emso.exe/emso binary from a console window and
   you will see debug output in this case)

6. Copy the shared object / DLL to its required location,
   and rename it according to the requirements of EMSO. This
   means taking out the version number for the DLL and
   placing it in the 'interface' directory within the EMSO
   base directory.

       On windows, this would be c:\Program
       Files\EMSO\interface\emsofreesteam.dll

       On Linux, this would be
       /usr/local/emso/interface/libemsofreesteam.so

   You can do this automatically in most cases by just typing

       make install

   from the freesteam/emso directory where you built
   emsofreesteam.dll. The location where the DLL/SO is copied
   is defined in you platform-specific makefile, eg

       freesteam/make-pc-i686-mingw32.mak

   where you will see a line beginning

       EMSO_INCLUDE_DIR =

7. Start emso and open the test/emsotest.mso file included
   with the freesteam distribution.

8. Right-click on test2, click 'check consistency' and then
   try running the simulation.


At present, on Windows with MinGW/MSYS under WinXP, this
works OK. If you find that these steps break for you,
please let me know by email to

john_DOT_pye#student-unsw-edu-au

If you do have any problems, and you're using Windows, I
suggest you download and install the Microsoft Dependency
Walker, from http://www.dependencywalker.com/ and check
to make sure you're not having DLL dependency problems.


EMSO LIBRARIES WHICH USE FREESTEAM
----------------------------------

Included in the freesteam distribution are some EMSO models
and flowsheets which demonstrate how to use emsofreesteam.

In general, you can get almost any combination of steam
properties given specified input properties.

Function calls to emsofreesteam are of the form

        outputproperties_inputproperties

For example,

    freesteam as CalcObject(File="emsofreesteam");
        T = freesteam.T_ph(p,h)

For valid combinations of input and output properties, see
the emsohooks file for the latest options, or below for a
(possibly out-of-date) list:

inputs        outputs
------        -------
pTx           T   rho
ph            p   x
pu            v   k
uv            s   mu
ps            h   Tsvx
steamT        cp  Tuvx
waterT        cv  region

Bear in mind that some of these input parameter combinations
will invoke two-way solvers and thus could be be
computationally much more expensive that other options. If
possible, choose an input parameter combination that will be
cheap in the steam region(s) of interest, and set appropriate
default values of the steam variables.

---------------------------------
John Pye
Wednesday, 2 March 2005

