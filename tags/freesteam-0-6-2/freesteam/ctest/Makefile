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

CPASS = $(wildcard *.cpass.cpp)
CFAIL = $(wildcard *.cfail.cpp)

EXE_SUFFIX = .exe

all: ctest

-include $(CPASS:.cpp=.d)
-include $(CFAIL:.cpp=.d)

LIB = ../libfreesteam.a

#--------------------------
# COMPILE-TIME TESTING

.PHONY: ctest cpasstest cfailtest ctestheading do_ctest

CPASSEXES = $(CPASS:.cpp=$(EXE_SUFFIX) )
CFAILOKS = $(CFAIL:.cpp=.ok )

ctest: $(LIB) ctestheading do_ctest

do_ctest:
	@if [ -f ctest.skip ]; then echo "Skipping ('rm ctest.skip' to force compile-time tests)"; echo; exit 0; fi; \
	echo "Running compile-time tests... ('touch ctest.skip' to skip these tests)"; \
	make $(CPASSEXES) $(CFAILOKS); \
	echo \
	echo "COMPILE-TIME TESTS SUCCEEDED!" \
	echo \

ctestheading:
	@echo
	@echo "COMPILE-TIME TESTING"
	@echo

%.cpass$(EXE_SUFFIX): %.cpass.cpp $(LIB) Makefile
	@echo "ctest $< (expect pass)..."
	@echo $(CXX) -DCTEST $< $(LIB) -o $@ $(LDFLAGS);
	@if $(CXX) -DCTEST $< $(LIB) -o $@ $(LDFLAGS); then exit 0; \
	else \
	  echo; \
	  echo "$<:1: FAIL: Expected to compile, but did not."; \
	  echo; \
	  exit 1; \
	fi;

%.cfail.ok: %.cfail.cpp $(LIB) Makefile
	@echo "ctest $< (expect fail)..."
	@echo $(CXX) -DCTEST $< $(LIB) -o cfail$(EXE_SUFFIX) $(LDFLAGS)
	@if $(CXX) -DCTEST $< $(LIB) -o cfail$(EXE_SUFFIX) $(LDFLAGS); then \
	  echo; \
	  echo "$<:1: FAIL: Shouldn't have compiled, but did."; \
	  echo; \
	  if [ -f $@ ]; then rm cfail$(EXE_SUFFIX); fi; \
	  exit 1; \
	fi; \
	echo "$<" > $@;

#---------------------------------------------------------
# CLEAN

clean:
	@-rm *.o *.d.* $(CFAILOKS) $(CPASSEXES) *.stackdump

distclean: clean
	@-rm *.d