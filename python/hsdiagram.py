#!/usr/bin/python
# mollier - plot a Mollier diagram using freesteam libraries
# Copyright 2008,2009 Grant Ingram
# Copyright 2009 John Pye
##     This program is free software: you can redistribute it and/or modify
##     it under the terms of the GNU General Public License as published by
##     the Free Software Foundation, either version 3 of the License, or
##     (at your option) any later version.

##     This program is distributed in the hope that it will be useful,
##     but WITHOUT ANY WARRANTY; without even the implied warranty of
##     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##     GNU General Public License for more details.

##     You should have received a copy of the GNU General Public License
##     along with this program.  If not, see <http://www.gnu.org/licenses/>

from freesteam import *
from matplotlib import rc
rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
## for Palatino and other serif fonts use:
#rc('font',**{'family':'serif','serif':['Palatino']))
rc('text', usetex=True)
from pylab import *
from math import *

# Diagram Ranges - it would be nice to do this automagically but for
# the moment this will have to do..
smin = 5; smax = 9;
hmin = 2000; hmax = 4500;
tmin =  6.96963241256; tmax = 800;
pressure_range = [0.01,0.05,0.1,0.2,0.5,1,2,5,10,20,40,60,80,100,150,200]
pmax = 500; pmin = 0.01;
twophase_x_range = [0.65,0.70,0.75,0.80,0.85,0.90,0.95,1.0]
const_T_range = [50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800]
xy_ratio = float(hmax - hmin) / float(smax - smin)

# This makes the picture A4 matplotlib accepts dimensions in inches...
figure(figsize = (8.26771654,11.6929134))
paper_ratio = 11.6929134 / 8.26771654

# supercritical region
print "Supercritical region..."

supcrit_p = [250,300,400,500]
supcrit_T = linspace(tmin,tmax,50)

for i in supcrit_p:
    x = []; y = [];
    p = i * 1e5 #bar
    for j in supcrit_T:
        T = j+273.15 # K
        S = steam_pT(p,T)
        y.append(S.h/1000)
        x.append(S.s/1000)
    plot(x,y,'b-')
    text(x[-1]-0.05,y[-1]+10,r'%3.0f bar' % i,color="blue",rotation='vertical')

# lines of constant pressure

print "Lines of constant pressure..."

for i in pressure_range:
    p = i * 1e5 #bar
    #print "p = %f bar"%p
    S = region4_Tx(Tsat_p(p),1)
    saturation_T = (S.T-273.15)
    superheat_T_range = linspace(saturation_T,tmax,50)
    
    x = []; y = []; 
    T = saturation_T + 273.15 # K
    twophase_x_range_fine = linspace(min(twophase_x_range),max(twophase_x_range),50)
    for j in twophase_x_range_fine:
        S = region4_Tx(T,j)
        x.append(S.s/1000)
        y.append(S.h/1000)

    for j in superheat_T_range:
        T = j+273.15 # K
        #print "p = %f, T = %f" %(p,T)
        S = steam_pT(p,T)
        if S.T > Tsat_p(S.p):
            x.append(S.s/1000)
            y.append(S.h/1000)
    plot(x,y,'b-')

    if x[-1] < smax:
        text(x[-1]-0.05,y[-1]+10,r'%3.0f bar' % i,color="blue",rotation='vertical')
    else:
        for k in range(len(x)): # find index which the line crosses smax
            if x[k] > smax: # we've gone past the limit
                angle = atan(paper_ratio * ( ((y[k-8]-y[k-7]) /(x[k-8]-x[k-7])) / xy_ratio) )
                text_x_move = 0.15 * sin (angle) 
                text_y_move = 0.15 * cos (angle) * paper_ratio / xy_ratio
                angle = angle * 180.0 / pi
                text(x[k-8]-text_x_move,y[k-8]-text_y_move,r'%3.2f bar' % i,color="blue",rotation=angle)
                break
                
# lines of constant dryness fraction

print "Lines of dryness fraction (quality)..."

twophase_T = linspace(tmin,373,20)

for i in twophase_x_range:
    x = []; y = [];
    for j in twophase_T:
        T = j+273.15 # K
        S = region4_Tx(T,i)
        y.append(S.h/1000)
        x.append(S.s/1000)
    plot(x,y,'r-')
    if y[0] > hmin:
        text(x[0]+0.12,y[0]-5,r'%0.2f' % i,color="red")

# lines of constant temperature

print "Lines of constant temperature..."

for i in const_T_range:
    T = i + 273.15 # K
    #print "T = %f" % T
    if T < TCRIT: # temperature is below the critical point
        S = region4_Tx(T,1)
        saturation_p = float(S.p / 1e5)        
        p_range = linspace(pmin,saturation_p,50)
    else:
        p_range = linspace(pmin,pmax,50)

    x = []; y = []; 

    for j in p_range:
        p = j * 1e5 # bar
        S = steam_pT(p,T)
        if S.T > Tsat_p(p):
            x.append(S.s/1000)
            y.append(S.h/1000)                

    if len(x):
        plot(x,y,'g-')
        text(smax+0.1,max(y)-10,r'%3.0f $^\circ$ C' % i,color="green")

# work-around for non-helvetica default axes
# in theory this shouldn't be required
ylabels = []
ytickloc = arange(hmin,hmax+500,500)
for i in ytickloc: ylabels.append(str(i))
xlabels = []
xtickloc = arange(smin,smax+0.5,0.5)
for i in xtickloc: xlabels.append(str(i))

figtext(0.15,0.85,\
        "Calculated with freesteam v%s \n IAPWS-IF97 Industrial Formulation" % FREESTEAM_VERSION
		,bbox=dict(facecolor='white', alpha=0.75))

# Axes details
grid(True)
gca().xaxis.grid(True, which='minor')  # minor grid on too
gca().yaxis.grid(True, which='minor')  # minor grid on too
title('Mollier Diagram')
ylabel('Specific Enthalpy, kJ / kg')
xlabel('Specific Entropy, kJ / kg K')
xticks(xtickloc,xlabels)
yticks(ytickloc,ylabels)
xlim( smin,smax )
ylim( hmin,hmax )

#print "Exporting PNG file to current directory..."
#savefig('mollier.png',dpi = 600)

#print "Exporting EPS file to current directory..."
#savefig('mollier.eps',dpi = 600)

show()

