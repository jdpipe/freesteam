; NSIS script to create an freesteam binary installer.
; by John Pye, 2006.
;
; Based on example2.nsi from the NSIS distribution.
;
; This script is based on example1.nsi, but it remembers the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer

Name "freesteam"

!include LogicLib.nsh

; The file to write
!ifdef OUTFILE
OutFile ${OUTFILE}.exe
!else
OutFile freesteam-setup.exe
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
Var /GLOBAL GTKOK
Var /GLOBAL GTKPATH
Var /GLOBAL GLADEOK
Var /GLOBAL GLADEPATH
Var /GLOBAL ASCENDOK
Var /GLOBAL ASCENDPATH

Var /GLOBAL PYINSTALLED
Var /GLOBAL ASCENDINSTALLED
Var /GLOBAL LIBINSTALLED

Function .onInit
	StrCpy $PYINSTALLED ""
	StrCpy $ASCENDINSTALLED ""
	StrCpy $LIBINSTALLED ""
	
	Call DetectPython
	Pop $PYOK
	Pop $PYPATH
	
	Call DetectGTK
	Pop $GTKOK
	Pop $GTKPATH

	Call DetectGlade
	Pop $GLADEOK
	Pop $GLADEPATH
	
	Call DetectASCEND
	Pop $ASCENDOK
	Pop $ASCENDPATH
		
FunctionEnd


; The stuff to install
Section "freesteam (required)"
  SectionIn RO

  DetailPrint "--- COMMON FILES ---"
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  File "LICENSE.txt"
  File "README.html"
  File "CHANGELOG.txt"
    
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

Section "Static library & header files"
  DetailPrint "--- STATIC LIBRARY & HEADER FILES ---"
	SetOutPath $INSTDIR\include
	File "libfreesteam.a"
	File "*.h"
	WriteRegDWORD HKLM "SOFTWARE\freesteam" "Lib" 1
	StrCpy $LIBINSTALLED "1"
	Return
SectionEnd
	
Section "Python language bindings"
	${If} $PYOK == 'OK'
		;MessageBox MB_OK "Python: $PYPATH, GTK: $GTKPATH"

		DetailPrint "--- PYTHON INTERFACE ---"

		; Set output path to the installation directory.
		SetOutPath $PYPATH\Lib\site-packages

		; Python interface
		File python\_freesteam.dll
		File python\freesteam.py

		StrCpy $PYINSTALLED "1"
		WriteRegDWORD HKLM "SOFTWARE\freesteam" "Python" 1	
	${Else}
		MessageBox MB_OK "Python bindings can not be installed, because Python was not found on this system. If you do want to use the PyGTK GUI, please check the installation instructions ($PYPATH)"
	${EndIf}
	Return
SectionEnd

Section "ASCEND hooks"
	${If} $ASCENDOK == 'OK'
		DetailPrint "--- ASCEND HOOKS ---"
		SetOutPath $INSTDIR\ascend
		File ascend\*.a4c
		File ascend\freesteam.dll
		StrCpy $ASCENDINSTALLED "1"
		WriteRegDWORD HKLM "SOFTWARE\freesteam" "ASCEND" 1
	${Else}
		MessageBox MB_OK "ASCNED hooks can not be installed, because ASCEND was not found on this system. If you do want to use the ASCEND hooks, please check the installation instructions ($ASCENDPATH)"
	${EndIf}
	Return
SectionEnd

;---------------------------------

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"
  
  WriteRegDWORD HKLM "SOFTWARE\freesteam" "StartMenu" 1

  CreateDirectory "$SMPROGRAMS\freesteam"
  CreateShortCut "$SMPROGRAMS\freesteam\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\freesteam\LICENSE.lnk" "$INSTDIR\LICENSE.txt" "" "$INSTDIR\LICENSE.txt" 0
  CreateShortCut "$SMPROGRAMS\freesteam\README.lnk" "$INSTDIR\README.html" "" "$INSTDIR\README.html" 0
  CreateShortCut "$SMPROGRAMS\freesteam\CHANGELOG.lnk" "$INSTDIR\CHANGELOG.txt" "" "$INSTDIR\CHANGELOG.txt" 0

SectionEnd

;------------------------------------------------------------------
; UNINSTALLER

Section "Uninstall"

;--- header files & static lib ---

	ReadRegDWORD $0 HKLM "SOFTWARE\freesteam" "Lib"
	IntCmp $0 0 unnolib unlib

unlib:
	Delete $INSTDIR\libfreesteam.a
	RmDir /r $INSTDIR\include

unnolib:

;--- python components ---

	ReadRegDWORD $0 HKLM "SOFTWARE\freesteam" "Python"
	IntCmp $0 0 unnopython unpython
  
unpython:
	DetailPrint "--- REMOVING PYTHON COMPONENTS ---"
	Delete $PYPATH\Lib\site-packages\_freesteam.dll
	Delete $PYPATH\Lib\site-packages\freesteam.py*

unnopython:

;--- ASCEND hooks ---

	ReadRegDWORD $0 HKLM "SOFTWARE\freesteam" "ASCEND"
	IntCmp $0 0 unnoascend unascend
  
unascend:
	DetailPrint "--- REMOVING ASCEND HOOKS ---"
	RmDir /r "$INSTDIR\ascend"
	
unnoascend:
	

;--- start menu ---

	ReadRegDWORD $1 HKLM "SOFTWARE\freesteam" "StartMenu"
	IntCmp $1 0 unnostart unstart 
unstart:
	; Remove shortcuts, if any
	DetailPrint "--- REMOVING START MENU SHORTCUTS ---"
	RmDir /r "$SMPROGRAMS\freesteam"

unnostart:

;--- common components ---

	DetailPrint "--- REMOVING COMMON COMPONENTS ---"
	; Remove registry keys

	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\freesteam"
	DeleteRegKey HKLM "SOFTWARE\freesteam"

	; Remove files and uninstaller

	Delete $INSTDIR\LICENSE.txt
	Delete $INSTDIR\README.html
	Delete $INSTDIR\CHANGELOG.txt
	Delete $INSTDIR\uninstall.exe
	
	
	; Remove directories used
	RMDir "$INSTDIR"

SectionEnd

;---------------------------------------------------------------------
; UTILITY ROUTINES

Function DetectPython
	ReadRegStr $R6 HKCU "SOFTWARE\Python\PythonCore\2.4\InstallPath" ""
	${If} $R6 == ''
		ReadRegStr $R6 HKLM "SOFTWARE\Python\PythonCore\2.4\InstallPath" ""
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

;--------------------------------------------------------------------
; Prefer the current user's installation of GTK, fall back to the local machine

Function DetectGTK
	ReadRegStr $R6 HKCU "SOFTWARE\GTK\2.0" "DllPath"
	${If} $R6 == ''
		ReadRegStr $R6 HKLM "SOFTWARE\GTK\2.0" "DllPath"
		${If} $R6 == ''
			Push "No GTK registry key found"
			Push "NOK"
			Return
		${EndIf}
	${EndIf}

	${If} ${FileExists} "$R6\libgtk-win32-2.0-0.dll"
		Push "$R6"
		Push "OK"
	${Else}
		Push "No libgtk-win32-2.0-0.dll found in'$R6'"
		Push "NOK"
	${EndIf}
FunctionEnd

;--------------------------------------------------------------------
; Prefer the current user's installation of GTK, fall back to the local machine

Function DetectGlade
	ReadRegStr $R6 HKCU "SOFTWARE\GTK\2.0" "DllPath"
	${If} $R6 == ''
		ReadRegStr $R6 HKLM "SOFTWARE\GTK\2.0" "DllPath"
		${If} $R6 == ''
			Push "No GTK registry key found"
			Push "NOK"
			Return
		${EndIf}
	${EndIf}

	${If} ${FileExists} "$R6\libglade-2.0-0.dll"
		Push "$R6"
		Push "OK"
	${Else}
		Push "No libglade-2.0-0.dll found in'$R6'"
		Push "NOK"
	${EndIf}
FunctionEnd
