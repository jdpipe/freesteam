
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
you will find that this makes a very convenient way to quickly
do steam calculations.

Note that the 'Measurement' class has a correspondence with the
'Units' class in the freesteam C++ code.

The set of units of measurement is listed in the source code in
the file measurement.cpp.

If you have any questions with the Python interface for
freesteam, please let me know via the contact details at
http://freesteam.sf.net. The Python interface is still fairly
new and I am keen to improve it with suggestions from users.

Windows users
-------------

The windows installer will place _freesteam.dll in your
Python directory, but only if Python was detected at the 
time you installed freesteam. If you installed python afterwards,
just re-run the freesteam setup program and the python 
components should then be instlled. You can check the detailed
install log to make sure.

Linux users
-----------

'scons install' should put your _freesteam.so and freesteam.py
files in the correct location so that python finds them.


-- 
John Pye
Mar 2007
