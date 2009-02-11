import freesteam

S = freesteam.SteamState()
S.set_ph(100e5, 300)

print S.region
print "h =",S.h
print "v =",S.v
print "p =",S.p
print "s =",S.s

