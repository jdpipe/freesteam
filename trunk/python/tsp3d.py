#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Example of 3D (T,s,p) surface plotted using new Matplotlib 3D-plotting feature.
# Contributed by Rod Stephenson. Thanks!

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import freesteam

def plot_tsp3d():
    fig = plt.figure()
    ax = Axes3D(fig)
    pp = np.logspace(-3,3,20)*1e5
    ss = np.linspace(0.,10., 40)*1e3
    xdata,ydata = np.meshgrid(pp, ss)
    zdata = np.zeros(xdata.shape)

    for i, p in enumerate(pp):
        for j, s in enumerate(ss):
            T = None
            if not freesteam.bounds_ps(p,s,0):
                try:
                    T = freesteam.steam_ps(p,s).T
                except:
                    pass
            zdata[j, i]= T
    ax.plot_wireframe(xdata, ydata, zdata, rstride=1, cstride=1)
    ax.set_xlabel('Pressure / [kPa]')
    ax.set_ylabel('Entropy / [kJ/kgK]')
    ax.set_zlabel('Temperature / [K]')

    TT0 = np.linspace(273.15, freesteam.TCRIT, 100)
    psat = [freesteam.psat_T(T) for T in TT0]
    sf = [freesteam.region4_Tx(T,0).s for T in TT0]
    sg = [freesteam.region4_Tx(T,1).s for T in TT0]
    ax.plot(psat, sf, TT0,'k-')
    ax.plot(psat, sg, TT0,'r-')
    plt.show()

if __name__ == '__main__':
    plot_tsp3d()
