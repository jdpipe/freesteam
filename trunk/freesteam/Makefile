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


# This is the GNU Makefile for freesteam. 
# Use 'make test' or 'make example' on an GCC system such as Linux, mingw or cygwin to get started.

#---------------------------
# PROJECT

NAME = freesteam
VERSION = 0.4.1

EMSO_NAME = emsofreesteam-$(VERSION)

#---------------------------
# GET PLATFORM SPECIFIC SETTINGS

BUILD_PLATFORM := $(shell ./config.guess)
include make-$(BUILD_PLATFORM).mak

#---------------------------
# CONFIGURATION

#CXX = /usr/local/bin/gfilt
CXX = g++

CXXFLAGS = -fmessage-length=170 -O3 -Wall
LDFLAGS = -lm -O3

CPPUNIT_CONFIG = /usr/local/bin/cppunit-config

CPPFLAGS_CPPUNIT = $(shell $(CPPUNIT_CONFIG) --cflags)
LIBS_CPPUNIT = $(shell $(CPPUNIT_CONFIG) --libs)

CPPFLAGS = -DVERSION=\"$(BUILD_PLATFORM)-$(VERSION)\" -W $(CPPFLAGS_CPPUNIT)

OBJS = exception.o steamcalculator.o state.o region1.o region2.o region3.o region4.o boundaries.o \
	steamcalculatorexception.o zeroin.o designbycontract.o units.o solver.o steamproperty.o \
	satcurve.o b23curve.o b13curve.o convergencetest.o solver2.o

# This line will cause all tests to be run in alphabetical order by filename
#TESTS = $(shell ls *.test.cpp | /bin/sed -r s%\.test\.cpp%.test.o%)

# Or, define the tests in the order you want:
TESTS = cppunit.test.o empty.test.o units.test.o batchtest.test.o \
	zeroin.test.o \
	region4.test.o b23.test.o boundaries.test.o \
	rhosat.test.o \
	region1.test.o region2.test.o region3.test.o \
	b234point.test.o \
	criticalpoint.test.o \
	lowpressure.test.o \
	iapws95.test.o iapws9795.test.o \
	pressuresolver.test.o temperaturesolver.test.o \
	satcurve.test.o	b23curve.test.o \
	b13curve.test.o \
	whichstatestr.test.o \
	solver2.test.o
#	temperaturesolver2.test.o

CPASS = $(wildcard *.cpass.cpp)
CFAIL = $(wildcard *.cfail.cpp)

TESTEXE = runtest$(EXE_SUFFIX)
TESTOBJS = steamtestpoint.o iapws95.o

HOOKOBJS = emsohooks.o

CLIS = diagram.cli.cpp pboundt.cli.cpp

CLIOBJS = diagram.o

LIB = libsteam.a

MAKEDEPFLAG = -MM

#--------------------------
# BUILD

.PHONY: all again

all: $(LIB) example

again: mostlyclean all

$(LIB): $(OBJS)
	$(AR) -cr $@ $(OBJS)

example: example.o $(LIB)
	$(CXX) $^ -o $@ $(LDFLAGS)

#--------------------------
# EMSO

EMSO = $(EMSO_NAME)$(SO_SUFFIX)

emso: $(LIB) $(EMSO)

$(EMSO): emsohooks.o Makefile
	g++ -shared -o $@ $< $(LIB) $(LDFLAGS) $(EMSO_LD_SONAME)

#-------------------------
# CLEAN

.PHONY: clean mostlyclean cleanobjs cleantests cleanctests

clean: mostlyclean
	-rm *.d

mostlyclean: cleanobjs cleantests cleanctests

cleanobjs:
	-rm *.o *~ core *.stackdump *.a

cleantests:
	@-rm *.test *.test$(EXE_SUFFIX)

cleanctests:
	@-rm *.cpass$(EXE_SUFFIX)
	@-rm *.cfail$(EXE_SUFFIX)
	@-rm &.cfail.cpp.ok
	@-rm ctest.skip
	
#--------------------------
# MAKEDEPEND

-include $(OBJS:.o=.d )
-include $(TESTS:.o=.d)
-include $(TESTOBJS:.o=.d)
-include $(CLIS:.cpp=.cli.d)
-include $(CLIOBJS:.o=.d)
-include $(HOOKOBJS:.o=.d)
-include $(CPASS:.cpp=.d)
-include $(CFAIL:.cpp=.d)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) $(MAKEDEPFLAG) $(CPPFLAGS) $(@:.d=.cpp) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$;

#--------------------------
# DISTRIBUTION

.PHONY: tar gz bz2

tar: $(NAME)-$(VERSION).tar

gz: $(NAME)-$(VERSION).tar.gz

bz2: $(NAME)-$(VERSION).tar.bz2

$(NAME)-$(VERSION).tar.gz:$(NAME)-$(VERSION).tar
	gzip -9 $<
	
$(NAME)-$(VERSION).tar.bz2:$(NAME)-$(VERSION).tar
	bzip2 -9 $<

$(NAME)-$(VERSION).tar: # tags
	mkdir /tmp/$(NAME)-$(VERSION)
	-rm ./$(NAME)-$(VERSION).tar
	cp -R * /tmp/$(NAME)-$(VERSION)
	-rm /tmp/$(NAME)-$(VERSION)/*.o
	-rm /tmp/$(NAME)-$(VERSION)/*.d
	-rm /tmp/$(NAME)-$(VERSION)/*.ok
	-rm /tmp/$(NAME)-$(VERSION)/*.a
	-rm /tmp/$(NAME)-$(VERSION)/*$(EXE_SUFFIX)
	-rm /tmp/$(NAME)-$(VERSION)/*.log
	-rm /tmp/$(NAME)-$(VERSION)/core
	-rm /tmp/$(NAME)-$(VERSION)/*.stackdump
	-rm /tmp/$(NAME)-$(VERSION)/*.tar
	-rm /tmp/$(NAME)-$(VERSION)/*.tar.bz2
	-rm /tmp/$(NAME)-$(VERSION)/*.tar.gz
	-rm /tmp/$(NAME)-$(VERSION)/*.
	-rm /tmp/$(NAME)-$(VERSION)/ctest.skip
	-rm -Rf /tmp/$(NAME)-$(VERSION)/CVS/
	tar -C /tmp -cvf ./$(NAME)-$(VERSION).tar $(NAME)-$(VERSION)
	-rm -rf /tmp/$(NAME)-$(VERSION)

#--------------------------
# COMPILE-TIME TESTING

.PHONY: ctest cpasstest cfailtest ctestheading do_ctest

CPASSEXES = $(CPASS:.cpp=$(EXE_SUFFIX) )
CFAILOKS = $(CFAIL:.cpp=.ok )

ctest: $(LIB) ctestheading do_ctest

do_ctest:
	@if [ -f ctest.skip ]; then echo "Skipping ('rm ctest.skip' to force compile-time tests)"; exit 0; fi; \
	echo "Running compile-time tests... ('touch ctest.skip' to skip these tests)"; \
	make $(CPASSEXES) $(CFAILOKS)

ctestheading:
	@echo
	@echo "COMPILE-TIME TESTING"
	@echo

%.cpass$(EXE_SUFFIX): %.cpass.cpp $(LIB) Makefile
	@echo "ctest $< (expect pass)..."
	@echo $(CXX) -DCTEST $< $(LIB) -o $@ $(LDFLAGS);
	@if $(CXX) -DCTEST $< $(LIB) -o $@ $(LDFLAGS); then 0 \
	else \
	  echo; \
	  echo "$<:1: CTEST FAIL: Expected to compile, but did not."; \
	  echo; \
	  exit 1; \
	fi;

%.cfail.ok: %.cfail.cpp $(LIB) Makefile
	@echo "ctest $< (expect fail)..."
	@echo $(CXX) -DCTEST $< $(LIB) -o cfail$(EXE_SUFFIX) $(LDFLAGS)
	@if $(CXX) -DCTEST $< $(LIB) -o cfail$(EXE_SUFFIX) $(LDFLAGS); then \
	  echo; \
	  echo "$<:1: CTEST FAIL: Shouldn't have compiled, but did."; \
	  echo; \
	  if [ -f $@ ]; then rm cfail$(EXE_SUFFIX); fi; \
	  exit 1; \
	fi; \
	echo "$<" > $@;

#--------------------------
# RUN-TIME TESTING

.PHONY:	test testheading

test: ctest testheading $(TEXT_EXE) $(TESTS)
	./$(TEST_EXE)

$(TEST_EXE): test.o $(TESTS) $(TESTOBJS) $(LIB)
	@echo
	@echo "CppUnit libs flag: " $(LIBS_CPPUNIT)
	@echo
	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS_CPPUNIT)

testheading:
	@echo
	@echo "RUNTIME TESTING"
	@echo

#-----------------------------
# LITTLE COMMANDLINE DIAGNOSTIC PROGRAMS

%.cli$(EXE_SUFFIX): %.cli.o $(OBJS) $(CLIOBJS)
	$(CXX) $< $(OBJS) $(CLIOBJS) -o $@ $(LDFLAGS)

critsurf.m: criticalsurface.cli$(EXE_SUFFIX)
	./$< > $@

pboundt.m: pboundt.cli$(EXE_SUFFIX)
	./$< > $@

diagram.m: diagram.cli$(EXE_SUFFIX)
	./$< > $@

#-----------------------------
# DOCUMENTATION

.PHONY: README

README:
	wget -N -O README.html http://pye.dyndns.org/freesteam/

#-----------------------------
# INDENT THE CODE

ASTYLE = astyle
ASTYLEFLAGS = -c --style=kr -CNSL --indent=tab=4 -m8 --max-instatement-indent=4 --one-line=keep-blocks --suffix=.asty --one-line=keep-statements

ASTYLE_TARGETS_CXX = $(shell ls *.cpp | /bin/sed -r s%\.cpp$$%.cpp.asty%)
ASTYLE_TARGETS_H = $(shell ls *.h | /bin/sed -r s%\.h$$%.h.asty%)


.PHONY: beautiful 

beautiful: $(ASTYLE_TARGETS_CXX) $(ASTYLE_TARGETS_H)
	@echo "(but only a mamma c++ could love a baby c++)"

%.cpp.asty: %.cpp
	-$(ASTYLE) $(ASTYLEFLAGS) $< && touch $@;
%.h.asty: %.h
	-$(ASTYLE) $(ASTYLEFLAGS) $< && touch $@;
