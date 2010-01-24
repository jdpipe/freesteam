# GNU Makefile to build freesteam example program using
# GCC on Windows with MSYS/MinGW. Tested with G++ 3.4.5.

# Note that you may need to modify the 'FREESTEAM=' line
# for your local install.

FREESTEAM=/c/Program Files/freesteam

LIB=${FREESTEAM}/lib
INCLUDE=${FREESTEAM}/include

all: example.exe

example.o: example.cpp
	$(CXX) -c -o $@ -I"${INCLUDE}" $<

example.exe: example.o
	$(CXX) -o $@ -L"${LIB}" $< -lfreesteam

clean:
	-rm example.o
	-rm example.exe

