import freesteam
import matplotlib
matplotlib.use('gtkcairo')
from pylab import *
import math

figure()
hold(1)
n = 400
TT = [273.15 + (freesteam.TCRIT - 273.15)*x/n for x in range(n+1)]

hf = [freesteam.region4_Tx(T,0).h/1e3 for T in TT]
hg = [freesteam.region4_Tx(T,1).h/1e3 for T in TT]
plot(hf,TT,'b-')
plot(hg,TT,'r-')

pp = logspace(-3,3)*1e5
hh = arange(50.,4500.,100)*1e3

hh1 = arange(50.,4500.,20)*1e3

x = []
y = []
u = []
v = []
for p in pp:
	plot(hh1/1e3,[freesteam.steam_ph(p,h).T for h in hh1],alpha=0.2)
	for h in hh:
		try:
			S = freesteam.steam_ph(p,h)
			x += [S.h/1.e3]
			y += [S.T]
			dy = freesteam.freesteam_deriv(S,'T','h','p')
			dx = 0.0005
			m = math.sqrt(dx**2 + dy**2)
			u += [dx/m]
			v += [dy/m]
		except:
			pass

quiver(x,y,u,v,alpha=0.6)
axis([0,4500,273.15,1073.15])
xlabel('h / [kJ/kg]')
ylabel('T / [K]')
show()
