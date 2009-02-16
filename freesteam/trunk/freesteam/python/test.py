#!/usr/bin/env python
# trickery to add .. to LD_LIBRARY_PATH before attempting to load freesteam lib
import os, sys, platform
up = os.path.abspath(os.path.join(sys.path[0],'..'))

if platform.system()=="Windows":
	lib = '_freesteam.pyd'
	pathvar = 'PATH'
	exe = sys.executable
else:
	pathvar = 'LD_LIBARY_PATH'
	lib = '_freesteam.so'
	exe = 'python'
	
if not os.environ.get(pathvar) or up not in os.environ.get(pathvar):
	os.environ[pathvar] = up
	script = os.path.join(sys.path[0],"test.py")
	print "RESTARTING"
	os.execvp(exe,[script] + sys.argv)
else:
	print "%s = %s" %(pathvar,os.environ.get(pathvar))

if not os.path.exists(os.path.join(sys.path[0],lib)):
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

