import freesteam
import matplotlib
matplotlib.use('gtkcairo')
from pylab import *
import math

figure()
hold(1)
n = 400
TT = [273.15 + (freesteam.TCRIT - 273.15)*x/n for x in range(n+1)]

sf = [freesteam.region4_Tx(T,0).s/1e3 for T in TT]
sg = [freesteam.region4_Tx(T,1).s/1e3 for T in TT]
plot(sf,TT,'b-')
plot(sg,TT,'r-')

pp = logspace(-3,3)*1e5
ss = arange(0.,10.,0.1)*1e3

x = []
y = []
u = []
v = []
for p in pp:
	TT = []
	for s in ss:
		try: 
			T = freesteam.steam_ps(p,s).T
		except:
			T = None
		TT += [T]
	plot(ss/1e3,TT,alpha=0.2)
	print "pp =",pp
	print "ss =",ss
	print "TT =",TT
	#sys.exit(1)

#quiver(x,y,u,v,alpha=0.6)
axis([0,10,273.15,1073.15])
xlabel('s / [kJ/kgK]')
ylabel('T / [K]')
show()
