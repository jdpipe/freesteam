#
# Makefile for steam tables library
# steam.a
#
# by John Pye
# $ Date $
#

#---------------------------
# PROJECT

NAME = steam
VERSION = 2.2

#---------------------------
# CONFIGURATION

#CXX = /usr/local/bin/gfilt
CXX = g++

CPPFLAGS = -DVERSION=\"$(BUILD_PLATFORM)-$(VERSION)\" -W -O3
#CXXFLAGS = -fmessage-length=165 -ggdb
CXXFLAGS = -fmessage-length=0 -ggdb
LDFLAGS = -lncurses -lc -lm -L/lib/

CPPUNIT_CONFIG = /usr/local/bin/cppunit-config

CXXFLAGS_CPPUNIT = $(shell $(CPPUNIT_CONFIG) --cflags)
LIBS_CPPUNIT = $(shell $(CPPUNIT_CONFIG) --libs)

OBJS = steamcalculator.o state.o region1.o region2.o region3.o region4.o boundaries.o steamcalculatorexception.o zeroin.o designbycontract.o units.o

# This line will cause all tests to be run in alphabetical order by filename
#TESTS = $(shell ls *.test.cpp | /bin/sed -r s%\.test\.cpp%.test.o%)

# Or, define the tests in the order you want:
TESTS = cppunit.test.o empty.test.o region1.test.o region2.test.o region3.test.o

LIB = libsteam.a

MAKEDEPFLAG = -MM

#--------------------------
# BUILD

all: $(LIB)
.PHONY: all

again: clean all
	
clean: cleanobjs cleantests

cleanobjs:
	-rm *.o *.d *~ core *.stackdump *.a

$(LIB): $(OBJS)
	$(AR) -cr $@ $(OBJS)

#--------------------------
# MAKEDEPEND

-include $(OBJS:.o=.d )
-include $(TESTS:.test.o=.test.d)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) $(MAKEDEPFLAG) $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

#--------------------------
# DISTRIBUTION

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
# TESTING

test.exe: test.o $(TESTS) $(LIB)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS_CPPUNIT)


#	$(CXX) $^ -o $@ $(CPPFLAGS) $(CXXFLAGS_CPPUNIT) $(CXXFLAGS) $(LDFLAGS) $(LIBS_CPPUNIT) -DTEST

test: test.exe $(TESTS) 
	./$@

.PHONY:	test

cleantests:
	@-rm *.test *.test.exe

#
#%.test.o %.test.d: %.test.cpp $(LIB)
#	$(CXX) $^ -o $@ $(CPPFLAGS) $(CXXFLAGS_CPPUNIT) $(CXXFLAGS) -DTEST
#
#-----------------------------
# INDENT THE CODE

ASTYLE = astyle
ASTYLEFLAGS = -c --style=kr -CNSL --indent=tab=4 -m8 --max-instatement-indent=4 --one-line=keep-blocks --suffix=.asty --one-line=keep-statements

ASTYLE_TARGETS_CXX = $(shell ls *.cpp | /bin/sed -r s%\.cpp$$%.cpp.asty%)
ASTYLE_TARGETS_H = $(shell ls *.h | /bin/sed -r s%\.h$$%.h.asty%)

beautiful: $(ASTYLE_TARGETS_CXX) $(ASTYLE_TARGETS_H)
.PHONY: beautiful 
# (but a rose is still a rose)

%.cpp.asty: %.cpp
	-$(ASTYLE) $(ASTYLEFLAGS) $< && touch $@;
%.h.asty: %.h
	-$(ASTYLE) $(ASTYLEFLAGS) $< && touch $@;
