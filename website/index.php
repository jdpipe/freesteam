<?php require("header.php"); ?>

<h1>freesteam<br></h1>

<?php 
require("nav.php");

$this_page = array(
	new Link("What can I do with it?","#whatcanidowithit")
	,new Link("License","#license")
	,new Link("Bugs","#bugs")
	,new Link("Future directions","#futuredirections")
	,new Link("Related sites","#links")
);

show_nav($this_page);

?>

<h2>Introduction</h2>

<p><b>freesteam</b> is an open source implementation of
international-standard steam
tables from the <a href="http://www.iapws.org">International Association for the Properties of Water
and
Steam</a> (IAPWS). <b>freesteam</b> lets you compute water and steam properties for a wide range of pressures and temperatures: you can specify the state of the steam in terms of a variety of combinations of 'known' properties, then freesteam will solve and allow you to query to find the values of the 'unknown' properties.</p>

<ul>
  <li>IAPWS-IF97 Industrial formulation (high speed, but complex code) </li>
  <li>IAPWS-95 Scientific formulation (low speed, but simple code)</li>
</ul>

<p>The library includes bindings for the <a href="http://www.python.org">Python</a> language, as well as bindings to allow it to be used from the <a href="http://ascend.cheme.cmu.edu/">ASCEND modelling environment</a>. For Windows users, a convenient .exe installer is provided.

<p><font color=red><i><b>New:&nbsp;</b></i></font>The 0.8 release contains a rewrite of the thermal conductivity correlation, and fixes a bug in the calculation of densities in the two-phase Region 4.</p>

<p><font color=red><i><b>UPDATE:&nbsp;</b></i></font>(Feb 2009) Work is currently going on towards a 0.9 release of freesteam in which the main code will be rewritten in plain C. This will make freesteam easier for most people to use (e.g. Visual Basic problems with C++ ABI will go away) and will make it easier for people to contribute (the current C++ templates are difficult to understand and to maintain).</p>

<p>There is a <a href="https://lists.sourceforge.net/lists/listinfo/freesteam-users">mailing list for freesteam users</a>, which is the preferred support mechansim (it ensures that FAQs are captured for others to re-use). Alternatively, you can email the developer directly on <script><!--

// 3m41L harvester protection

m='n.pye@an';
l='u.au';
i='d';
e='joh';
a='u.e';

document.write('<a href="mailto:'+e+m+a+i+l+'">'+e+m+a+i+l+'</a>');

//--></script>
<noscript><a href="mailto:john.pye@anu.removethisrubbish.edu.au">john.pye@anu.removethisrubbish.edu.au</a>
(spam protection)</noscript>. The project is under active development as of Feb 2009, and contributions are most welcome!</p>

<h2><a name="whatcanidowithit"></a>1. What can I do with it?</h2>


<h3>1.1 IAPWS-IF97 Industrial Formulation<br></h3>

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

		<ul>
		<li>(p,T) for subcooled/superheated steam.
		<li>(rho,T) for supercritical steam.
		<li>(T,x) for saturated steam
		<li>(p,T,x) anywhere
		<li>(p) plus one of (s,h,u,v,cp,cv,rho)
		<li>(T) plus one of (s,h,u,v,cp,cv,rho)
		</ul>

      <p>By two-way iteration, state can furthermore be solved for:<br>
(u,v) (p,h) (p,u) (p,s) (T,s) (T,h)<br>
      </p>

      <p>Alternatively, you can specify:</p>
      <ul>
		<li>saturated water at T
		<li>saturated steam at T<br>
      </ul>

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
          <tr>
            <td style="vertical-align: top;">k</td>
            <td style="vertical-align: top;">Thermal conductivity</td>
          </tr>
        </tbody>
      </table>
      <p>A correlation for viscosity is also provided but this has not been rigorously validated.

<p><font color=red><i><b>New:&nbsp;</b></i></font>Calculation of surface tension is now supported (from version 0.6.2)</p>

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
steam properties.</p>

<p>You can also access freesteam from the EMSO simulation. More plug-ins for other systems are proposed: python hooks, MATLAB, Excel and OpenOffice/Gnumeric plugins. See <a href="#futuredirections">future directions</a> (below) for more info.</p>

<p><b>Please check out the <a href="example.php">example code</a></b></p>

<h3>1.2 IAPWS-95 Scientific Formulation<br></h3>

<p>The code for calculating steam properties according to the IAPWS-95 equations, developed by Don Peterson, is included. In their present form, the formulation allows you to calculate the above properties but only in terms of <b>density</b> and <b>temperature</b>.</p> <p>IAWPS-95 is said to be higher accuracy but consumes greater computing time. This may depend on the formulation of your problem however - some problems require quite a lot of iterative root-finding when using IAPWS-IF97.</p>

<h3>1.3 Goal-seeking for steam state</h3>

<p><b>freesteam</b> offers more flexibility than many other steam tables packages in its ability to find a particular steam state in terms of the combination of steam properties you desire. With other packages, you may only be given the properties correlated in terms of (<i>p</i>,<i>T</i>) or (<i>&rho;</i>,<i>T</i>), etc. This would then require you to link to or write your own root-finding code before you could use the correlation to solve your problem.</p>

<p>At present, freesteam permits the robust solution of steam state in terms of the following sets of properties: (p,T) 
(u,v) (p,h) (p,u) (p,s) (T,s) (T,h) (p,v) (p,cp) (p,cv) (p,rho) (T,u) (T,v) (T,cp) (T,cv) (T,rho). The architecture for adding new property combinations is modular, if you need another combination, let me know.</p>

<p>There is a test-suite provide with the source code that allows you to verify the above iterative solutions against the results from forward computation.</p>

<h2><a name="license"></a>2. License</h2>

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


<h2><a name="bugs"></a>3. Bugs</h2>

<p>For a complete up-to-date list of bugs and feature requests, visit
the Mantis <a href="http://ascendbugs.cheme.cmu.edu/set_project.php?project_id=3">bug tracker</a> for
the project. We are not using the SourceForge 'tracker' at this stage.</p>

<p>Some of the bugs current at the time of writing:</p>
<ul>
  <li>There is a problem with <span style="font-weight: bold;">whichStateStr</span> which we need to fix.</li>

  <li>The <span style="font-weight: bold;">one-way solver</span> has some problems in certain situations. For example, solving for a desired value of specific volume along the saturation curve will give errors at low temperatures, because of the 'density anomaly'. See <a href="http://ascendbugs.cheme.cmu.edu/bug_view_page.php?bug_id=0000060">this bug</a>.</li>

  <li>The current implementation uses an IAPWS Supplementary Release correlation for the density of <span style="font-weight: bold;">saturated steam </span>when the temperature exceeds that of the region 1/region 3 boundary. This is not in keeping with the 'pure' IAPWS-IF97 Release, and gives slightly different values for specific entropy at the critical point as a result. See <a href="http://ascendbugs.cheme.cmu.edu/bug_view_page.php?bug_id=0000059">this bug</a> for more information.</li>
</ul>


<h2><a name="futuredirections"></a>4. Future directions</h2>

<p>Some of the things waiting 'on the drawing board' are as follows. If you can help, please let me know!</p>

<h3>High priority</h3>

<ul>
  <li><b>Write some documentation</b>!
  
  <li>Add a directory with some more <b>example code</b>.
  
  <li>Support for Visual C++ 2005 compiler. Seems that this may require adoption of the units-of-measurement C++ template classes from Boost.org, in place of those currently in use from Embedded.com, which are broken at present.
    
  <li><a href="http://www.potto.org">Genick Bar-Meir</a> has started working on a <span style="font-weight:bold">Qt-based GUI</span> for freesteam. Stay tuned!</li>

</ul>

<h3>DIY Ideas 4U</h3>

<ul>
  <li>Initial work has being done on an <span style="font-weight: bold;">Excel add-in</span> version using the <a href="http://xlw.sourceforge.net/">XLW</a> wrapper classes. There are some build issues with XLW to be resolved.</li>

  <li>Preliminary work on another <span style="font-weight: bold;">hook library</span> to integrate freesteam with the <a href="http://www.fchart.com/ees/ees.shtml">EES</a> equation solver (<a href="ees.pdf#page=211">manual</a> here)has also been performed; this will likewise require some special license agreement. At present, EES provides only the IAPWS-95 'scientific and general use' correlation.</li>

  <li>Integration of the IAPWS-95 correlation into <a href="http://www.ginac.de/">GiNaC</a>. This work is largely complete (the code's not on the net yet though): it needs to be incorporated into the freesteam distribution.
  
  <li>Saturation properties for region 3 are currently implemented used the rho_sat(T) correlations from the IAPWS <a href="http://www.iapws.org/relguide/supsat.pdf">supsat</a> release. Need to change this to use an iterative solver based on the region 3 correlations, since the current equations give slight inconsistencies with pure IAPWS data for pressures above about 16 MPa.</li>
  
  <li><span style="font-weight: bold;">Backwards correlations</span> will massively reduce the amount of two-way iteration required when finding the state for unsual combinations of variables.</li>
  
  <li>An <span style="font-weight: bold;">interactive </span>command-line
steam tables program for easy demonstration/interactive testing.</li>

  <li>An add-in version for OpenOffice would be good. Have received some partially-working source code for this from Allan Curtis</li>

  <li>There have been contributions for the <span style="font-weight: bold;">reverse equations in Region 2</span> - I haven't incorporated these into the source code yet though.</li>
  
  <li>There have been contributions from Allan Curtis for some <span style="font-weight: bold;">r</span><span style="font-weight: bold;">everse equations in Region 3</span> - I haven't incorporated these into the source code yet though.</li>
  
  <li>The <span style="font-weight: bold;">compiled size </span>of the code is quite large at the moment, probably because of the large amount of debugging code and GDB symbols. However some of this might be to do with bad use of C++ templates, need to take a look at that.</li>
  
  <li><span style="font-weight: bold;">Region 5</span> has not yet been implemented at all, but it's a low priority. Let us know if you'd need it.</li>
  
  <li>We'd like to extend the <span style="font-weight: bold;">phase diagrams</span> code so that lines of constant properties (pressure, temperature, entropy, enthalpy, etc) can be shown as well as the saturation/region lines.</li>

  <li><span style="font-weight: bold;">Develop a GUI </span>version using <a href="http://www.gtk.org/">GTK+</a> or similar.</li>
</ul>


<h2><a name="links"></a>5. Related sites</h2>

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
   
  <dt>Surface tension</dt>
  <dd><a href="http://www.iapws.org/relguide/surf.pdf">http://www.iapws.org/relguide/surf.pdf</a><br><br></dd>
    
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
  
  <dt>Free (in the OS sense) IAPWS calculators</dt>
  <dd><a href="http://www.x-eng.com/">X Steam</a> (for Excel, OpenOffice and Matlab, licensing unclear)<br>

  Bernhard Spang's IF-97 Excel plug-in <a
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
Iapwsif97</i><br><br></dd>

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

<?php
require("footer.php")
?>