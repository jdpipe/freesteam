# SCONS tools to detect GSL on Windows and Linux
# For windows, we assume that GSL-1.11 for MinGW is
# being used, see here:
# http://ascendwiki.cheme.cmu.edu/Binary_installer_for_GSL-1.11_on_MinGW
# For Linux, we assume standard Linux packages are used,
# and hence that gsl-config will be present on the PATH.

import os, os.path, platform, subprocess
from SCons.Script import *

munge = lambda s: s

try:
	# if we have access to GetShortPathName, we'll use it...
	import win32api
	def munge1(s):
		s1 = s
		try:
			# we can only munge the path if it actually exists
			s1 = win32api.GetShortPathName(s)
		except:
			# if it doesn't exist, we just return the un-munged path
			pass
		return s1
	munge = munge1 
except:
	pass

def generate(env):
	"""
	Detect GSL settings and add them to the environment.
	"""
	try:
		if platform.system()=="Windows":
			import _winreg
			x=_winreg.ConnectRegistry(None,_winreg.HKEY_LOCAL_MACHINE)
			y= _winreg.OpenKey(x,r"SOFTWARE\gsl")
			BIN,t = _winreg.QueryValueEx(y,"INSTALL_BIN")
			LIB,t = _winreg.QueryValueEx(y,"INSTALL_LIB")
			INCLUDE,t = _winreg.QueryValueEx(y,"INSTALL_INCLUDE")
			
			env['GSL_CPPPATH'] = [munge(INCLUDE)]
			env['GSL_LIBPATH'] = [munge(LIB)]
			env['GSL_LIBS'] = ['gslcblas','gsl']
			env['HAVE_GSL'] = True
									
		else:
			cmd = ['gsl-config','--cflags','--libs']
			env1 = env.Clone()
			env1['CPPPATH'] = None
			env1['LIBPATH'] = None
			env1['LIBS'] = None
			env1.ParseConfig(cmd)
			env['GSL_CPPPATH'] = env1.get('CPPPATH')
			env['GSL_LIBPATH'] = env1.get('LIBPATH')
			env['GSL_LIBS'] = env1.get('LIBS')
			env['HAVE_GSL'] = True

		print "GSL_LIBS =",env.get('GSL_LIBS')
		print "GSL_LIBPATH =",env.get('GSL_LIBPATH')
		print "GSL_CPPPATH =",env.get('GSL_CPPPATH')

	except:
		env['HAVE_GRAPHVIZ'] = False

def exists(env):
	"""
	Make sure this tool exists (and that header files are installed).
	"""
	if platform.system()=="Windows":
		try:
			import _winreg
			x=_winreg.ConnectRegistry(None,_winreg.HKEY_LOCAL_MACHINE)
			y= _winreg.OpenKey(x,r"SOFTWARE\gsl")
			INCLUDE,t = _winreg.QueryValueEx(y,'INSTALL_INCLUDE')
			return True
		except:
			return False
	else:
		if not subprocess.call('pkg-config libgvc libagraph --exists'):
			return True
		return False

