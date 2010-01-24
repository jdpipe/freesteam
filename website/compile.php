<?php require("header.php") ?>

<h1><a name="quickstart"></a>How to compile freesteam</h1>

<?php 
require("nav.php");

show_nav(NULL, "How to compile");

?>

<h2>Building version <?php print($latestversion)?></h2>

<p>The following instructions have been tested with version 2.0 on Windows XP (32 bit), Ubuntu Karmic, and Mac OS X 10.5.</p>

<p>To build <b>freesteam</b> you will need to have Python, SCons, and SWIG and the GCC compiler installed on your system. On Windows, you should use the MinGW toolset, together with MSYS for the command line (and make sure you have got c:\Python26 and c:\Python26\Scripts and the location of SWIG present in your PATH).

<ol>
<li>Download the source code tarball from the freesteam files page on sourceforge.net.
<li>Unpack the tarball using <tt>freesteam-2.0.tar.bz2</tt>
<li>Enter the source directory using <tt>cd freesteam-2.0</tt>
<li>Build the package using <tt>scons</tt>
</ol>

<p>On Windows, the binary installer can be generated if you have got NSIS installed on your system. Use <tt>scons installer</tt> to generate it. On Mac OS X, the installer package can be generated using PackageMaker. Follow the instructions in the <tt>macosx</tt> subdirectory.

<p>On Linux and Mac, you can install the package to /usr/local by typing <tt>scons install</tt>. If you want to change installed file locations, check the scons command-line parameters using <tt>scons -h</tt>.

<p>To test the resulting library, type <tt>scons test &amp;&amp; ./test</tt>. A range of test results will be reported back via the commandline.

<p>At this stage, freesteam has not been tested with Microsoft, Intel, Borland or any otherr compilers. If you would like to try, and report the results, please let us know.

<?php require("footer.php") ?>
