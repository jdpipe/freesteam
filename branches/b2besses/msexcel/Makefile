WORKDIR = %cd%

########################################################
#	Platform specific section
########################################################
ifeq ($(PLATFORM), WIN64)
CC = x86_64-w64-mingw32-gcc.exe
CXX = x86_64-w64-mingw32-g++.exe
LD = x86_64-w64-mingw32-g++.exe
LIBDIR =  -L"$(XLW)\xlw\lib\x64"
CFLAGS =  -m64 -DWIN64 -O3 -Wall -DNDEBUG
OUT = freesteam-w64.xll

else
CC = mingw32-gcc.exe
CXX = mingw32-g++.exe
LD = mingw32-g++.exe
LIBDIR =  -L"$(XLW)\xlw\lib"
CFLAGS =  -O3 -Wall -DNDEBUG
OUT = freesteam.xll

endif

########################################################
#	Common section
########################################################
INC =  -I$(XLW)\\xlw\\include -I..\\
LIB =-lxlw-gcc-s-5_0_0f0
LDFLAGS =  -Wl,"$(XLW)/xlw/include/xlw/xlw.def" \
        -Wl,-static \
        -static-libgcc
OBJDIR = obj

OBJ = $(OBJDIR)\\zeroin.o $(OBJDIR)\\viscosity.o $(OBJDIR)\\thcond.o $(OBJDIR)\\steam_pT.o $(OBJDIR)\\steam.o $(OBJDIR)\\region4.o $(OBJDIR)\\region3.o $(OBJDIR)\\region2.o $(OBJDIR)\\region1.o $(OBJDIR)\\xlwWrapper.o $(OBJDIR)\\fstm_sat.o $(OBJDIR)\\fstm_pT.o $(OBJDIR)\\common.o $(OBJDIR)\\b23.o

all: release

before: 
	mingw32-make -f RunInterfaceGenerator.mak BUILD=Release
	cmd /c if not exist $(OBJDIR) md $(OBJDIR)

release: before out

out: $(OBJ) $(DEP)
	$(LD) -shared -s -o $(OUT) $(LDFLAGS) $(LIBDIR) $(OBJ) $(LIB)

$(OBJDIR)\\zeroin.o: ..\\zeroin.c
	$(CC) $(CFLAGS) $(INC) -c ..\\zeroin.c -o $(OBJDIR)\\zeroin.o

$(OBJDIR)\\viscosity.o: ..\\viscosity.c
	$(CC) $(CFLAGS) $(INC) -c ..\\viscosity.c -o $(OBJDIR)\\viscosity.o

$(OBJDIR)\\thcond.o: ..\\thcond.c
	$(CC) $(CFLAGS) $(INC) -c ..\\thcond.c -o $(OBJDIR)\\thcond.o

$(OBJDIR)\\steam_pT.o: ..\\steam_pT.c
	$(CC) $(CFLAGS) $(INC) -c ..\\steam_pT.c -o $(OBJDIR)\\steam_pT.o

$(OBJDIR)\\steam.o: ..\\steam.c
	$(CC) $(CFLAGS) $(INC) -c ..\\steam.c -o $(OBJDIR)\\steam.o

$(OBJDIR)\\region4.o: ..\\region4.c
	$(CC) $(CFLAGS) $(INC) -c ..\\region4.c -o $(OBJDIR)\\region4.o

$(OBJDIR)\\region3.o: ..\\region3.c
	$(CC) $(CFLAGS) $(INC) -c ..\\region3.c -o $(OBJDIR)\\region3.o

$(OBJDIR)\\region2.o: ..\\region2.c
	$(CC) $(CFLAGS) $(INC) -c ..\\region2.c -o $(OBJDIR)\\region2.o

$(OBJDIR)\\region1.o: ..\\region1.c
	$(CC) $(CFLAGS) $(INC) -c ..\\region1.c -o $(OBJDIR)\\region1.o

$(OBJDIR)\\xlwWrapper.o: xlwWrapper.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlwWrapper.cpp -o $(OBJDIR)\\xlwWrapper.o

$(OBJDIR)\\fstm_sat.o: fstm_sat.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_sat.cpp -o $(OBJDIR)\\fstm_sat.o

$(OBJDIR)\\fstm_pT.o: fstm_pT.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_pT.cpp -o $(OBJDIR)\\fstm_pT.o

$(OBJDIR)\\common.o: ..\\common.c
	$(CC) $(CFLAGS) $(INC) -c ..\\common.c -o $(OBJDIR)\\common.o

$(OBJDIR)\\b23.o: ..\\b23.c
	$(CC) $(CFLAGS) $(INC) -c ..\\b23.c -o $(OBJDIR)\\b23.o

clean: 
	cmd /c del /f $(OBJ) $(OUT) xlwWrapper.cpp
	cmd /c rd $(OBJDIR)

.PHONY: before after clean

