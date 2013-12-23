ifndef BUILD
BUILD=DEBUG
endif

all: xlwWrapper_ph.cpp xlwWrapper_ps.cpp xlwWrapper_pT.cpp xlwWrapper_pu.cpp xlwWrapper_pv.cpp xlwWrapper_sat.cpp xlwWrapper_Ts.cpp

xlwWrapper_ph.cpp : cppinterface_ph.h
	xlw/InterfaceGenerator/InterfaceGenerator.exe cppinterface_ph.h xlwWrapper_ph.cpp

xlwWrapper_ps.cpp : cppinterface_ps.h
	xlw/InterfaceGenerator/InterfaceGenerator.exe cppinterface_ps.h xlwWrapper_ps.cpp

xlwWrapper_pT.cpp : cppinterface_pT.h
	xlw/InterfaceGenerator/InterfaceGenerator.exe cppinterface_pT.h xlwWrapper_pT.cpp

xlwWrapper_pu.cpp : cppinterface_pu.h
	xlw/InterfaceGenerator/InterfaceGenerator.exe cppinterface_pu.h xlwWrapper_pu.cpp

xlwWrapper_pv.cpp : cppinterface_pv.h
	xlw/InterfaceGenerator/InterfaceGenerator.exe cppinterface_pv.h xlwWrapper_pv.cpp

xlwWrapper_sat.cpp : cppinterface_sat.h
	xlw/InterfaceGenerator/InterfaceGenerator.exe cppinterface_sat.h xlwWrapper_sat.cpp

xlwWrapper_Ts.cpp : cppinterface_Ts.h
	xlw/InterfaceGenerator/InterfaceGenerator.exe cppinterface_Ts.h xlwWrapper_Ts.cpp

