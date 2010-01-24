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
international-standard IAPWS-IF97 steam
tables from the <a href="http://www.iapws.org">International Association for the Properties of Water
and
Steam</a> (IAPWS). <b>freesteam</b> lets you compute water and steam properties for a wide range of pressures and temperatures: you can specify the state of the steam in terms of a variety of combinations of 'known' properties, then freesteam will solve and allow you to query to find the values of the 'unknown' properties.</p>

<p>The library includes bindings for the <a href="http://www.python.org">Python</a> language, as well as bindings to allow it to be used from the <a href="http://ascend.cheme.cmu.edu/">ASCEND modelling environment</a>. For Windows and Mac and Ubuntu users, convenient binary packages are provided.

<p><font color=red><i><b>New:&nbsp;</b></i></font>Freesteam 2.0 released! This is a complete rewrite of freesteam in the C language, for faster, smaller binaries and better compatibility for accessing the DLL/SO from different C++ compilers.

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
(spam protection)</noscript>. The project is currently under active development (as of Jan 2010), and contributions are most welcome!</p>

<h2><a name="whatcanidowithit"></a>1. What can I do with it?</h2>

Using freesteam, you can set the state of steam using various pairs of know
properties. The following input pairs are implemented:

<ul>
<li>(p,T)
<li>(p,h)
<li>(p,s)
<li>(p,v)
<li>(T,s)
<li>(T,x)
</ul>

<p>In C, these states are set using syntax like <tt>SteamState S = freesteam_set_ph(p,h)</tt>. In Python, the syntax is <tt>S = steam_ph(p,h)</tt>. The routines return a 'SteamState' object that can then be queried for any output properties you wish to know, including

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
          <tr>
            <td style="vertical-align: top;">mu</td>
            <td style="vertical-align: top;">Dynamic viscosity</td>
          </tr>
          <tr>
            <td>w</td>
            <td>Speed of sound<br>
            </td>
          </tr>
        </tbody>
      </table>

<p>The above properties are accessed using functions in C, like <tt>double p = freesteam_p(S)</tt>, and using properties in Python, like <tt>p = S.p</tt>.

<p>Calculation of surface tension is not currently implemented in version 2.0

<p>Saturation curves and the boundary curves used internally by IAPWS-IF97 can
also be accessed, if required.

<p>Freesteam is now able (version 2.x)
to auto-generate a <a href="http://downloads.sourceforge.net/project/freesteam/freesteam/2.0/freesteam-steam-tables.pdf">complete set of steam tables</a> suitable for student use, as well as a range of property diagrams, using Python code. Some <a href="properties/">property diagrams</a> produced using freesteam-0.8.1 are also available.

<p><b>Please check out the <a href="example.php">example code</a></b></p>

<h2><a name="license"></a>2. License</h2>

<p><span style="font-weight: bold;">freesteam </span>is released under
the GNU GPL, which is available online
at
<a href="http://www.gnu.org/copyleft/gpl.html">http://www.gnu.org/copyleft/gpl.html</a>.
It is also distributed with the source code distribution for freesteam,
as LICENSE.txt.
</p>
<p>Note that under the GPL, you can sell or otherwise distribute any code that
dynamically or statically links to the freesteam library unless your code is 
also licensed under the GPL.<br>
</p>


<h2><a name="bugs"></a>3. Bugs</h2>

<p>For a complete up-to-date list of bugs and feature requests, visit
the Mantis <a href="http://ascendbugs.cheme.cmu.edu/set_project.php?project_id=3">bug tracker</a> for
the project. We are not using the SourceForge 'tracker' at this stage.</p>

<p>Some of the bugs current at the time of writing:</p>
<ul>
  <li>Mac binary installer does not include the sample Python scripts
  <li>There are still some small errors in the (p,h) calculation, of the order of 0.1%.
  <li>Our documentation needs to clearly indicate which IAPWS releases are used to provide different functionality.
</ul>

<h2><a name="futuredirections"></a>4. Future directions</h2>

<p>Some of the things waiting 'on the drawing board' are as follows. If you can help, please let me know!</p>

<h3>High priority</h3>

<ul>
  <li><b>Updated documentation</b>!
  <li>Some more <b>example code</b>.    
</ul>

<h3>DIY Ideas 4U</h3>

<ul>
  <li>We would like to expose freesteam functionality to Excel via the <a href="http://xlw.sourceforge.net/">XLW</a> wrapper classes.</li>
  
  <li><span style="font-weight: bold;">Backwards correlations</span> should be iteratively refined by a subsequent Newton solver stage, to reduce errors associated with their use.</li>

  <li>An add-in for OpenOffice would be good. Have received some partially-working source code for this from Allan Curtis</li>

  <li><span style="font-weight: bold;">Region 5</span> has not yet been implemented at all, but it's a low priority. Let us know if you'd need it.</li>

  <li><span style="font-weight: bold;">Develop a cross-platform GUI </span>version using <a href="http://www.gtk.org/">GTK+</a> or similar.</li>
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

  <dd>ChemicaLogic, USA: <span style="font-style: italic;"><a href="http://www.chemicalogic.com" rel=nofollow>SteamTab SDK</a></span><br></dd>
  <dd>University of Applied Sciences Zittau/G&ouml;rlitz, Germany: <i>FluidEXL</i></dd>
  <dd>Edward Throm: <i>ASTEM97</i></dd>
  <dd>G&amp;P Engineering: <i>StmProps</i></dd>
  <dd>Zittau/Goerlitz University of Applied Sciences: <i><a href="http://www.thermodynamics-zittau.de/" rel=nofollow>LibIF97</a></i></dd>
  <dd>Wagner, Wolfgang, Kretzschmar, Hans-Joachim, <i>International Steam Tables - The
Industrial Standard IAPWS-IF97 for the Thermodynamic Properties and
Supplemetary Equations for Other Properties: Tables, Algorithms,
Diagrams, Software</i>, 2nd ed., 2006 (!),
Springer, Germany.</dd>
  <dd>MegaWatSoft: <i>Steam97</i></dd>
  <dd>ARC Engineering plc: <i>arcSteam</i></dd>
  <dd>GP International: <i>GPSteam</i> ("ASME
97" formulation)</dd>
  <dd>TechWare Engineering: <i><a href="http://www.techwareeng.com/ProdWS.html" rel=nofollow>WinSteam</a></i></dd>
  <dd><i><a href="http://www.wsp.ru" rel=nofollow>WaterSteamPro</a></i></dd>
  <dd>Rudjer Boskovic Institute: <i>ISANO
Iapwsif97</i><br><br></dd>

  <dt>Other steam calculators (Non IF97)</dt>
  <dd>WinSim <a href="http://www.winsim.com/steam/steam.html">STEAM67</a>
(ASME 1967 Steam Tables, with source code in C)<br>
PARVO95, PARVO67 <a href="http://fortraner.narod.ru/">http://fortraner.narod.ru/</a>
(Fortran) In Russian, not tested.</dd>
  <dd>Taftan <tt><a href="http://www.taftan.com" rel=nofollow>Thermo Utilities</a></tt> (IFC '67 correlation)</dd>
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
