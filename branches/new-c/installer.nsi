; NSIS script to create a freesteam binary installer.
; (c) John Pye, 2006-2010.
;
; Based on example2.nsi from the NSIS distribution.
;
; This script is based on example1.nsi, but it remembers the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer

!ifndef VERSION
!define VERSION 0.cvs
!endif

Name "freesteam ${VERSION}"

!include LogicLib.nsh
!include nsis\EnvVarUpdate.nsh

; The file to write
!ifdef OUTFILE
OutFile ${OUTFILE}
!else
OutFile freesteam-${VERSION}-py${PYVERSION}-setup.exe
!endif

!ifndef PYVERSION
!define PYVERSION "2.5"
!endif

;SetCompressor /FINAL zlib
SetCompressor /SOLID lzma

; The default installation directory
InstallDir $PROGRAMFILES\freesteam

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\freesteam" "Install_Dir"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

Var /GLOBAL PYOK
Var /GLOBAL PYPATH
Var /GLOBAL ASCENDOK
Var /GLOBAL ASCENDPATH
Var /GLOBAL ASCENDMODELSOK
Var /GLOBAL ASCENDMODELS

Var /GLOBAL PYINSTALLED
Var /GLOBAL ASCENDINSTALLED
Var /GLOBAL LIBINSTALLED
Var /GLOBAL EXAMPLEINSTALLED

Function .onInit
	StrCpy $PYINSTALLED ""
	StrCpy $ASCENDINSTALLED ""
	StrCpy $LIBINSTALLED ""
	StrCpy $EXAMPLEINSTALLED ""
	
	Call DetectPython
	Pop $PYOK
	Pop $PYPATH
		
	Call DetectASCEND
	Pop $ASCENDOK
	Pop $ASCENDPATH

	Call DetectASCENDModels
	Pop $ASCENDMODELSOK
	Pop $ASCENDMODELS		
			
FunctionEnd


; The stuff to install
Section "freesteam (required)"
  SectionIn RO

  DetailPrint "--- COMMON FILES ---"
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  File "LICENSE.txt"
  File "README.txt"
  File "CHANGELOG.txt"

  ; We'll use the Windows directory for the DLL
  SetOutPath $SYSDIR
  File "freesteam.dll"
    
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\freesteam "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\freesteam" "DisplayName" "freesteam IAPWS-IF97 steam tables library"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\freesteam" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\freesteam" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\freesteam" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

;--------------------------------

Section "Static library and header files"
  DetailPrint "--- HEADER FILES ---"
  	SetOutPath $INSTDIR
  	File "freesteam-config"

	SetOutPath $INSTDIR\lib
	File "libfreesteam.a"
	File "freesteam.def"
	SetOutPath "$INSTDIR\include\freesteam"
	File "*.h"
	File "python\freesteam.i"
	WriteRegDWORD HKLM "SOFTWARE\freesteam" "Lib" 1
	StrCpy $LIBINSTALLED "1"

	; Create 'freesteam-config.bat' batch file for launching the python script 'ascend-config'.
	ClearErrors
	FileOpen $0 $INSTDIR\freesteam-config.bat w
	IfErrors configerror
	FileWrite $0 "@echo off"
	FileWriteByte $0 "13"
	FileWriteByte $0 "10"
	FileWrite $0 "cd "
	FileWrite $0 $INSTDIR 
	FileWriteByte $0 "13"
	FileWriteByte $0 "10"
	FileWrite $0 "$PYPATH\python "
	FileWriteByte $0 "34" 
	FileWrite $0 "$INSTDIR\freesteam-config"
	FileWriteByte $0 "34"
	FileWrite $0 " %1 %2 %3 %4 %5 %6 %7 %8"
	FileWriteByte $0 "13"
	FileWriteByte $0 "10"
	
	Return

configerror:
	MessageBox MB_OK "The 'freesteam-config.bat' file was not installed properly; problems writing to that file."	

SectionEnd

;Section "Example C++ code"
;  DetailPrint "--- EXAMPLE FILES ---"
;	SetOutPath $INSTDIR\example
;	File "example\example.cpp"
;	File "example\SConstruct"
;	File "example\Makefile"
;	File "example\README.txt"
;	WriteRegDWORD HKLM "SOFTWARE\freesteam" "Example" 1
;	StrCpy $EXAMPLEINSTALLED "1"
;	Return
;SectionEnd

Section "Python language bindings"
	${If} $PYOK == 'OK'
		;MessageBox MB_OK "Python: $PYPATH"

		DetailPrint "--- PYTHON INTERFACE ---"

		; README file for python users
		SetOutPath $INSTDIR\python
		File python\README.txt
		File python\test.py
		File python\phdiagram.py
		File python\pTdiagram.py
		File python\pudiagram.py
		File python\rhoTdiagram.py
		File python\satcurve.py
		File python\tables.py
		File python\Thdiagram.py
		File python\Tsdiagram.py

		; Set output path to the installation directory.
		SetOutPath $PYPATH\Lib\site-packages

		; Python interface
		File python\_freesteam.pyd
		File python\freesteam.py

		StrCpy $PYINSTALLED "1"
		WriteRegDWORD HKLM "SOFTWARE\freesteam" "Python" 1	
	${Else}
		MessageBox MB_OK "Python bindings can not be installed, because Python was not found on this system. ($PYPATH)"
	${EndIf}
	Return
SectionEnd

Section "ASCEND hooks"
	${If} $ASCENDOK == 'OK'
		DetailPrint "--- ASCEND HOOKS ---"
		SetOutPath $INSTDIR\ascend
		File ascend\*.a4c
		File ascend\freesteam_ascend.dll
		StrCpy $ASCENDINSTALLED "1"
		WriteRegDWORD HKLM "SOFTWARE\freesteam" "ASCEND" 1
	${Else}
		MessageBox MB_OK "ASCEND hooks can not be installed, because ASCEND was not found on this system. If you do want to use the ASCEND hooks, please check the installation instructions ($ASCENDPATH)"
	${EndIf}

	${If} $ASCENDMODELSOK = 'OK'
		${EnvVarUpdate} $0 "ASCENDLIBRARY" "P" "HKLM" "$ASCENDMODELS"
	${EndIf}

	${EnvVarUpdate} $0 "ASCENDLIBRARY" "A" "HKLM" "$INSTDIR\ascend"

	Return
SectionEnd

Section "Add to PATH"
	${EnvVarUpdate} $0 "PATH" "A" "HKLM" "$INSTDIR"
	WriteRegDWORD HKLM "SOFTWARE\freesteam" "AddedToPATH" 1	
	Return
SectionEnd

;---------------------------------

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"
  
  WriteRegDWORD HKLM "SOFTWARE\freesteam" "StartMenu" 1

  CreateDirectory "$SMPROGRAMS\freesteam"
  CreateShortCut "$SMPROGRAMS\freesteam\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\freesteam\LICENSE.lnk" "$INSTDIR\LICENSE.txt" "" "$INSTDIR\LICENSE.txt" 0
  CreateShortCut "$SMPROGRAMS\freesteam\README.lnk" "$INSTDIR\README.txt" "" "$INSTDIR\README.txt" 0
  CreateShortCut "$SMPROGRAMS\freesteam\CHANGELOG.lnk" "$INSTDIR\CHANGELOG.txt" "" "$INSTDIR\CHANGELOG.txt" 0

  ${If} $EXAMPLEINSTALLED == "1"
	CreateShortCut "$SMPROGRAMS\freesteam\Example code.lnk" "$INSTDIR\example" "" "$INSTDIR\example" 0
  ${EndIf}

  ${If} $PYINSTALLED == "1"
	CreateShortCut "$SMPROGRAMS\freesteam\Python README.lnk" "$INSTDIR\python\README.txt" "" "$INSTDIR\python\README.txt" 0
	CreateShortCut "$SMPROGRAMS\freesteam\Python scripts.lnk" "$INSTDIR\python" "" "$INSTDIR\python" 0
  ${EndIf}
  
  ${If} $ASCENDINSTALLED == "1"
	CreateShortCut "$SMPROGRAMS\freesteam\ASCEND models.lnk" "$INSTDIR\ascend" "" "$INSTDIR\ascend" 0
  ${EndIf}
  
SectionEnd

;------------------------------------------------------------------
; UNINSTALLER

Section "Uninstall"

;--- header files & static lib ---

	ReadRegDWORD $0 HKLM "SOFTWARE\freesteam" "Lib"
	${If} $0 != 0
		Delete "$INSTDIR\freesteam-config"
		Delete "$INSTDIR\freesteam-config.bat"
		RmDir /r "$INSTDIR\include"
		Delete "$INSTDIR\lib\libfreesteam.a"
		Delete "$INSTDIR\lib\freesteam.def"
		RmDir "$INSTDIR\lib"
	${EndIf}

;--- example code ---

	ReadRegDWORD $0 HKLM "SOFTWARE\freesteam" "Example"
	${If} $0 != 0
			RmDir /r $INSTDIR\example
	${EndIf}

;--- python components ---

	ReadRegDWORD $0 HKLM "SOFTWARE\freesteam" "Python"
	${If} $0 != 0
		DetailPrint "--- REMOVING PYTHON COMPONENTS ---"
		Delete $PYPATH\Lib\site-packages\_freesteam.dll
		Delete $PYPATH\Lib\site-packages\freesteam.py*
		Delete $INSTDIR\python\README.txt
		Delete $INSTDIR\python\test.py
		Delete $INSTDIR\python\phdiagram.py
		Delete $INSTDIR\python\pTdiagram.py
		Delete $INSTDIR\python\pudiagram.py
		Delete $INSTDIR\python\rhoTdiagram.py
		Delete $INSTDIR\python\satcurve.py
		Delete $INSTDIR\python\tables.py
		Delete $INSTDIR\python\Thdiagram.py
		Delete $INSTDIR\python\Tsdiagram.py
		
		RmDir $INSTDIR\python
	${EndIf}
	
;--- ASCEND hooks ---

	ReadRegDWORD $0 HKLM "SOFTWARE\freesteam" "ASCEND"
	${If} $0 != 0:
		DetailPrint "--- REMOVING ASCEND HOOKS ---"
		RmDir /r "$INSTDIR\ascend"
		${un.EnvVarUpdate} $0 "ASCENDLIBRARY" "R" "HKLM" "$INSTDIR\ascend"
	${EndIf}

;--- remove from PATH ---

	ReadRegDWORD $1 HKLM "SOFTWARE\freesteam" "AddedToPATH"
	${If} $1 != 0:
		DetailPrint "--- REMOVING FROM PATH ---"
		${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$INSTDIR"  
	${EndIf}

;--- start menu ---

	ReadRegDWORD $1 HKLM "SOFTWARE\freesteam" "StartMenu"
	${If} $1 != 0:
		; Remove shortcuts, if any
		DetailPrint "--- REMOVING START MENU SHORTCUTS ---"
		RmDir /r "$SMPROGRAMS\freesteam"
	${EndIf}

;--- common components ---

	DetailPrint "--- REMOVING COMMON COMPONENTS ---"
	
	; Remove registry keys
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\freesteam"
	DeleteRegKey HKLM "SOFTWARE\freesteam"

	; Remove files and uninstaller
	Delete $SYSDIR\freesteam.dll
	Delete $INSTDIR\LICENSE.txt
	Delete $INSTDIR\README.txt
	Delete $INSTDIR\CHANGELOG.txt
	Delete $INSTDIR\uninstall.exe
	
	; Remove directories used (should be empty now)
	RMDir "$INSTDIR"
	
SectionEnd

;---------------------------------------------------------------------
; UTILITY ROUTINES

Function DetectPython
	ReadRegStr $R6 HKCU "SOFTWARE\Python\PythonCore\${PYVERSION}\InstallPath" ""
	${If} $R6 == ''
		ReadRegStr $R6 HKLM "SOFTWARE\Python\PythonCore\${PYVERSION}\InstallPath" ""
		${If} $R6 == ''
			Push "No registry key found"
			Push "NOK"
			Return
		${EndIf}
	${EndIf}
	
	${If} ${FileExists} "$R6\python.exe"
		Push "$R6"
		Push "OK"
	${Else}
		Push "No python.exe found"
		Push "NOK"
	${EndIf}
FunctionEnd

;---------------------------------------------------------------------
; Detect ASCEND

Function DetectASCEND
	ReadRegStr $R6 HKCU "SOFTWARE\ASCEND" "Install_Dir"
	${If} $R6 == ''
		ReadRegStr $R6 HKLM "SOFTWARE\ASCEND" "Install_Dir"
		${If} $R6 == ''
			Push "No registry key found"
			Push "NOK"
			Return
		${EndIf}
	${EndIf}
	
	${If} ${FileExists} "$R6\ascend-config.bat"
		Push "$R6"
		Push "OK"
	${Else}
		Push "No ascend-config.bat found"
		Push "NOK"
	${EndIf}
FunctionEnd

Function DetectASCENDModels
	ReadRegStr $R6 HKCU "SOFTWARE\ASCEND" "INSTALL_MODELS"
	${If} $R6 == ''
		ReadRegStr $R6 HKLM "SOFTWARE\ASCEND" "INSTALL_MODELS"
		${If} $R6 == ''
			Push "No INSTALL_MODELSregistry key found"
			Push "NOK"
			Return
		${EndIf}
	${EndIf}
	
	${If} ${FileExists} "$R6\system.a4l"
		Push "$R6"
		Push "OK"
	${Else}
		Push "No 'system.a4l' found"
		Push "NOK"
	${EndIf}
FunctionEnd

