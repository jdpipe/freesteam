WORKDIR = %cd%

########################################################
#	Common C flags
########################################################
COMCFLAGS = -O3 -Wall -DNDEBUG

########################################################
#	Platform specific section
########################################################
ifeq ($(PLATFORM), WIN64)
CC = x86_64-w64-mingw32-gcc.exe
CXX = x86_64-w64-mingw32-g++.exe
LD = x86_64-w64-mingw32-g++.exe
LIBDIR = -L"C:\GNU\gsl-1.15-dev-win64\lib"
CFLAGS = -m64 -DWIN64 $(COMCFLAGS)
OBJDIR = obj64
OUT = freesteam-w64.xll

else
CC = mingw32-gcc.exe
CXX = mingw32-g++.exe
LD = mingw32-g++.exe
LIBDIR = -L"C:\GNU\gsl-1.15-dev-win32\lib"
CFLAGS = $(COMCFLAGS)
OBJDIR = obj
OUT = freesteam.xll

endif

########################################################
#	Common section
########################################################
INC = -I.\\xlw\\include -I..\\ -I"C:\GNU\gsl-1.15-dev-win32\include"
LIB = -lgsl -lgslcblas
LDFLAGS = -Wl,"xlw/include/xlw/xlw.def" -Wl,-static -static-libgcc

########################################################

OBJ = $(OBJDIR)\\MJCellMatrix.o \
	$(OBJDIR)\\HiResTimer.o \
	$(OBJDIR)\\FileConverter.o \
	$(OBJDIR)\\DoubleOrNothing.o \
	$(OBJDIR)\\Dispatcher.o \
	$(OBJDIR)\\ArgList.o \
	$(OBJDIR)\\NCmatrices.o \
	$(OBJDIR)\\XlfArgDesc.o \
	$(OBJDIR)\\xlcall.o \
	$(OBJDIR)\\XlfServices.o \
	$(OBJDIR)\\XlfRef.o \
	$(OBJDIR)\\XlfOperProperties.o \
	$(OBJDIR)\\XlfOperImpl.o \
	$(OBJDIR)\\XlfFuncDesc.o \
	$(OBJDIR)\\XlfExcel.o \
	$(OBJDIR)\\XlfCmdDesc.o \
	$(OBJDIR)\\XlfArgDescList.o \
	$(OBJDIR)\\XlfAbstractCmdDesc.o \
	$(OBJDIR)\\XlOpenClose.o \
	$(OBJDIR)\\XlFunctionRegistration.o \
	$(OBJDIR)\\Win32StreamBuf.o \
	$(OBJDIR)\\TempMemory.o \
	$(OBJDIR)\\PathUpdater.o \
	$(OBJDIR)\\PascalStringConversions.o \
	$(OBJDIR)\\steam.o \
	$(OBJDIR)\\steam_Ts.o \
	$(OBJDIR)\\steam_pT.o \
	$(OBJDIR)\\steam_ph.o \
	$(OBJDIR)\\steam_ps.o \
	$(OBJDIR)\\steam_pu.o \
	$(OBJDIR)\\steam_pv.o \
	$(OBJDIR)\\surftens.o \
	$(OBJDIR)\\thcond.o \
	$(OBJDIR)\\viscosity.o \
	$(OBJDIR)\\zeroin.o \
	$(OBJDIR)\\b23.o \
	$(OBJDIR)\\backwards.o \
	$(OBJDIR)\\common.o \
	$(OBJDIR)\\fstm_Ts.o \
	$(OBJDIR)\\fstm_pT.o \
	$(OBJDIR)\\fstm_ph.o \
	$(OBJDIR)\\fstm_ps.o \
	$(OBJDIR)\\fstm_pu.o \
	$(OBJDIR)\\fstm_pv.o \
	$(OBJDIR)\\fstm_sat.o \
	$(OBJDIR)\\xlwWrapper.o \
	$(OBJDIR)\\region1.o \
	$(OBJDIR)\\region2.o \
	$(OBJDIR)\\region3.o \
	$(OBJDIR)\\region4.o \
	$(OBJDIR)\\solver2.o \
	$(OBJDIR)\\bounds.o \
	$(OBJDIR)\\derivs.o \
	$(OBJDIR)\\fstm_utils.o

all: release

before:
	mingw32-make -f RunInterfaceGenerator.mak BUILD=Release
	cmd /c if not exist $(OBJDIR) md $(OBJDIR)

########################################################

release: before out

out: $(OBJ) $(DEP)
	$(LD) -shared -s -o $(OUT) $(LDFLAGS) $(LIBDIR) $(OBJ) $(LIB)

########################################################
# XLW
########################################################

$(OBJDIR)\\MJCellMatrix.o: xlw\\src\\MJCellMatrix.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\MJCellMatrix.cpp -o $(OBJDIR)\\MJCellMatrix.o

$(OBJDIR)\\HiResTimer.o: xlw\\src\\HiResTimer.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\HiResTimer.cpp -o $(OBJDIR)\\HiResTimer.o

$(OBJDIR)\\FileConverter.o: xlw\\src\\FileConverter.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\FileConverter.cpp -o $(OBJDIR)\\FileConverter.o

$(OBJDIR)\\DoubleOrNothing.o: xlw\\src\\DoubleOrNothing.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\DoubleOrNothing.cpp -o $(OBJDIR)\\DoubleOrNothing.o

$(OBJDIR)\\Dispatcher.o: xlw\\src\\Dispatcher.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\Dispatcher.cpp -o $(OBJDIR)\\Dispatcher.o

$(OBJDIR)\\ArgList.o: xlw\\src\\ArgList.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\ArgList.cpp -o $(OBJDIR)\\ArgList.o

$(OBJDIR)\\NCmatrices.o: xlw\\src\\NCmatrices.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\NCmatrices.cpp -o $(OBJDIR)\\NCmatrices.o

$(OBJDIR)\\XlfArgDesc.o: xlw\\src\\XlfArgDesc.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlfArgDesc.cpp -o $(OBJDIR)\\XlfArgDesc.o

$(OBJDIR)\\xlcall.o: xlw\\src\\xlcall.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\xlcall.cpp -o $(OBJDIR)\\xlcall.o

$(OBJDIR)\\XlfServices.o: xlw\\src\\XlfServices.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlfServices.cpp -o $(OBJDIR)\\XlfServices.o

$(OBJDIR)\\XlfRef.o: xlw\\src\\XlfRef.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlfRef.cpp -o $(OBJDIR)\\XlfRef.o

$(OBJDIR)\\XlfOperProperties.o: xlw\\src\\XlfOperProperties.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlfOperProperties.cpp -o $(OBJDIR)\\XlfOperProperties.o

$(OBJDIR)\\XlfOperImpl.o: xlw\\src\\XlfOperImpl.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlfOperImpl.cpp -o $(OBJDIR)\\XlfOperImpl.o

$(OBJDIR)\\XlfFuncDesc.o: xlw\\src\\XlfFuncDesc.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlfFuncDesc.cpp -o $(OBJDIR)\\XlfFuncDesc.o

$(OBJDIR)\\XlfExcel.o: xlw\\src\\XlfExcel.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlfExcel.cpp -o $(OBJDIR)\\XlfExcel.o

$(OBJDIR)\\XlfCmdDesc.o: xlw\\src\\XlfCmdDesc.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlfCmdDesc.cpp -o $(OBJDIR)\\XlfCmdDesc.o

$(OBJDIR)\\XlfArgDescList.o: xlw\\src\\XlfArgDescList.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlfArgDescList.cpp -o $(OBJDIR)\\XlfArgDescList.o

$(OBJDIR)\\XlfAbstractCmdDesc.o: xlw\\src\\XlfAbstractCmdDesc.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlfAbstractCmdDesc.cpp -o $(OBJDIR)\\XlfAbstractCmdDesc.o

$(OBJDIR)\\XlOpenClose.o: xlw\\src\\XlOpenClose.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlOpenClose.cpp -o $(OBJDIR)\\XlOpenClose.o

$(OBJDIR)\\XlFunctionRegistration.o: xlw\\src\\XlFunctionRegistration.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\XlFunctionRegistration.cpp -o $(OBJDIR)\\XlFunctionRegistration.o

$(OBJDIR)\\Win32StreamBuf.o: xlw\\src\\Win32StreamBuf.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\Win32StreamBuf.cpp -o $(OBJDIR)\\Win32StreamBuf.o

$(OBJDIR)\\TempMemory.o: xlw\\src\\TempMemory.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\TempMemory.cpp -o $(OBJDIR)\\TempMemory.o

$(OBJDIR)\\PathUpdater.o: xlw\\src\\PathUpdater.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\PathUpdater.cpp -o $(OBJDIR)\\PathUpdater.o

$(OBJDIR)\\PascalStringConversions.o: xlw\\src\\PascalStringConversions.cpp
	$(CXX) $(CFLAGS) $(INC) -c xlw\\src\\PascalStringConversions.cpp -o $(OBJDIR)\\PascalStringConversions.o

########################################################
# Freesteam
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

########################################################
# Freesteam XLL
########################################################

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

$(OBJDIR)\\fstm_utils.o: fstm_utils.cpp
	$(CXX) $(CFLAGS) $(INC) -c fstm_utils.cpp -o $(OBJDIR)\\fstm_utils.o

########################################################

clean:
	cmd /c del /f $(OBJ) $(OUT) xlwWrapper.cpp
	cmd /c rd $(OBJDIR)

.PHONY: before clean

