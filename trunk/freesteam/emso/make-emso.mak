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

#--------------------------
# EMSO integration

EMSO_DEBUG =

OBJS = emsohooks.o

EMSO = $(SO_PREFIX)$(EMSO_NAME)$(SO_SUFFIX)

emsohooks.o: emsohooks.cpp
ifeq ($(EMSO_DEBUG),1)
	$(CXX) $(CPPFLAGS) -DEMSO_DEBUG $(CXXFLAGS) -c -o $@ $<
else
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
endif
	
emso: $(LIB) $(OBJS) $(EMSO)

$(EMSO): $(LIB) emsohooks.o
	$(CXX) -shared -o $@ emsohooks.o $(LIB) $(LDFLAGS)

#--------------------------
# MAKEDEPEND

-include $(OBJS:.o=.d )

#-----------------------------------------------------------
# LIBRARY

$(LIB):
	make -C .. lib

.PHONY: libcheck

libcheck: $(LIB)
	make -C .. libcheck

#-----------------------------------------------------------
# CLEAN

clean:
	@-rm *.o *.d.*
	
distclean: clean
	@-rm *.dll *.d

#-----------------------------------------------------------
# INSTALL

install: emso	
ifdef EMSO_INTERFACE_DIR
	cp $(EMSO) $(EMSO_INTERFACE_DIR)/$(EMSO);
else
	@echo "Can't install, you need to specify EMSO_INTERFACE_DIR"
	exit 1;
endif