ifndef BUILD
BUILD=DEBUG
endif

all: xlwWrapper.cpp

xlwWrapper.cpp : cppinterface.h
	xlw/InterfaceGenerator/InterfaceGenerator.exe cppinterface.h xlwWrapper.cpp