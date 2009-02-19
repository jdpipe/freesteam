#!/usr/bin/python

# This example program can be used with the Python bindings 
# for freesteam. It plots a series of property curves on a T-s 
# diagram. For it to work, you need to have 'numpy' and 
# 'matplotlib' installed. For details of those, go to
# http://matplotlib.sf.net.

from freesteam import *

try:
	from pylab import *
except ImportError:
	print "This example requires that you have 'matplotlib' installed."
	print "You can download it from http://matplotlib.sf.net."
	import sys
	sys.exit(1)

hh = linspace(500,3700,200)

figure()
hold(True)
leg = []

pp = [Measurement(p,"bar") for p in [10,20,50,100,150,250,300,400,800]]

for p in pp:
	TT = zeros(size(hh))

	for i in range(size(hh)):
			S = steam_ph().solve(p,Measurement(hh[i],"kJ/kg"))
			TT[i] = S.temp().to("K")

	plot(hh,TT)
	leg.append("p = %.0f bar" % p.to("bar"))

legend(leg,loc='upper left')
ylabel('Temperature (K)')
xlabel('Enthalpy (kJ/kg)')
show()
