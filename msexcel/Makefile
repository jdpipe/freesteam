WORKDIR = %cd%

########################################################
#	Platform specific section
########################################################
ifeq ($(PLATFORM), WIN64)
CC = x86_64-w64-mingw32-gcc.exe
CXX = x86_64-w64-mingw32-g++.exe
LD = x86_64-w64-mingw32-g++.exe
LIBDIR =  -L"$(XLW)\xlw\lib\x64" -L"C:\GNU\gsl-1.15-dev-win64\lib"
CFLAGS =  -m64 -DWIN64 -O3 -Wall
OBJDIR = obj64
OUT = freesteam-w64.xll

else
CC = mingw32-gcc.exe
CXX = mingw32-g++.exe
LD = mingw32-g++.exe
LIBDIR =  -L"$(XLW)\xlw\lib" -L"C:\GNU\gsl-1.15-dev-win32\lib"
CFLAGS =  -O3 -Wall
OBJDIR = obj
OUT = freesteam.xll

endif

########################################################
#	Common section
########################################################
INC =  -I$(XLW)\\xlw\\include -I..\\ -I"C:\GNU\gsl-1.15-dev-win32\include"
LIB =-lxlw-gcc-s-5_0_0f0 -lgsl -lgslcblas
LDFLAGS =  -Wl,"$(XLW)/xlw/include/xlw/xlw.def" \
        -Wl,-static \
        -static-libgcc

########################################################

OBJ = $(OBJDIR)\\steam.o $(OBJDIR)\\steam_Ts.o $(OBJDIR)\\steam_pT.o $(OBJDIR)\\steam_ph.o $(OBJDIR)\\steam_ps.o $(OBJDIR)\\steam_pu.o $(OBJDIR)\\steam_pv.o $(OBJDIR)\\surftens.o $(OBJDIR)\\thcond.o $(OBJDIR)\\viscosity.o $(OBJDIR)\\zeroin.o $(OBJDIR)\\b23.o $(OBJDIR)\\backwards.o $(OBJDIR)\\common.o $(OBJDIR)\\fstm_Ts.o $(OBJDIR)\\fstm_pT.o $(OBJDIR)\\fstm_ph.o $(OBJDIR)\\fstm_ps.o $(OBJDIR)\\fstm_pu.o $(OBJDIR)\\fstm_pv.o $(OBJDIR)\\fstm_sat.o $(OBJDIR)\\xlwWrapper.o $(OBJDIR)\\region1.o $(OBJDIR)\\region2.o $(OBJDIR)\\region3.o $(OBJDIR)\\region4.o $(OBJDIR)\\solver2.o $(OBJDIR)\\bounds.o $(OBJDIR)\\derivs.o

all: release

before:
	mingw32-make -f RunInterfaceGenerator.mak BUILD=Release
	cmd /c if not exist $(OBJDIR) md $(OBJDIR)

########################################################

release: before out

out: $(OBJ) $(DEP)
	$(LD) -shared -s -o $(OUT) $(LDFLAGS) $(LIBDIR) $(OBJ) $(LIB)

########################################################

$(OBJDIR)\\steam.o: ..\\steam.c
	$(CC) $(CFLAGS) $(INC) -c ..\\steam.c -o $(OBJDIR)\\steam.o

$(OBJDIR)\\steam_Ts.o: ..\\steam_Ts.c
	$(CC) $(CFLAGS) $(INC) -c ..\\steam_Ts.c -o $(OBJDIR)\\steam_Ts.o

$(OBJDIR)\\steam_pT.o: ..\\steam_pT.c
	$(CC) $(CFLAGS) $(INC) -c ..\\steam_pT.c -o $(OBJDIR)\\steam_pT.o

$(OBJDIR)\\steam_ph.o: ..\\steam_ph.c
	$(CC) $(CFLAGS) $(INC) -c ..\\steam_ph.c -o $(OBJDIR)\\steam_ph.o

$(OBJDIR)\\steam_ps.o: ..\\steam_ps.c
	$(CC) $(CFLAGS) $(INC) -c ..\\steam_ps.c -o $(OBJDIR)\\steam_ps.o

$(OBJDIR)\\steam_pu.o: ..\\steam_pu.c
	$(CC) $(CFLAGS) $(INC) -c ..\\steam_pu.c -o $(OBJDIR)\\steam_pu.o

$(OBJDIR)\\steam_pv.o: ..\\steam_pv.c
	$(CC) $(CFLAGS) $(INC) -c ..\\steam_pv.c -o $(OBJDIR)\\steam_pv.o

$(OBJDIR)\\surftens.o: ..\\surftens.c
	$(CC) $(CFLAGS) $(INC) -c ..\\surftens.c -o $(OBJDIR)\\surftens.o

$(OBJDIR)\\thcond.o: ..\\thcond.c
	$(CC) $(CFLAGS) $(INC) -c ..\\thcond.c -o $(OBJDIR)\\thcond.o

$(OBJDIR)\\viscosity.o: ..\\viscosity.c
	$(CC) $(CFLAGS) $(INC) -c ..\\viscosity.c -o $(OBJDIR)\\viscosity.o

$(OBJDIR)\\zeroin.o: ..\\zeroin.c
	$(CC) $(CFLAGS) $(INC) -c ..\\zeroin.c -o $(OBJDIR)\\zeroin.o

$(OBJDIR)\\b23.o: ..\\b23.c
	$(CC) $(CFLAGS) $(INC) -c ..\\b23.c -o $(OBJDIR)\\b23.o

$(OBJDIR)\\backwards.o: ..\\backwards.c
	$(CC) $(CFLAGS) $(INC) -c ..\\backwards.c -o $(OBJDIR)\\backwards.o

$(OBJDIR)\\common.o: ..\\common.c
	$(CC) $(CFLAGS) $(INC) -c ..\\common.c -o $(OBJDIR)\\common.o

$(OBJDIR)\\fstm_Ts.o: fstm_Ts.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_Ts.cpp -o $(OBJDIR)\\fstm_Ts.o

$(OBJDIR)\\fstm_Tx.o: fstm_Tx.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_Tx.cpp -o $(OBJDIR)\\fstm_Tx.o

$(OBJDIR)\\fstm_pT.o: fstm_pT.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_pT.cpp -o $(OBJDIR)\\fstm_pT.o

$(OBJDIR)\\fstm_ph.o: fstm_ph.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_ph.cpp -o $(OBJDIR)\\fstm_ph.o

$(OBJDIR)\\fstm_ps.o: fstm_ps.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_ps.cpp -o $(OBJDIR)\\fstm_ps.o

$(OBJDIR)\\fstm_pu.o: fstm_pu.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_pu.cpp -o $(OBJDIR)\\fstm_pu.o

$(OBJDIR)\\fstm_pv.o: fstm_pv.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_pv.cpp -o $(OBJDIR)\\fstm_pv.o

$(OBJDIR)\\fstm_sat.o: fstm_sat.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_sat.cpp -o $(OBJDIR)\\fstm_sat.o

$(OBJDIR)\\xlwWrapper.o: xlwWrapper.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlwWrapper.cpp -o $(OBJDIR)\\xlwWrapper.o

$(OBJDIR)\\region1.o: ..\\region1.c
	$(CC) $(CFLAGS) $(INC) -c ..\\region1.c -o $(OBJDIR)\\region1.o

$(OBJDIR)\\region2.o: ..\\region2.c
	$(CC) $(CFLAGS) $(INC) -c ..\\region2.c -o $(OBJDIR)\\region2.o

$(OBJDIR)\\region3.o: ..\\region3.c
	$(CC) $(CFLAGS) $(INC) -c ..\\region3.c -o $(OBJDIR)\\region3.o

$(OBJDIR)\\region4.o: ..\\region4.c
	$(CC) $(CFLAGS) $(INC) -c ..\\region4.c -o $(OBJDIR)\\region4.o

$(OBJDIR)\\solver2.o: ..\\solver2.c
	$(CC) $(CFLAGS) $(INC) -c ..\\solver2.c -o $(OBJDIR)\\solver2.o

$(OBJDIR)\\bounds.o: ..\\bounds.c
	$(CC) $(CFLAGS) $(INC) -c ..\\bounds.c -o $(OBJDIR)\\bounds.o

$(OBJDIR)\\derivs.o: ..\\derivs.c
	$(CC) $(CFLAGS) $(INC) -c ..\\derivs.c -o $(OBJDIR)\\derivs.o

########################################################

clean:
	cmd /c del /f $(OBJ) $(OUT) xlwWrapper.cpp
	cmd /c rd $(OBJDIR)

.PHONY: before clean

