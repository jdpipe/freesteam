
ifndef BUILD
BUILD=DEBUG
endif

.PHONY : clean


all: xlwWrapper.cpp

xlwWrapper.cpp : cppinterface.h
		"$(subst $(strip \),/,$(XLW))/xlw/build/codeblocks-gcc/bin/$(BUILD)/InterfaceGenerator.exe" cppinterface.h xlwWrapper.cpp
		@echo OK

clean :
		del xlwWrapper.cpp
