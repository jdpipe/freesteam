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
			env['HAVE_GSL'] = True		
			if env.get('GSL_STATIC'):
				env['GSL_STATICLIBS'] = [os.path.join(munge(LIB),"lib%s.a"%i) for i in ["gsl","gslcblas"]]
				# not sure when the following is needing, so ignoring for now:
				#env['GSL_LIBS_CBLAS'] = env['GSL_LIBS'] + [os.path.join(munge(LIB),"libgslcblas.a")]
			else:
				env['GSL_LIBS'] = ['gsl']
				# not sure when the following is needing, so ignoring for now:
				#env['GSL_LIBS_CBLAS'] = ['gslcblas','gsl']
		else:
			cmd = ['gsl-config','--cflags','--libs']
			old_env = env.Clone()
			env.ParseConfig(cmd)
			env['GSL_CPPPATH'] = env.get('CPPPATH')
			env['GSL_LIBPATH'] = env.get('LIBPATH')
			env['GSL_LIBS'] = env.get('LIBS')
			for i in ['LIBS','LIBPATH','CPPPATH']:
				if old_env.get(i) is None:
					if env.has_key(i):
						del env[i]
				else:
					env[i] = old_env[i]
			env['HAVE_GSL'] = True

			# on Ubuntu, remove gslcblas if present 
			if platform.system()=="Linux":
				if platform.dist()[0]=="Ubuntu":
					if 'gslcblas' in env['GSL_LIBS']:
						env['GSL_LIBS'].remove('gslcblas')

		#print "GSL_LIBS =",env.get('GSL_LIBS')
		#print "GSL_LIBPATH =",env.get('GSL_LIBPATH')
		#print "GSL_CPPPATH =",env.get('GSL_CPPPATH')

	except Exception,e:
		print "Checking for GSL... not found! (%s)" % str(e)
		env['HAVE_GSL'] = False

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
# vim: set ts=4 noexpandtab:

