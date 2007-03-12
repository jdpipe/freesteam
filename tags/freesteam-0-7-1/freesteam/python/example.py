#!/usr/bin/python
from freesteam import *
from pylab import *

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
