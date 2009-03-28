import freesteam
import matplotlib
matplotlib.use('gtkcairo')
from pylab import *
import math, sys

figure()
hold(1)

# SATURATION CURVE

Tmin = 273.15
Tmax = freesteam.TCRIT
n = 1000
DT = (Tmax - Tmin)/n
TT0 = 273.15 + DT*array(range(n+1))
psat = array([freesteam.psat_T(T)/1e6 for T in TT0])
hf = [freesteam.region4_Tx(T,0).h/1e3 for T in TT0]
hg = [freesteam.region4_Tx(T,1).h/1e3 for T in TT0]

# REGION MAP

hmin = 0
hmax = 4500e3
Dh = hmax - hmin
pmin = 1.e-3*1e5
pmax = 1.e3*1e5
DP = pmax - pmin
pp = arange(pmin,pmax,DP/400)
hh = arange(hmin,hmax,Dh/400)
im = zeros((len(pp),len(hh)))
x = 0
for p in pp:
	#print "p = %f MPa" % (p/1e6)
	y = 0
	for h in hh:
		r = freesteam.region_ph(p,h)
		#print "p = %f MPa, h = %f kJ/kg, region[%d,%d] = %d, T = %f" % (p/1e6,h/1e3,x,y,r,S.T)
		im[x,y] = float(r) / 4.
		y += 1
	x += 1

imshow(im,extent=[hmin/1e3,hmax/1e3,pmin/1e6,pmax/1e6],origin='lower',aspect='auto',interpolation='nearest',alpha=0.6)

# FIXME add lines of constant temperature
TT = logspace(math.log10(273.15),math.log10(1073.15),30)
for T in TT:
	print "T =",T
	smin = freesteam.steam_pT(freesteam.PMAX,T).s
	smax = freesteam.region2_pT(1,T).s
	ss = linspace(smin,smax)
	print "smin =",smin, ", smax =",smax
	hh = []
	pp = []
	for s in ss:
		if not freesteam.bounds_Ts(T,s,0):
			continue;
		#print "T = ",T, " s =",s
		S = freesteam.steam_Ts(T,s)
		hh += [S.h/1e3]	
		pp += [S.p/1e6]
	plot(hh,pp,'g-')

# plot the sat curve
plot(hf,psat,'b-')
plot(hg,psat,'r-')

xlabel("h / [kJ/kg]")
ylabel("p / [MPa]")
show()

