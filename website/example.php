<?php require("header.php") ?>

<h1>Example code</h1>


<?php 
require("nav.php");

show_nav(NULL, "Examples");

?>

<h2>Examples</h2>

<p>We propose to add more examples here when time permits. If you have any examples from your own work, I'll put them here and link to your site.</p>

<h2>Example 1</h2>

<p>This example demonstrates use of a few of the basic capabilities of freesteam via C code.<p>

<p>Calculate the rise in temperature seen in isentropic compression of a sample
of steam initially at 1 bar, 400 K, to a final pressure of 10 bar. Next, calculate
the saturation temperature at the final pressure.

<h3>Solution</h3>

<p>The following code is included in the source
distribution as <code>examples/isentropic.c</code>. See the <tt>examples/README.txt</tt> files for instructions on how to build and run it.</p>

<?php require("isentropic.html"); ?>

<p><span style="font-weight: bold;">Output:</span><br></p>

<pre>
This example demonstrates use of a few of the basic capabilities of freesteam.
It calculates the rise in temperature seen in isentropic compression of a sample
of steam initially at 1 bar, 400 K, to a final pressure of 10 bar. It also
calculates the saturation temperature for steam at that final pressure.

Initial temperature = 400.000000 K, pressure = 1.000000 bar
Entropy at initial state is 7.502401 kJ/kgK
New temperature is 684.505123 K at 10.000000 bar
Check: final entropy is 7.502401 kJ/kgK
Saturation temperature at 10.000000 bar is 453.035632 K.
</pre>

<h2>Example 2</h2>

<p>Use freesteam's Python bindings to quickly calculate some steam properties. The Python API has changed extensively with the release of freesteam 2.0, so you need to use that version for this to work.</p>

<h3>Solution</h3>

<pre>
<b>from freesteam import *</b>

<i># Calculation of properties for given (p,h)</i>
p = 1e5
h = 2000e3

S = <b>steam_ph</b>(p,h)

print S<b>.T</b>
print S<b>.rho</b>
print S<b>.s</b>

<i># Calculation of properties for given (p,T)</i>
S2 = <b>steam_pT</b>(p, 400)

print S2<b>.region</b>
print S2<b>.h</b>

<i># Calculation of saturation properties at given temperature, quality</i>
S3 = <b>steam_Tx</b>(400, 0.5)
print S3.h
print S3.region

<i># Calculation of saturated liquid properties at given pressure</i>
Tsat = <b>Tsat_p</b>(p)
print Tsat
S3 = steam_Tx(Tsat, 0)
print S3.h
</pre>

<h2>Example 3</h3>

<p>This example demonstrates how the new <a href="http://matplotlib.sourceforge.net/mpl_toolkits/mplot3d/tutorial.html">mplot3d</a> features in Matplotlib can be used to plot properties surfaces using data from freesteam. This example plots a (T,p,s) surface.

<pre>
<i>#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Example of 3D (T,s,p) surface plotted using new Matplotlib 3D-plotting feature.
# Contributed by Rod Stephenson. Thanks!</i>

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
<b>import freesteam</b>

def plot_tsp3d():
    fig = plt.figure()
    ax = Axes3D(fig)
    pp = np.logspace(-3,3,20)
    ss = np.linspace(0.,10., 40)
    xdata,ydata = np.meshgrid(pp, ss)
    zdata = np.zeros(xdata.shape)

    for i, p in enumerate(pp):
        for j, s in enumerate(ss):
            T = None
            if not <b>freesteam.bounds_ps(p*1e5,s*1e3,0)</b>:
                try:
                    T = <b>freesteam.steam_ps(p*1e5,s*1e3).T</b>
                except:
                    pass
            zdata[j, i]= T
    ax.plot_wireframe(xdata, ydata, zdata, rstride=1, cstride=1)
    ax.set_xlabel('Pressure / [bar]')
    ax.set_ylabel('Entropy / [kJ/kgK]')
    ax.set_zlabel('Temperature / [K]')

    TT0 = np.linspace(273.15, <b>freesteam.TCRIT</b>, 100)
    psat = [<b>freesteam.psat_T(T)</b>/1e5 for T in TT0]
    sf = [freesteam.region4_Tx(T,0).s/1e3 for T in TT0]
    sg = [freesteam.region4_Tx(T,1).s/1e3 for T in TT0]
    ax.plot(psat, sf, TT0,'k-')
    ax.plot(psat, sg, TT0,'r-')
    plt.show()

if __name__ == '__main__':
    plot_tsp3d()
</pre>

<p>The resulting plot can be rotated in 3D using the mouse. A screenshot is
shown below.</p>

<p><img src="example-tsp.png" /></p>

<? require_once("footer.php"); ?>
