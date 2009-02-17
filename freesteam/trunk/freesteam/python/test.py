#!/usr/bin/env python
# trickery to add .. to LD_LIBRARY_PATH before attempting to load freesteam lib
import os, sys, platform

if not os.path.exists(os.path.join(sys.path[0],'_freesteam.so')):
	sys.stderr.write("\nLibrary _freesteam.so not present in %s\n" % sys.path[0])
	sys.exit(1)

if platform.system()=="Windows":
	sys.stderr.write("\nBefore running tests on Windows, you need to your PATH\n")
	sys.stderr.write("to include to location of freesteam.dll. In MSYS, you\n")
	sys.stderr.write("can do this with 'export PATH=..:$PATH', assuming.\n")
	sys.stderr.write("you are in the directory containing test.py.\n")
	sys.exit(1)
	
up = os.path.abspath(os.path.join(sys.path[0],'..'))
if not os.environ.get('LD_LIBRARY_PATH') or up not in os.environ.get('LD_LIBRARY_PATH'):
	os.environ['LD_LIBRARY_PATH'] = up
	script = os.path.join(sys.path[0],"test.py")					
	os.execvp("python",[script] + sys.argv)

#-------------------------------------------------------------------------------
# now for the tests...

import freesteam

print "CREATING STEAM STATE"

S = freesteam.SteamState()

print "SETTING STEAM STATE"

S.set_ph(100e5, 300)

print "TESTING RESULTS"

print "region =",ord(S.region)
print "h =",S.h
print "v =",S.v
print "p =",S.p
print "s =",S.s

