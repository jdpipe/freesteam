import platform
if platform.system()=="Windows":
  # we have to do this unless we want to put our extension directly into Python's system path. or maybe there's a better way...
  import os, sys
  from pathlib import Path
  os.add_dll_directory(Path(os.environ['HOME']) + Path(f".local/lib/python{sys.version_info.major}.{sys.version_info.minor}"))

import freesteam

S = freesteam.steam_ph(100e5,300)

print("TESTING RESULTS")

print("region =",S.region)
print("h =",S.h)
print("v =",S.v)
print("p =",S.p)
print("s =",S.s)
print("mu=",S.mu)

print("TESTING PV")

p = 500e5
v = 1./401.
print("(p,v) = (%f, %f)" % (p, v))
print("bounds errors?",freesteam.bounds_pv(p,v,1))
print("region?",freesteam.region_pv(p,v))

S = freesteam.steam_pv(p,v)

print("region =",S.region)
print("h =",S.h)
print("v =",S.v)
print("p =",S.p)
print("s =",S.s)
print("mu=",S.mu)


