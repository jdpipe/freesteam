<?php require("header.php") ?>

<h1>Example code</h1>


<?php 
require("nav.php");

show_nav(NULL, "Examples");

?>

<h2>Examples</h2>

<p>We propose to add more examples here when time permits. If you have any examples from your own work, I'll put them here and link to your site.</p>

<h2>Problem 1</h2>

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

<h2>Problem 2</h2>

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

<? require_once("footer.php"); ?>
