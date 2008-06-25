# freesteam - IAPWS-IF97 steam tables library
# Copyright (C) 2004-2005  John Pye
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

CLIS = diagram.cli.cpp pboundt.cli.cpp

CLIOBJS = diagram.o

-include $(CLIS:.cpp=.cli.d)
-include $(CLIOBJS:.o=.d)

#-----------------------------
# LITTLE COMMANDLINE DIAGNOSTIC PROGRAMS

%.cli$(EXE_SUFFIX): %.cli.o $(CLIOBJS) libcheck
	$(CXX) $(CXXFLAGS) -o $@ $< $(CLIOBJS) $(LIB) $(LDFLAGS)

critsurf.m: criticalsurface.cli$(EXE_SUFFIX)
	./$< > $@

pboundt.m: pboundt.cli$(EXE_SUFFIX)
	./$< > $@

diagram.m: diagram.cli$(EXE_SUFFIX)
	./$< > $@

#-----------------------------------------------------------
# LIBRARY

$(LIB):
	make -C .. lib

.PHONY: libcheck

libcheck: $(LIB)
	make -C .. libcheck

#---------------------------------------------------------
# CLEAN

clean:
	@-rm *.o *.d.* *.stackdump

distclean: clean
	@-rm *.cli$(EXE_SUFFIX) critsurf.m pboundt.m diagram.m *.d
