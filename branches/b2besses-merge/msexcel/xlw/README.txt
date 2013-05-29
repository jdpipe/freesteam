
XLW MS Excel Wrapper Generator (4.0.0)
======================================

This directory contains a local copy of the XLW code taken from
https://xlw.svn.sourceforge.net/svnroot/xlw/tags/R040000f0/xlw4 (r1453).

We've created this local copy of XLW because of the ENORMOUS size of their
binary download, together with the difficulties of supporting different GCC
compilers, with different exception convections (sjlj, etc). It's really
just easier to compile our own copy.

To build this local copy of XLW, just type "scons" in this directory. The
code in the parent directory will expect you to have done this. Eventually
we'll integrate the build process so that this extra step is not required.

(That's difficult because the InterfaceGenerator, which we call 'xlw_ifgen.exe'
here, needs to be first built and then re-used as a SCons 'Tool', which is 
a bit tricky, apparently)

-- 
John Pye
29 May 2013

