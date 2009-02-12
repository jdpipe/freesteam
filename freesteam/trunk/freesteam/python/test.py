#!/usr/bin/env python
# trickery to add .. to LD_LIBRARY_PATH before attempting to load freesteam lib
import os, sys
up = os.path.abspath(os.path.join(sys.path[0],'..'))
if not os.environ.get('LD_LIBRARY_PATH') or up not in os.environ.get('LD_LIBRARY_PATH'):
	os.environ['LD_LIBRARY_PATH'] = up
	script = os.path.join(sys.path[0],"test.py")					
	os.execvp("python",[script] + sys.argv)

if not os.path.exists(os.path.join(sys.path[0],'_freesteam.so')):
	sys.stderr.write("Library _freesteam.so not present in %s\n" % sys.path[0])
	sys.exit(1);

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

