import os, platform,sys,subprocess
from SCons.Script import *

def generate(env):
	"""
	Detect ASCEND settings and add them to the environment.
	"""
	try:
		if platform.system()=="Windows":
			pathsep = os.environ['PATH'].split(";")
			pp = [os.path.abspath(os.path.join(os.path.expanduser(p),"ascend-config")) for p in pathsep]
			Path = None
			for p in pp:
				if os.path.exists(p):
					#print "pkg-config at",p
					Path = p
			if not Path:
				raise RuntimeError("Could not find 'pkg-config.exe' in your PATH")
				
			#print "PATH =",Path
			cmd = [Path,'--cflags','--libs']
			env1 = env.Copy()
			env1.ParseConfig(cmd)
			env.Append(
				ASCEND_CPPPATH = env1.get('CPPPATH') or []
				,ASCEND_CPPDEFINES = env1.get('CPPDEFINES') or []
				,ASCEND_LIBPATH = env1.get('LIBPATH') or []
				,ASCEND_LIBS = env1.get('LIBS') or []
			)
		else:
			cmd = ['ascend-config','--libs','--cppflags','--libs']
			env1 = env.Clone()
			env1.ParseConfig(cmd)
			env.Append(
				ASCEND_CPPPATH = env1.get('CPPPATH') or []
				,ASCEND_CPPDEFINES = env1.get('CPPDEFINES') or []
				,ASCEND_LIBPATH = env1.get('LIBPATH') or []
				,ASCEND_LIBS = env1.get('LIBS') or []
			)

			cmd = ['ascend-config']
			proc = subprocess.Popen(cmd+['--extlib-prefix'],stdout=subprocess.PIPE)
			out = proc.communicate()[0]
			env.Append(ASCEND_EXTLIB_PREFIX=out.strip())
			proc = subprocess.Popen(cmd+['--extlib-suffix'],stdout=subprocess.PIPE)
			out = proc.communicate()[0]
			env.Append(ASCEND_EXTLIB_SUFFIX=out.strip())

		env.Append(HAVE_ASCEND=True)

		#print "ASCEND_LIBS =",env.get('ASCEND_LIBS')
		#print "ASCEND_LIBPATH =",env.get('ASCEND_LIBPATH')
		#print "ASCEND_CPPPATH =",env.get('ASCEND_CPPPATH')
		#print "ASCEND_EXTLIB_SUFFIX =",env.get('ASCEND_EXTLIB_SUFFIX')
		#print "ASCEND_EXTLIB_PREFIX =",env.get('ASCEND_EXTLIB_PREFIX')

	except Exception,e:
		print "FAILED TO SET UP ASCEND (%s)" % str(e)
		pass

def exists(env):
	"""
	Make sure this tool exists.
	"""
	if platform.system()=="Windows":
		try:
			import _winreg
			x=_winreg.ConnectRegistry(None,_winreg.HKEY_LOCAL_MACHINE)
			y= _winreg.OpenKey(x,r"SOFTWARE\ASCEND")
			INCLUDE,t = _winreg.QueryValueEx(y,'INSTALL_INCLUDE')
			return True
		except:
			return False
	else:
		if env.Execute(['ascend-config','--version']):
			return True
		return False

