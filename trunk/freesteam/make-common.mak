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

SHELL = /bin/sh

BUILD_PLATFORM := $(shell $(PACKAGE_ROOT)/config.guess)

include $(PACKAGE_ROOT)/make-$(BUILD_PLATFORM).mak

NAME = freesteam
VERSION = 0.5

EMSO_NAME = emsofreesteam
EES_NAME = eesfreesteam-$(VERSION)

vpath ctest ctest/
vpath test test/
vpath cli cli/
vpath ees ees/
vpath emso emso/

all:

#---------------------------
# CONFIGURATION

#CXX = /usr/local/bin/gfilt
CXX = g++

CXXFLAGS = -O3 -Wall $(PLATFORM_CXX_FLAGS)
LDFLAGS = $(PLATFORM_LD_FLAGS) -lm -O3

CPPFLAGS = $(PLATFORM_CPP_FLAGS) -DVERSION=\"$(BUILD_PLATFORM)-$(VERSION)\" -W

MAKEDEPFLAG = -MM

LIBNAME = steam

LIB = $(PACKAGE_ROOT)/lib$(LIBNAME).a

DOXYGEN = doxygen

#-----------------------------
# MAKE DEPEND

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) $(MAKEDEPFLAG) $(CPPFLAGS) $(@:.d=.cpp) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$;

#-----------------------------
# INDENT THE CODE

ASTYLE = astyle
ASTYLEFLAGS = -c --style=kr -CNSL --indent=tab=4 -m8 --max-instatement-indent=4 --one-line=keep-blocks --suffix=.asty --one-line=keep-statements

ASTYLE_SOURCES_CXX = $(wildcard *.cpp)
ASTYLE_SOURCES_H = $(wildcard *.h)

ASTYLE_TARGETS_CXX = $(ASTYLE_SOURCES_CXX:.cpp=.cpp.asty)
ASTYLE_TARGETS_H = $(ASTYLE_SOURCES_H:.h=.h.asty)

.PHONY: beautiful 

beautiful: $(ASTYLE_TARGETS_CXX) $(ASTYLE_TARGETS_H)
	@echo "(but only a mamma c++ could love a baby c++)"

%.cpp.asty: %.cpp
	-$(ASTYLE) $(ASTYLEFLAGS) $< && touch $@;
%.h.asty: %.h
	-$(ASTYLE) $(ASTYLEFLAGS) $< && touch $@;

