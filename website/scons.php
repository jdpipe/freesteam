<?php require("header.php") ?>

<h1><a name="quickscons"></a>Building freesteam with Scons</h1>

<?php 
require("nav.php");

show_nav(NULL, "Building with SCons");

?>

<h2>How to compile</h2>

<p>Starting with version 0.6, freesteam is built using <a href="http://www.scons.org">SCons</a>. SCons is an excellent and fairly new build tool that boasts much better cross-platform support that Autotools. If you're not familiar with SCons, I'd recommend you give it a careful look.</p>

<p>Building with SCons requires that you have <a href="http://www.python.org">Python</a> (version 2.4) and <b>SCons 0.96.92</b> (or newer, presumably) installed. These can both be installed very easily on Windows using .exe installers from the respective websites.</p>

<h3>Linux</h3>

<ol>
<li><code>tar jxvf <i>/path/to/your</i>/freesteam-<?php print($latestversion)?>.tar.bz2</code></li>
<li><code>cd freesteam-<?php print($latestversion)?></code></li>
<li><code>scons</code></li>
<li>If you had CppUnit installed, you can then type <code>test/runtest</code> to run tests on your build.</li>
<li>To install the headers and static library in /usr/local/lib and /usr/local/include, type <code>scons install</code>
</ol>

<h3>Windows</h3>

Assuming you are using <a href="http://www.mingw.org">MinGW</a> with the MSYS commandline, the following steps should do it. Note the addition of python directories to the environment's <code>$PATH</code>.

<ol>
<li><code>tar jxvf <i>/path/to/your</i>freesteam-<?php print($latestversion)?>.tar.bz2</code></li>
<li><code>cd freesteam-<?php print($latestversion)?></code></li>
<li><code>export PATH=/mingw/bin:/bin:/Python24/Scripts:/Python24</code></li>
<li><code>scons</code></li>
<li>To install to c:\MinGW\lib and c:\MinGW\include, type <code>scons install</code></li>
</ol>

<p><b>Note</b>: We're working on getting things running on the Visual C++ compiler. Currently there are some issues with the 'units.h' file, and some problems with allowing C++ exception handling. If you're able to help with these issues, let me know.</p>

<h2>Python bindings</h2>

<p>To build the Python bindings for ASCEND, you'll need to have a development version of Python installed (standard Python on Windows, or else a 'python-devel' package on Linux). You'll also need SWIG. Python version 2.4 or later and SWIG 1.3.24 or later should be fine.</p>

<p>The SCons installer should detect Python and SWIG automatically. If you use 'scons install' in this case, the relevant files should be installed inside Python such that you can use <tt>from freesteam import *</tt> whenever you want to use reesteam.</p>

<h2>ASCEND bindings</h2>

<p>SCons will also attempt to detect <a href="http://ascend.cheme.cmu.edu/">ASCEND</a> on your system. This is a mathematical modelling and process simulation tool. You'll need a very new version of ASCEND here: at least 0.9.5.95. ASCEND support is still very experimental, and the interface is likely to change still.</p>

<p>If SCons does not correctly detect ASCEND during the build, you may find it helpful to use the <tt>ASCEND_SRCROOT</tt> option for SCons, and invoke scons with the <tt>ascend</tt> target as for example <tt>cd ~/src/freesteam && scons ASCEND_SRCROOT=~/src/ascend ascend</tt>.

<h2>EMSO library</h2>

<p>The SCons build process above allows for detection of <a href="http://www.enq.ufrgs.br/trac/alsoc/wiki/EMSO">EMSO</a> on your system, and will build 'hook libraries' to allow you to use freesteam functionality in your EMSO models. Note that this use of freesteam is not currently permitted under the license arrangement. Therefore your EMSO hook library DLLs are not re-distributable and can not be used for commercial purposes. I intend to permit their use for academic use, or for commercial use with a fee, but this has not yet been finalised (neither has the EMSO license, so that's OK, right?)</p>

<p>There is a reasonable number of sample models in the <code>emso/test</code> directory in the source tree that should allow you to test your EMSO hook library and work out what functionality is provided. There is no documentation for this yet. Note that you may need to modify the EMSO 'dll directory' settings using the EMSO GUI.</p>

<?php require("footer.php") ?>
