del MakeRelease.log
mingw32-make -f Makefile-win32.mak clean >> MakeRelease.log
mingw32-make -f Makefile-win32.mak >> MakeRelease.log

mingw32-make -f Makefile-win64.mak clean >> MakeRelease.log
mingw32-make -f Makefile-win64.mak >> MakeRelease.log
pause
