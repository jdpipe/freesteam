import matplotlib
matplotlib.use('gtkcairo')
from pylab import *
import freesteam

n = 2000.
T = [273.15 + (freesteam.TCRIT - 273.15)*x/n for x in range(n+1)]

dpdT = [freesteam.dpsatdT_T(t)/1e6 for t in T]
rhof = [freesteam.rhof_T(t) for t in T]
rhog = [freesteam.rhog_T(t) for t in T]

plot(T,dpdT,'g-')
hold(1)
plot(T,rhof,'b-')
plot(T,rhog,'r-')

figure()
T = [273.15 + (freesteam.TCRIT - 273.15)*x/n for x in range(n+1)]
rhof = [freesteam.drhofdT_T(t) for t in T]
rhog = [freesteam.drhogdT_T(t) for t in T]
plot(T,rhof,'b-')
plot(T,rhog,'r-')

show()

