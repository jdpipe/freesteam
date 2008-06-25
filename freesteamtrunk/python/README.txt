
freesteam for Python users
--------------------------

freesteam will put the necessary libraries into your Python 
'site-packages' folder such that you can use the following 
Python code:

  from freesteam import *
  
  p = Measurement(10,"bar")
  T = Measurement(300,"K")
  S = Steam()
  S.set_pT(p,T)
  print "density = %f kg/m3" % S.dens().to("kg/m3")
  
If you try the following using IPython (http://ipython.scipy.org)
you will find that this makes a convenient way to quickly
do steam calculations. You might want to make the 'Measurement'
class a little faster to type, for example as follows:

  from freesteam import *
  M = Measurement
  p = M(1,"bar")

Note that this 'Measurement' class has a correspondence with the
'Units' class in the freesteam C++ code.

The set of units of measurement is listed in the source code in
the file measurement.cpp. If you find yourself needing units of
measurement that are not available with the provided library,
let me know and I can add them in the next release.

If you have any questions with the Python interface for
freesteam, please let me know via the contact details at
http://freesteam.sf.net. The Python interface is still fairly
new and I am keen to improve it with suggestions from users.

Windows users
-------------

The windows installer will place _freesteam.dll in your
Python directory, but only if Python 2.5 was detected at the 
time you installed freesteam. If you installed Python afterwards,
just re-run the freesteam setup program and the Python 
components should then be installed. You can check the detailed
install log to make sure that the required DLL is in place.

Linux users
-----------

'scons install' should put your _freesteam.so and freesteam.py
files in the correct location so that python finds them.

An RPM for Fedora is available. You must install 'freesteam-python'
in order to get access to freesteam from the Python language.

Example python script
---------------------

A small example script is included in the the freesteam package.
On Windows, it is installed in the same directory as this
README file. It can be run with python and wil produce a simple
'T-s' diagram for steam, plotted using the matplotlib plotting
library, which you can download from http://matplotlib.sf.net.


-- 
John Pye
Aug 2007
