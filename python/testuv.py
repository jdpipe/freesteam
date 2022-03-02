from freesteam import *

p0 = 300e5;
h0 = 2000e3;
S0 = steam_ph(p0,h0)
print("S0.region =",S0.region)

u0 = S0.u
v0 = S0.v
print("u0 =",u0,"v0 =",v0)

S = steam_uv(u0,v0)

print("u =", S.u)

print("v =",S.v)

