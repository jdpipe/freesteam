#
# Makefile for steam tables library
# libsteam.a
#
# by John Pye
#

#---------------------------
# PROJECT

NAME = freesteam
VERSION = 0.2

#---------------------------
# CONFIGURATION

#CXX = /usr/local/bin/gfilt
CXX = g++

CPPFLAGS = -DVERSION=\"$(BUILD_PLATFORM)-$(VERSION)\" -W
#CXXFLAGS = -fmessage-length=165 -ggdb
CXXFLAGS = -fmessage-length=0 -O3
LDFLAGS = -lncurses -lc -lm -L/lib/

CPPUNIT_CONFIG = /usr/local/bin/cppunit-config

CXXFLAGS_CPPUNIT = $(shell $(CPPUNIT_CONFIG) --cflags)
LIBS_CPPUNIT = $(shell $(CPPUNIT_CONFIG) --libs)

OBJS = steamcalculator.o state.o region1.o region2.o region3.o region4.o boundaries.o steamcalculatorexception.o zeroin.o designbycontract.o units.o solver.o satcurve.o

# This line will cause all tests to be run in alphabetical order by filename
#TESTS = $(shell ls *.test.cpp | /bin/sed -r s%\.test\.cpp%.test.o%)

# Or, define the tests in the order you want:
TESTS = cppunit.test.o empty.test.o units.test.o batchtest.test.o \
	zeroin.test.o \
	region4.test.o b23.test.o boundaries.test.o region1.test.o \
	region2.test.o region3.test.o \
	iapws95.test.o iapws9795.test.o \
	pressuresolver.test.o temperaturesolver.test.o \
	satcurve.test.o	
#	temperaturesolver2.test.o

###### compile-time testing disabled:
CPASS = $(wildcard *.cpass.cpp)
CFAIL = $(wildcard *.cfail.cpp)

#CPASS = units-1.cpass.cpp
#CFAIL = units-1.cfail.cpp units-2.cfail.cpp units-

TESTOBJS = steamtestpoint.o iapws95.o

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

#-------------------------
# CLEAN

.PHONY: clean mostlyclean cleanobjs cleantests cleanctests

clean: mostlyclean
	-rm *.d

mostlyclean: cleanobjs cleantests cleanctests

cleanobjs:
	-rm *.o *~ core *.stackdump *.a

cleantests:
	@-rm *.test *.test.exe

cleanctests:
	@-rm *.cpass.exe
	@-rm *.cfail.exe
	@-rm &.cfail.cpp.ok
	
#--------------------------
# MAKEDEPEND

-include $(OBJS:.o=.d )
-include $(TESTS:.o=.d)
-include $(TESTOBJS:.o=.d)
-include $(CPASS:.cpp=.d)
-include $(CFAIL:.cpp=.d)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) $(MAKEDEPFLAG) $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

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
	-rm /tmp/$(NAME)-$(VERSION)/*.exe
	-rm /tmp/$(NAME)-$(VERSION)/*.log
	-rm /tmp/$(NAME)-$(VERSION)/core
	-rm /tmp/$(NAME)-$(VERSION)/*.stackdump
	-rm /tmp/$(NAME)-$(VERSION)/*.tar
	-rm /tmp/$(NAME)-$(VERSION)/*.tar.bz2
	-rm /tmp/$(NAME)-$(VERSION)/*.tar.gz
	-rm /tmp/$(NAME)-$(VERSION)/*.
	-rm -Rf /tmp/$(NAME)-$(VERSION)/CVS/
	tar -C /tmp -cvf ./$(NAME)-$(VERSION).tar $(NAME)-$(VERSION)
	-rm -rf /tmp/$(NAME)-$(VERSION)

#--------------------------
# COMPILE-TIME TESTING

.PHONY: ctest cpasstest cfailtest ctestheading do_ctest

CPASSEXES = $(CPASS:.cpp=.exe )
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

%.cpass.exe: %.cpass.cpp $(LIB) Makefile
	@echo "ctest $< (expect pass)..."
	@echo $(CXX) -DCTEST $< $(LIB) -o $@ $(LDFLAGS);
	@if $(CXX) -DCTEST $< $(LIB) -o $@ $(LDFLAGS); then \
	else \
	  echo; \
	  echo "$<:1: CTEST FAIL: Expected to compile, but did not."; \
	  echo; \
	  exit 1; \
	fi;

%.cfail.ok: %.cfail.cpp $(LIB) Makefile
	@echo "ctest $< (expect fail)..."
	@echo $(CXX) -DCTEST $< $(LIB) -o cfail.exe $(LDFLAGS)
	@if $(CXX) -DCTEST $< $(LIB) -o cfail.exe $(LDFLAGS); then \
	  echo; \
	  echo "$<:1: CTEST FAIL: Shouldn't have compiled, but did."; \
	  echo; \
	  if [ -f $@ ]; then rm cfail.exe; fi; \
	  exit 1; \
	fi; \
	echo "$<" > $@;

#--------------------------
# RUN-TIME TESTING

.PHONY:	test testheading

test: ctest testheading test.exe $(TESTS)
	./$@

test.exe: test.o $(TESTS) $(TESTOBJS) $(LIB)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS_CPPUNIT)

testheading:
	@echo
	@echo "RUNTIME TESTING"
	@echo

#-----------------------------
# LITTLE COMMANDLINE DIAGNOSTIC PROGRAMS

%.cli.exe: %.cli.o $(OBJS) 
	$(CXX) $< $(OBJS) -o $@ $(LDFLAGS)

critsurf.m: criticalsurface.cli.exe
	./$< > $@

pboundt.m: pboundt.cli.exe
	./$< > $@

#-----------------------------
# INDENT THE CODE

ASTYLE = astyle
ASTYLEFLAGS = -c --style=kr -CNSL --indent=tab=4 -m8 --max-instatement-indent=4 --one-line=keep-blocks --suffix=.asty --one-line=keep-statements

ASTYLE_TARGETS_CXX = $(shell ls *.cpp | /bin/sed -r s%\.cpp$$%.cpp.asty%)
ASTYLE_TARGETS_H = $(shell ls *.h | /bin/sed -r s%\.h$$%.h.asty%)


.PHONY: beautiful 

beautiful: $(ASTYLE_TARGETS_CXX) $(ASTYLE_TARGETS_H)
# (but a rose is still a rose)

%.cpp.asty: %.cpp
	-$(ASTYLE) $(ASTYLEFLAGS) $< && touch $@;
%.h.asty: %.h
	-$(ASTYLE) $(ASTYLEFLAGS) $< && touch $@;
