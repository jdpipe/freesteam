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

TEST_EXE = runtest$(EXE_SUFFIX)
TESTOBJS = steamtestpoint.o

CPPUNIT_CONFIG = cppunit-config

CPPFLAGS_CPPUNIT = $(shell $(CPPUNIT_CONFIG) --cflags)
LDFLAGS_CPPUNIT = $(shell $(CPPUNIT_CONFIG) --libs)

ctest:
	make -C ../ctest

testobjs: $(TESTOBJS)


#-----------------------------------------------------------
# LIBRARY

$(LIB):
	make -C .. lib

.PHONY: libcheck

libcheck: $(LIB)
	make -C .. libcheck

#-----------------------------------------------------------
# DEFINE THE TESTS

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

#--------------------------
# RUN-TIME TESTING

.PHONY:	buildtest testheading

.PRECIOUS: $(TESTS) $(TESTOBJS)

test: buildtest $(TEST_EXE)
	./$(TEST_EXE)

buildtest: $(LIB)
	@if [ ! -n "$(CPPUNIT_CONFIG)" ] && [ ! -e `which $(CPPUNIT_CONFIG)` ]; then \
		echo; \
		echo "CPPUNIT NOT FOUND, make sure 'cppunit-config' command is in your path"; \
		echo; \
		exit 1; \
	fi;
	@echo
	@echo "BUILDING TESTS with CppUnit version" `$(CPPUNIT_CONFIG) --version`
	@echo "(cppunit-config command is " $(CPPUNIT_CONFIG) ")"
	@echo "Flags: " $(CPPFLAGS_CPPUNIT)
	@echo "Libs:  " $(LDFLAGS_CPPUNIT)
	@echo
	
$(TEST_EXE): test.o $(TESTS) $(TESTOBJS) libcheck
	$(CXX) test.o $(TESTS) $(TESTOBJS) $(LIB) -o $@ $(LDFLAGS) $(LDFLAGS_CPPUNIT)

testheading:
	@echo
	@echo "RUNTIME TESTING"
	@echo

%.test.d: %.test.cpp
	@set -e; rm -f $@; \
	$(CXX) $(MAKEDEPFLAG) $(CPPFLAGS) $(CPPFLAGS_CPPUNIT) $(@:.d=.cpp) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$;

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(CPPFLAGS_CPPUNIT) -c -o $@ $<

#---------------------------------------------------------
# CLEAN

clean:
	@-rm *.o *.d.*

distclean: clean
	@-rm $(TEST_EXE) *.d

#---------------------------------------------------------
# DEPENDENCIES

-include $(TESTS:.o=.d)
-include $(TESTOBJS:.o=.d)

