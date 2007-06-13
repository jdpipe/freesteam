<?php require("header.php") ?>

<h1>Example code</h1>


<?php 
require("nav.php");

show_nav(NULL, "Examples");

?>

<h2>Examples</h2>

<p>We propose to add more examples here when time permits. If you have any examples from your own work, I'll put them here and link to your site.</p>

<h2>Problem 1</h2>

<p>Here is a sample problem that gives a fair indication of what is possible with freesteam. It involves direct evaluation from the IAPWS-IF97 correlation, then one-variable iterative solution, then two-variable iterative solution of the steam properties. </p>

<ol>
  <li>calculate the density of steam at 5 bar and 200&deg;C</li>

  <li>calculate the temperature if the steam from (1) is isentropically
compressed to 10 bar.</li>

  <li>calculate the pressure and temperature of steam at the same
specific volume as (2) but with the specific internal energy raised by
200 kJ/kg</li>
</ol>

<h3>Solution</h3>

<p>The following code is included in the source
distribution as <code>example.cpp</code>. See the main page instructions for how to build and run it..</p>

<pre>#include "steamcalculator.h"
#include "solver.h"
#include "solver2.h"
#include &lt;iostream&gt;
#include &lt;iomanip&gt;
using namespace std;

int main(){
&nbsp;&nbsp;&nbsp;&nbsp;try{
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SteamCalculator S1, S2, S3;

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<B>// turn on display of units</B>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr.flags(ios_base::showbase);

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<B>// initialise T1, p1, p2</B>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Temperature T1 = fromcelsius(200.0);
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pressure p1 = 5.0 * bar;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pressure p2 = 10.0 * bar;

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<B>// Part 1</B>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; endl &lt;&lt; "Part (1) -"
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"density at 10 bar, 200&deg;C" &lt;&lt; endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "p1 = " &lt;&lt; p1/bar &lt;&lt; " bar" &lt;&lt; endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "T1 = " &lt;&lt; T1;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; " (" &lt;&lt; tocelsius(T1) &lt;&lt; "&deg;C)" &lt;&lt; endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;S1.set_pT(p1, T1);
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Density rho1 = S1.dens();
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "rho1 = " &lt;&lt; rho1 &lt;&lt; endl;

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<B>// Part 2</B>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; endl &lt;&lt; "Part (2)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"- isentropic compression to 10 bar" &lt;&lt; endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SpecificEntropy s1 = S1.specentropy();
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "s1 = " &lt;&lt; s1 &lt;&lt; endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "p2 = " &lt;&lt; p2/bar &lt;&lt; " bar" &lt;&lt; endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Solver&lt;Pressure,SpecificEntropy,Temperature&gt; PS(p2, s1);
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;S2 = PS.solve(0.0001 * kJ_kgK, 0.0001 * Kelvin);
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Temperature T2 = S2.temp();
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "T2 = " &lt;&lt; T2;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; " (" &lt;&lt; tocelsius(T2) &lt;&lt; "&deg;C)" &lt;&lt; endl;

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<B>// part (3) - Finding p,T for v as above</B>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<B>// and u increased by 200 kJ_kg</B>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; endl &lt;&lt; "Part (3) - Finding p,T for v as above"
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;&lt; "and u increased by 200 kJ_kg" &lt;&lt; endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SpecificVolume v2 = S2.specvol();
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SpecificEnergy u2 = S2.specienergy();
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "v2 = " &lt;&lt; v2 &lt;&lt; endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "u2 = " &lt;&lt; u2 &lt;&lt; endl;

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SpecificEnergy u3 = u2 + 200.0 * kJ_kg;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "u3 = " &lt;&lt; u3 &lt;&lt; endl;

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Solver2&lt;SpecificEnergy,SpecificVolume&gt; SUV;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;S3 = SUV.solve(u3,v2);
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Temperature T3 = S3.temp();
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pressure p3 = S3.pres();
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "p3 = " &lt;&lt; p3/bar &lt;&lt; " bar" &lt;&lt; endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "T3 = " &lt;&lt; T3;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; " (" &lt;&lt; tocelsius(T3) &lt;&lt; "&deg;C)" &lt;&lt; endl;

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;exit(0);

&nbsp;&nbsp;&nbsp;&nbsp;}catch(Exception *E){
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cerr &lt;&lt; "Error: " &lt;&lt; E-&gt;what();
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;exit(1);
&nbsp;&nbsp;&nbsp;&nbsp;}
}
</pre>

<p><span style="font-weight: bold;">Output:</span><br></p>

<pre>$ ./example<br><br>Part (1) - density at 10 bar, 200&deg;C<br>p1 = 5 bar<br>T1 = 473.15 K (200&deg;C)<br>rho1 = 2.35275 kg/m3<br><br>Part (2) - isentropic compression to 10 bar<br>s1 = 7061.07 J/kgK<br>p2 = 10 bar<br>T2 = 556.418 K (283.268&deg;C)<br><br>Part (3) - Finding p,T for v as above and u increased by 200 kJ_kg<br>v2 = 0.249638 m3/kg<br>u2 = 2.76613e+06 J/kg<br>u3 = 2.96613e+06 J/kg<br>p3 = 12.372 bar<br>T3 = 679.854 K (406.704&deg;C)<br></pre>

<h2>Problem 2</h2>

<p>Use freesteam's Python bindings to quickly calculate some steam properties. This currently requires the CVS version of freesteam.</p>

<h3>Solution</h3>

<pre>
<b>from freesteam import *</b>

p = <b>Measurement</b>(1,"bar")
h = <b>Measurement</b>(2000,"kJ/kg")

S = <b>steam_ph().solve(</b>p,h<b>)</b>

print S<b>.temp()</b>
</pre>

<? require_once("footer.php"); ?>