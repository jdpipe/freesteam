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
# OBJECT FILES

OBJS = exception.o steamcalculator.o state.o region1.o region2.o region3.o region4.o boundaries.o \
	steamcalculatorexception.o zeroin.o designbycontract.o units.o solver.o steamproperty.o \
	satcurve.o b23curve.o b13curve.o convergencetest.o solver2.o iapws95.o

VPATH = test:ctest:ees:emso:cli

#--------------------------
# BUILD

.PHONY: all again

all: lib example

lib: $(LIB)

again: mostlyclean all

$(LIB): $(OBJS)
	$(AR) -cr $@ $(OBJS)

example: example.o $(LIB)
	$(CXX) -o $@ -static $(LDFLAGS) $^ 

#--------------------------
# SUBDIRECTORIES

.PHONY: emso ees test

emso:
	make -C emso

ees:
	make -C ees

test:
	make -C test

#--------------------------
# CLEAN

.PHONY : clean

CLEANDIRS := test ctest cli ees emso

clean: cleansubdirs cleanhere

cleanhere:
	@-rm *.o *.stackdump

cleansubdirs:
	$(foreach dir,$(CLEANDIRS),make -C $(dir) clean;)

distclean: distcleanhere distcleansubdirs

distcleanhere: cleanhere
	@-rm example$(EXE_SUFFIX) $(LIB) *.d

distcleansubdirs:
	-$(foreach dir,$(CLEANDIRS),make -C $(dir) distclean;)


# .PHONY: clean mostlyclean cleanobjs cleantests cleanctests
# 
# clean: mostlyclean
# 	-rm *.d
# 
# mostlyclean: cleanobjs cleantests cleanctests
# 
# cleanobjs:
# 	-rm *.o *~ core *.stackdump *.a
# 
# cleantests:
# 	@-rm *.test *.test$(EXE_SUFFIX)
# 
# cleanctests:
# 	@-rm *.cpass$(EXE_SUFFIX)
# 	@-rm *.cfail$(EXE_SUFFIX)
# 	@-rm &.cfail.cpp.ok
# 	@-rm ctest.skip
	
#--------------------------
# MAKEDEPEND

-include $(OBJS:.o=.d )

#--------------------------
# DISTRIBUTION

DISTFILE = $(NAME)-$(VERSION).tar.bz2

.PHONY: dist

dist: 

$(DISTFILE): # tags
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
	tar -C /tmp -cjvf ./$(DISTFILE) $(NAME)-$(VERSION)
	-rm -rf /tmp/$(NAME)-$(VERSION)



#-----------------------------
# DOCUMENTATION

.PHONY: README

README:
	wget -N -O README.html http://pye.dyndns.org/freesteam/

