<?php

$f=$_SERVER['DOCUMENT_ROOT'].$_SERVER['PHP_SELF'];
$t=filemtime($f);

header("Last-Modified: " . gmdate("D, d M Y H:i:s",$t) . " GMT");

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
  <title>freesteam: Steam Tables, Open Source, IAPWS-IF97</title>
  <link rel="stylesheet" media="screen" href="freesteam.css"
 type="text/css">
  <meta name="KEYWORDS"
 content="steam tables, IAPWS, steam properties, density, water, steam, IF97, saturated, superheated, subcooled, properties, enthalpy, entropy, specific heat, thermodynamics, thermo-fluids, IAPWS-95">
  <meta name="DESCRIPTION"
 content="Open source implementation of IAPWS-IF97 industry-standard steam tables in C++. IAPWS-95 (scientific use) correlations also included.">
</head>
<body>
<h1>freesteam<br></h1>

<table><tr><td valign=top>
<div class="nav"><span style="font-weight: bold;">Current version: </span>0.5.3<br>
<span style="font-weight: bold;">Released: </span>22 Mar 2005<br>
<br>
Quick links
<ul>
  <li><a t="Project Page" href="http://sf.net/projects/freesteam">SourceForge</a></li>
  <li><a title="Via ViewCVS on SourceForge"
 href="http://cvs.sourceforge.net/viewcvs.py/freesteam/freesteam/CHANGELOG.txt?only_with_tag=HEAD&amp;view=markup">Change
log</a><br>
  </li>
  <li><a title="File releases at SourceForge"
 href="http://sourceforge.net/project/showfiles.php?group_id=120898">Download</a>
  </li>
  <li><a title="Mantis bug tracker at pye.dyndns.org" href="/mantis/">Bug
Tracker</a></li>
  <li><a title="ViewCVS at SourceForge"
 href="http://cvs.sourceforge.net/viewcvs.py/freesteam/freesteam">Browse
CVS</a></li>
  <li><a title="Code docs generated with Doxygen"
 href="http://pye.dyndns.org/freesteam/docs/">Class Documentation</a></li>
  <li><a href="properties/">Property Diagrams</a><br>
  </li>
</ul>
Contents
<ul>
  <li><a href="#whatcanidowithit">What can I do with it?</a> </li>
  <li><a href="#quickstart">Quick start</a> </li>
  <li><a href="#othercompilers">Other compilers</a> </li>
  <li><a href="#license">License</a></li>
  <li><a href="#bugs">Bugs</a><br>
  </li>
  <li><a href="#futuredirections">Future directions</a> </li>
  <li><a href="#links">Links</a> </li>
  <li><a href="#contactdetails">Contact details</a></li>
</ul>
</div>

</td><td valign=top>

<h2>Introduction</h2>
<p><b>freesteam</b> is an open source implementation of
international-standard steam
tables from the <a href="http://www.iapws.org">International Association for the Properties of Water
and
Steam</a> (IAPWS):</p>
<ul>
  <li>IAPWS-IF97 Industrial formulation (high speed, but complex code) </li>
  <li>IAPWS-95 Scientific formulation (low speed, but simple code)</li>
</ul>
A plug-in to allow freesteam to be used from the EMSO simulation
package has been developed; stay tuned for more of these plug-ins.<br>
<p>If you find <b>freesteam</b> useful, or you have some comments, or
you think you might be able
to help with its development, please <span style="font-weight: bold;">contact
us</span> using the details below.</p>
<h2><a name="whatcanidowithit"></a>1. What can I do with it?</h2>
<h3>1.1 IAPWS-IF97 Industrial Formulation<br>
</h3>
<p>You can use freesteam to calculate the properties of steam and water
given its state, using the IAPWS-IF97 equations.<br>
</p>
<table style="text-align: left;" cellspacing="10">
  <tbody>
    <tr>
      <td style="vertical-align: top;" class="bord">
      <p>First you must specify
the <span style="font-weight: bold;">state </span>of the steam, <br>
using one of the following combinations of variables:<br>
      </p>
      <p>(p,T) for subcooled/superheated steam.<br>
(rho,T) for supercritical steam.<br>
(T,x) for saturated steam<br>
(p,T,x) anywhere<br>
(p) plus one of (s,h,u,v,cp,cv,rho)<br>
(T) plus one of (s,h,u,v,cp,cv,rho)</p>
      <p>By two-way iteration, state can be solved for:<br>
(u,v) (p,h) (p,u) (p,s) (T,s) (T,h)<br>
      </p>
      <p>Alternatively, you can specify:</p>
      <p>saturated water at T<br>
saturated steam at T<br>
      </p>
      </td>
      <td style="vertical-align: top;" class="bord">
      <p> Then, you can obtain the value of any of <br>
the following steam <span style="font-weight: bold;">properties</span>:</p>
      <table>
        <tbody>
          <tr>
            <td>p</td>
            <td style="vertical-align: middle;">Pressure</td>
          </tr>
          <tr>
            <td>T</td>
            <td>Temperature</td>
          </tr>
          <tr>
            <td>h</td>
            <td>Enthalpy</td>
          </tr>
          <tr>
            <td>s</td>
            <td>Entropy</td>
          </tr>
          <tr>
            <td>cp</td>
            <td>Isobaric heat capacity<br>
            </td>
          </tr>
          <tr>
            <td>cv</td>
            <td>Isochoric heat capacity<br>
            </td>
          </tr>
          <tr>
            <td>v</td>
            <td>Specific volume<br>
            </td>
          </tr>
          <tr>
            <td>rho</td>
            <td>Density</td>
          </tr>
          <tr>
            <td>u</td>
            <td>Internal energy<br>
            </td>
          </tr>
          <tr>
            <td style="vertical-align: top;">x<br>
            </td>
            <td style="vertical-align: top;">Quality (when saturated)<br>
            </td>
          </tr>
        </tbody>
      </table>
      </td>
    </tr>
  </tbody>
</table>
<p>Also available are some 'boundary' functions which allow you to
calculate the
following:</p>
<table>
  <tbody>
    <tr>
      <td>Saturation pressure</td>
      <td>psat(T)</td>
    </tr>
    <tr>
      <td>Saturation temperature</td>
      <td>Tsat(p)</td>
    </tr>
  </tbody>
</table>
<p>You can also create <a href="properties/">property diagrams</a>
from the freesteam code for most of the above combinations of
steam properties.<br>
<span style="font-weight: bold;"></span></p>
<h3><span style="font-weight: bold;">1.2 Example usage</span></h3>
<span style="font-weight: bold;">Problem:</span><br>
<ol>
  <li>calculate the density of steam at 5 bar and 200&deg;C</li>
  <li>calculate the temperature if the steam from (1) is isentropically
compressed to 10 bar.</li>
  <li>calculate the pressure and temperature of steam at the same
specific volume as (2) but with the specific internal energy raised by
200 kJ/kg<br>
  </li>
</ol>
<span style="font-weight: bold;">Solution: </span>(included in
distribution as example.cpp, use '<code>make example</code>' to compile
it)<br>
<pre>#include "steamcalculator.h"<br>#include "solver.h"<br>#include "solver2.h"<br>#include &lt;iostream&gt;<br>#include &lt;iomanip&gt;<br>using namespace std;<br><br>int main(){<br>	try{<br>		SteamCalculator S1, S2, S3;<br><br>		// turn on display of units<br>		cerr.flags(ios_base::showbase);<br><br>		// initialise T1, p1, p2<br>		Temperature T1 = fromcelsius(200.0);<br>		Pressure p1 = 5.0 * bar;<br>		Pressure p2 = 10.0 * bar;<br><br>		// Part 1<br>		cerr &lt;&lt; endl &lt;&lt; "Part (1) - density at 10 bar, 200&deg;C" &lt;&lt; endl;<br>		cerr &lt;&lt; "p1 = " &lt;&lt; p1/bar &lt;&lt; " bar" &lt;&lt; endl;<br>		cerr &lt;&lt; "T1 = " &lt;&lt; T1;<br>		cerr &lt;&lt; " (" &lt;&lt; tocelsius(T1) &lt;&lt; "&deg;C)" &lt;&lt; endl;<br>		S1.set_pT(p1, T1);<br>		Density rho1 = S1.dens();<br>		cerr &lt;&lt; "rho1 = " &lt;&lt; rho1 &lt;&lt; endl;<br><br>		// Part 2<br>		cerr &lt;&lt; endl &lt;&lt; "Part (2) - isentropic compression to 10 bar" &lt;&lt; endl;<br>		SpecificEntropy s1 = S1.specentropy();<br>		cerr &lt;&lt; "s1 = " &lt;&lt; s1 &lt;&lt; endl;<br>		cerr &lt;&lt; "p2 = " &lt;&lt; p2/bar &lt;&lt; " bar" &lt;&lt; endl;<br>		Solver&lt;Pressure,SpecificEntropy,Temperature&gt; PS(p2, s1);<br>		S2 = PS.solve(0.0001 * kJ_kgK, 0.0001 * Kelvin);<br>		Temperature T2 = S2.temp();<br>		cerr &lt;&lt; "T2 = " &lt;&lt; T2;<br>		cerr &lt;&lt; " (" &lt;&lt; tocelsius(T2) &lt;&lt; "&deg;C)" &lt;&lt; endl;<br><br>		// part (3) - Finding p,T for v as above and u increased by 200 kJ_kg<br>		cerr &lt;&lt; endl &lt;&lt; "Part (3) - Finding p,T for v as above and u increased by 200 kJ_kg" &lt;&lt; endl;<br>		SpecificVolume v2 = S2.specvol();<br>		SpecificEnergy u2 = S2.specienergy();<br>		cerr &lt;&lt; "v2 = " &lt;&lt; v2 &lt;&lt; endl;<br>		cerr &lt;&lt; "u2 = " &lt;&lt; u2 &lt;&lt; endl;<br><br>		SpecificEnergy u3 = u2 + 200.0 * kJ_kg;<br>		cerr &lt;&lt; "u3 = " &lt;&lt; u3 &lt;&lt; endl;<br><br>		Solver2&lt;SpecificEnergy,SpecificVolume&gt; SUV;<br>		S3 = SUV.solve(u3,v2);<br>		Temperature T3 = S3.temp();<br>		Pressure p3 = S3.pres();<br>		cerr &lt;&lt; "p3 = " &lt;&lt; p3/bar &lt;&lt; " bar" &lt;&lt; endl;<br>		cerr &lt;&lt; "T3 = " &lt;&lt; T3;<br>		cerr &lt;&lt; " (" &lt;&lt; tocelsius(T3) &lt;&lt; "&deg;C)" &lt;&lt; endl;<br><br>		exit(0);<br><br>	}catch(Exception *E){<br>		cerr &lt;&lt; "Error: " &lt;&lt; E-&gt;what();<br>		exit(1);<br>	}<br>}<br></pre>
<p><span style="font-weight: bold;">Output:</span><br>
</p>
<pre>$ ./example<br><br>Part (1) - density at 10 bar, 200&deg;C<br>p1 = 5 bar<br>T1 = 473.15 K (200&deg;C)<br>rho1 = 2.35275 kg/m3<br><br>Part (2) - isentropic compression to 10 bar<br>s1 = 7061.07 J/kgK<br>p2 = 10 bar<br>T2 = 556.418 K (283.268&deg;C)<br><br>Part (3) - Finding p,T for v as above and u increased by 200 kJ_kg<br>v2 = 0.249638 m3/kg<br>u2 = 2.76613e+06 J/kg<br>u3 = 2.96613e+06 J/kg<br>p3 = 12.372 bar<br>T3 = 679.854 K (406.704&deg;C)<br></pre>
<h3>1.3 IAPWS-95 Scientific Formulation<br>
</h3>
<p>The code for calculating steam properties according to the IAPWS-95
equations, developed by Don Peterson, is include. In their present
form, the formulation allows you to calculate the
above properties but only in terms of <b>density</b> and <b>temperature</b>.</p>
<p>IAWPS-95 is said to be higher accuracy but consumes greater
computing time. This may depend on the formulation of your problem
however - some problems require quite a lot of iterative root-finding
when using IAPWS-IF97.<br>
</p>
<h2><a name="quickstart"></a>2. Quick start</h2>
<ol>
  <li>You should be able to compile on any GNU GCC system: we have
tested on cygwin and Red Hat, others should be ok too.<br>
  </li>
  <li>Get the package <code>freesteam-0.4.tar.bz2</code> from <a
 href="http://sf.net/projects/freesteam">http://sf.net/projects/freesteam</a>.
    <br>
  </li>
  <li>Get and install the package <code>cppunit-1.10.2.tar.gz</code>.
You can get this at <a href="http://sourceforge.net/projects/cppunit">http://sourceforge.net/projects/cppunit</a>.
It is easily built from source via '<code>./configure</code>' and '<code>make
install</code>' using GCC. It is useful to copy the file '<code>cppunit-config</code>'
to your path, for example to <code>/usr/local/bin</code>. You can also
just use a binary version of CppUnit but you might need to check the
location for the file <code>cppunit-config</code> and modify the
freesteam <code>Makefile</code> accordingly.<br>
  </li>
  <li><code>tar jxvf freesteam-0.4.tar.bz2</code> and <code>cd
freesteam-0.4</code>.<br>
  </li>
  <li>Edit the <code>Makefile </code>if necessary, to reflect your
C++ compiler command and CppUnit library locations.</li>
  <li>On a Linux system, you'll need to type <code>chmod +x
config.guess</code><br>
  </li>
  <li>Build/test the package:</li>
  <ol>
    <li><code>make test</code> to test the package</li>
    <li><code>make example</code> to make the example program, then
type '<code>./example</code>'<br>
    </li>
  </ol>
</ol>
<h2><a name="othercompilers"></a>3. Other compilers</h2>
freesteam is known to compile and run under the following system setups:<br>
<ul>
  <li>GCC 3.3.3 (cygwin special) </li>
  <li>GCC 3.4 (Linx 2.6 Fedora Core 3 on AMD 64 Opteron in <a
 href="http://sf.net/docs/E02/">cf.net compile farm</a>).</li>
  <li>GCC 3.3.2 on Fedora Cora 1.<br>
  </li>
  <li>GCC 3.3.3 20040412 (Red Hat Linux 3.3.3-7)</li>
  <li>GCC 3.4.2 (mingw-special)<br>
  </li>
</ul>
<p>We hope to port freesteam to VC++ and the Intel C++ compiler. It
might even work with
those compilers
already, although it is anticipated that the code will not work in
present form
with VC++
6.0 because of the complexity of the template methods used
in the units-of-measurement class, 'units.h'.</p>
<h2><a name="license"></a>4. License</h2>
<p><span style="font-weight: bold;">freesteam </span>is released under
the GNU GPL, which is available online
at
<a href="http://www.gnu.org/copyleft/gpl.html">http://www.gnu.org/copyleft/gpl.html</a>.
It is also distributed with the source code distribution for freesteam,
as LICENSE.txt.
</p>
<p>Note that under the GPL, you can not dynamically or statically
link to the freesteam library from your own code unless your code is
also licensed under the GPL.<br>
</p>
<h2><a name="bugs"></a>6. Bugs</h2>
<p>For a complete up-to-date list of bugs and feature requests, visit
the Mantis <a href="http://pye.dyndns.org/mantis/">bug tracker</a> for
the project. We are not using the SourceForge 'tracker' at this stage.<br>
</p>
<p>Some of the bugs current at the time of writing:<br>
</p>
<ul>
  <li>There is a problem with <span style="font-weight: bold;">whichStateStr
    </span>which we need to fix.</li>
  <li>The <span style="font-weight: bold;">one-way solver</span> has
some problems in certain situations. For example, solving for a desired
value of specific volume along the saturation curve will give errors at
low temperatures, because of the 'density anomaly'. See <a
 href="http://pye.dyndns.org/mantis/bug_view_page.php?bug_id=0000060">this
bug</a>.</li>
  <li>The current implementation uses an IAPWS Supplementary Release
correlation for the density of <span style="font-weight: bold;">saturated
steam </span>when the temperature exceeds that of the region 1/region
3 boundary. This is not in keeping with the 'pure' IAPWS-IF97 Release,
and gives slightly different values for specific entropy at the
critical point as a result. See <a
 href="http://pye.dyndns.org/mantis/bug_view_page.php?bug_id=0000059">this
bug</a> for more information.</li>
</ul>
<h2><a name="futuredirections"></a>5. Future directions</h2>
<ul>
  <li>Use of the <span style="font-weight: bold;">hook library </span>which
allows the <a href="http://www.rps.eng.br/emso.html">EMSO</a> system
modelling
program to use the freesteam library requires a non-GPL license, which
we are currently preparing.</li>
  <li>Initial work is being done on an <span style="font-weight: bold;">Excel
add-in</span> version using the <a href="http://xlw.sourceforge.net/">XLW</a>
wrapper classes.<br>
  </li>
  <li>Preliminary work on another <span style="font-weight: bold;">hook
library</span> to integrate freesteam with the <a
 href="http://www.fchart.com/ees/ees.shtml">EES</a>
equation solver has also been performed; this will likewise require
some special license agreement.<br>
  </li>
  <li>Saturation properties for region 3 are currently implemented used
the rho_sat(T) correlations from the IAPWS <a
 href="http://www.iapws.org/relguide/supsat.pdf">supsat</a> release.
Need to change this to use an iterative solver based on the region 3
correlations, since the current
equations give slight inconsistencies with pure IAPWS data for
pressures above about 16 MPa.<br>
  </li>
  <li><span style="font-weight: bold;">Backwards correlations</span>
will massively reduce the amount of two-way iteration required when
finding the state for unsual combinations of variables.</li>
  <li>Perhaps <span style="font-weight: bold;">develop a GUI </span>version
using <a href="http://www.trolltech.com/">Qt</a> or <a
 href="http://www.wxwindows.org/">wxWindows</a> or similar. I have no
experience with these so
let me know if you'd like to help there.<br>
  </li>
  <li>We plan to put in place hooks that will allow freesteam to be
used from within <a style="font-weight: bold;"
 href="http://www.python.org/">Python</a>,
possibly using <a href="http://www.swig.org/">SWIG</a>. Ditto.<br>
  </li>
  <li>An <span style="font-weight: bold;">interactive </span>command-line
steam tables program for easy demonstration/interactive testing.</li>
  <li>An add-in version for OpenOffice would be good.<br>
  </li>
  <li>There have been contributions for the <span
 style="font-weight: bold;">reverse equations in Region 2</span> - I
haven't incorporated these into the source code yet though.</li>
  <li>There have been contributions for some <span
 style="font-weight: bold;">r</span><span style="font-weight: bold;">everse
equations in Region 3</span> - I
haven't incorporated these into the source code yet though.</li>
  <li>The <span style="font-weight: bold;">compiled size </span>of
the code is quite large at the moment, probably because of the large
amount of debugging code and GDB symbols. However some of this might be
to do with bad use of C++ templates, need to take a look at that.</li>
  <li><span style="font-weight: bold;">Region 5</span> has not yet been
implemented at all, but it's a low priority. Let us know if you'd need
it.</li>
  <li>We'd like to extend the <span style="font-weight: bold;">phase
diagrams</span> code so that lines of constant properties (pressure,
temperature, entropy, enthalpy, etc) can be shown as well as the
saturation/region lines.<br>
  </li>
</ul>
<h2><a name="links"></a>6. Links</h2>
<dl>
  <dt>IAPWS-IF97 steam tables </dt>
  <dd> <a href="http://www.iapws.org/">http://www.iapws.org/</a><br>
    <a href="http://www.iapws.org/relguide/visc.pdf">http://www.iapws.org/relguide/visc.pdf</a><br>
    <a href="http://www.iapws.org/relguide/thcond.pdf">http://www.iapws.org/relguide/thcond.pdf</a><br>
    <a href="http://www.iapws.org/relguide/IF97.pdf">http://www.iapws.org/relguide/IF97.pdf</a><br>
    <a href="http://www.iapws.org/relguide/supsat.pdf">http://www.iapws.org/relguide/supsat.pdf</a><br>
    <a href="http://dx.doi.org/10.1115/1.483186">IAPWS paper from
Journal of Engineering for Gas Turbines and Power</a><br>
    <br>
  </dd>
  <dt>Bernhard Spang's summary of IF97 </dt>
  <dd><a href="http://www.cheresources.com/iapwsif97.shtml">http://www.cheresources.com/iapwsif97.shtml<br>
    <br>
    </a> </dd>
  <dt>IAPWS-95 scientific formulation </dt>
  <dd><a href="http://www.iapws.org/relguide/IAPWS95.pdf">http://www.iapws.org/relguide/IAPWS95.pdf<br>
    <br>
    </a> </dd>
  <dt>CppUnit </dt>
  <dd><a href="http://cppunit.sourceforge.net/cgi-bin/moin.cgi">http://cppunit.sourceforge.net/cgi-bin/moin.cgi</a>
  </dd>
  <dd><a href="http://sourceforge.net/projects/cppunit">http://sourceforge.net/projects/cppunit<br>
    <br>
    </a> </dd>
  <dt>Rettig's units-of-measurement stuff </dt>
  <dd><a
 href="http://www.embedded.com/shared/printableArticle.jhtml?articleID=9900094">http://www.embedded.com/shared/printableArticle.jhtml?articleID=9900094</a>
  </dd>
  <dd><a href="http://www.embedded.com/code/2001code.htm">http://www.embedded.com/code/2001code.htm<br>
    <br>
    </a> </dd>
  <dt>Brent solver </dt>
  <dd><a href="http://www.library.cornell.edu/nr/bookcpdf.html">http://www.library.cornell.edu/nr/bookcpdf.html<br>
    <br>
    </a></dd>
  <dt><br>
  </dt>
  <dt>Free (in the OS sense) IAPWS calculators</dt>
  <dd>Bernhard Spang's IF-97 Excel plug-in <a
 href="http://www.cheresources.com/iapwsif97.shtml">http://www.cheresources.com/iapwsif97.shtml</a>
(Visual Basic)<br>
  </dd>
  <dd>Leon Kos, University of Ljubljana: <a
 href="http://www.lecad.uni-lj.si/%7Eleon/research/iapws-if97/IAPWS97.html">http://www.lecad.uni-lj.si/~leon/research/iapws-if97/IAPWS97.html</a>
(Javascript)<br>
  </dd>
  <dd><br>
  </dd>

  <dt>Commercial IAPWS calculators</dt>

  <dd>ChemicaLogic, USA: <span style="font-style: italic;">SteamTab SDK</span><br></dd>
  <dd>University of Applied Sciences Zittau/G&ouml;rlitz, Germany: <i>FluidEXL</i></dd>
  <dd>Edward Throm: <i>ASTEM97</i></dd>
  <dd>G&amp;P Engineering: <i>StmProps</i></dd>
  <dd>Wagner, Wolfgang, Kretzschmar, Hans-Joachim, <i>International Steam Tables - The
Industrial Standard IAPWS-IF97 for the Thermodynamic Properties and
Supplemetary Equations for Other Properties: Tables, Algorithms,
Diagrams, Software</i>, 2nd ed., 2006 (!),
Springer, Germany.</dd>
  <dd>MegaWatSoft: <i>Steam97</i></dd>
  <dd>ARC Engineering plc: <i>arcSteam</i></dd>
  <dd>GP International: <i>GPSteam</i> ("ASME
97" formulation)</dd>
  <dd>Rudjer Boskovic Institute: <i>ISANO
Iapwsif97</i></dd>

  <dt>Other steam calculators (Non IF97)</dt>
  <dd>WinSim <a href="http://www.winsim.com/steam/steam.html">STEAM67</a>
(ASME 1967 Steam Tables, with source code in C)<br>
PARVO95, PARVO67 <a href="http://fortraner.narod.ru/">http://fortraner.narod.ru/</a>
(Fortran) In Russian, not tested.</dd>
  <dt><br>
  </dt>
  <dt>Some useful steam table charts</dt>
  <dd>Copied from Reynolds' <span style="font-style: italic;">Thermodynamic
Properties in SI</span>, Stanford Univ, 1979:<br>
    <a
 href="http://www.mhtl.uwaterloo.ca/old/courses/ece309/notes/properties/charts/hs.html">h-s</a>,
    <a
 href="http://www.mhtl.uwaterloo.ca/old/courses/ece309/notes/properties/charts/ph.html">p-h</a>,
    <a
 href="http://www.mhtl.uwaterloo.ca/old/courses/ece309/notes/properties/charts/ts.html">T-s</a>,
    <a
 href="http://www.mhtl.uwaterloo.ca/old/courses/ece309/notes/properties/charts/vu.html">v-u</a></dd>
  <dt><br>
  </dt>
  <dt>Generally of interest about water and steam:</dt>
  <dd> <a href="http://www.lsbu.ac.uk/water/index.html">http://www.lsbu.ac.uk/water/index.html</a>
  </dd>
</dl>
<h2><a name="contactdetails"></a>7. Contact details</h2>
<p>You can contact us either via the email form on the
<a href="http://sourceforge.net/projects/freesteam/">project page on
sourceforge</a>, or email directly to
<script><!--

// 3m41L harvester protection

m='hn.pye@stud';
l='u.au';
i='nsw.ed';
e='jo';
a='ent.u';

document.write('<a href="mailto:'+e+m+a+i+l+'">'+e+m+a+i+l+'</a>');

//--></script>
<noscript>john DOT pye AT-SIGN student DOT unsw FULLSTOP edu POINT au
(spam protection)</noscript>
</p>
<p>You can also take a look at my homepage about the <a
 href="http://pye.dyndns.org/">CLFR</a> if you want...</p>

</td></tr></table>
<hr>
<address>
John Pye<br/>
Last modified: <?php print(date("j F Y",$t)) ?>
</address>
</body>
</html>
