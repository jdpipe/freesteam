<?php require("header.php") ?>

<h1><a name="quickstart"></a>How to compile freesteam</h1>

<?php 
require("nav.php");

show_nav(NULL, "How to compile");

?>

<h2>Building version <?php print($latestversion)?></h2>

<p><font color=red><b><i>Outdated!</i></b></font> These instructions refer to old versions of <b>freesteam</b> up to version 0.5.5. Please see the <a href="scons.php">SCons build</a> page for information on current versions.</p>

<p>You should be able to compile on any GNU GCC system: we have
tested on MinGW, Cygwin and Linux (Red Hat & Fedora), others should be ok too.</p>

<ol>
  <li>Get the package <code>freesteam-<?php print($latestversion)?>.tar.bz2</code> from <a
 href="http://sf.net/projects/freesteam">http://sf.net/projects/freesteam</a>.</li>
 
  <li>Get and install the package <code>cppunit-1.10.2.tar.gz</code>. You can get this at <a href="http://sourceforge.net/projects/cppunit">http://sourceforge.net/projects/cppunit</a>. It is easily built from source via '<code>./configure</code>' and '<code>make install</code>' using GCC. It is useful to copy the file '<code>cppunit-config</code>' to your path, for example to <code>/usr/local/bin</code>. You can also just use a binary version of CppUnit but you might need to check the location for the file <code>cppunit-config</code> and modify the freesteam <code>Makefile</code> accordingly. </li>
  
  <li><code>tar jxvf freesteam-<?php print($latestversion)?>.tar.bz2</code> and <code>cd freesteam-<?php print($latestversion)?></code>.</li>

  <li>Edit the <code>Makefile </code>if necessary, to reflect your C++ compiler command and CppUnit library locations.</li>

  <li>On a Linux system, you'll need to type <code>chmod +x config.guess</code></li>

  <li>Build/test the package:</li>

  <ol>
    <li><code>make test</code> to test the package</li>
    <li><code>make example</code> to make the example program, then type '<code>./example</code>'</li>
  </ol>
</ol>

<h2><a name="othercompilers"></a>About specific compilers</h2>

Version 0.5.5 is known to compile and run under the following system setups:<br>

<ul>
  <li>GCC 4.02 on Fedora Core 4 with SCons
  <li>GCC 3.4.2 on MinGW/MSYS with Scons
  </li>
</ul>

It has previously been built on the following, although they are not currently tested:

<ul>
  <li>GCC 3.3.3 (cygwin special) (was actively developped on this platform)</li>
  <li>GCC 3.4.3 on Fedora Core 3 on a P4 (was actively developped on this platform)</li>
  <li>GCC 3.4.2 (mingw-special) (was actively developped on this platform)</li>
  <li>GCC 3.4 (Linux 2.6 Fedora Core 3 on AMD 64 Opteron in <a href="http://sf.net/docs/E02/">cf.net compile farm</a>)</li>
  <li>GCC 3.3.2 on Fedora Cora 1.</li>
  <li>GCC 3.3.3 20040412 (Red Hat Linux 3.3.3-7)</li>
  </li>
</ul>

freesteam does <b>NOT</b> compile on the following systems. This doesn't mean I'm not sad about that. If you can help to sort out the issues with these compilers, that would be much appreciated.
<ul>
  <li>Visual C++ Toolkit 2003 (problems with the <code>units.h</code> header; hoping to resolve)</li>
  <li>Borland C Builder 5.5 (problems with templates. Need help with someone familiar with this compiler)</li>
</ul>

<p>I haven't tested with Intel compiler (I'm still downloading the 200 MB as I write). Also VC++/.NET 2005 (I will download the ISO image one of these days).</p>

<h2>Building with SCons</h2>

<p>The CVS version of freesteam builds with SCons. See <a href="scons.php">Building with SCons</a> for more information.</p>

<?php require("footer.php") ?>
