from pylab import *
import freesteam

T = [(273.15 + x) for x in range(int(freesteam.TCRIT - 273.15))]

dpdT = [freesteam.psat_T(t)/1e6 for t in T]

plot(T,dpdT,'r-')
ion()
show()


