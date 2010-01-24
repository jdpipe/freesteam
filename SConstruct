import os, os.path, commands, platform, sys
import distutils.sysconfig

version = "0.8.2"

pyversion = "%d.%d" % (sys.version_info[0],sys.version_info[1])

if platform.system()=="Windows":
	soname_major = ""
	soname_minor = ""
else:
	soname_major = ".0"
	soname_minor = ".0"

soname_default = "${SHLIBPREFIX}freesteam${SHLIBSUFFIX}${SONAME_MAJOR}"

#----------------------------------------------------
# CREATE ENVIRONMENT, LOAD TOOLS, SOME DEFAULT PATHS

can_install=True

# Should exist in path
cppunit_config_command = 'cppunit-config --libs --cflags'

tools = ['swig','disttar','nsis','substinfile']
if os.environ.has_key('OSTYPE') and os.environ['OSTYPE']=='msys':
	env = Environment(ENV=os.environ
		, toolpath = ['scons']
		, tools = ['mingw']+tools
	)
	if env.WhereIs('cppunit-config'):
		cppunit_config_command = 'sh cppunit-config --libs --cflags'
	else:
		cppunit_config_command = None
else:
	env = Environment(ENV=os.environ
		, toolpath = ['scons']
		, tools = ['default'] + tools
	)

	if platform.system()=="Windows":
		if env.has_key('MSVS'):
			# for exception handling, following errmsg from VC6
			env.Append(CXXFLAGS=['/EHsc'])
			env.Append(CPPDEFINES='UNITS_CAST_THROW');
			print "VISUAL C++ detected... (Version",env['MSVS']['VERSION']+")"
		else:
			print "WARNING: non-POSIX tools"
		env.AppendUnique(CPPPATH=[os.environ['INCLUDE'].split(";")])
		can_install=False
	
if platform.system()=="Windows":
	default_emso_location = "c:\\Program Files\\EMSO\\interface"
	default_prefix = 'c:/MinGW'
else:
	default_emso_location = None
	default_prefix = '/usr/local'

if not default_emso_location or not os.path.exists(default_emso_location):
	default_emso_location=False

# Find 'ascend-config' script, if available
if platform.system()=='Windows':
	# TODO should really get ascend-config location from Registry...
	default_ascend_config=None

else:
	try:
		default_ascend_config = env.WhereIs("ascend-config")
	except Exception, e:
		default_ascend_config = None

#------------------------------------------------------
# OPTIONS

opts = Options(['options.cache', 'config.py'])

opts.Add(PackageOption(
	'EMSO_INCDIR',"Location of EMSO 'interface' directory", default_emso_location
))
opts.Add(BoolOption(
	'HAVE_EMSO',"Build EMSO hook libraries", False
))

if can_install:
	opts.Add(PathOption(
		'INSTALL_PREFIX'
		,"Base directory for install, see also INSTALL_LIB and INSTALL_INCLUDE"
		,default_prefix
	))

	opts.Add(
		'LIBDIR'
		,"Location to install library"
		, "$INSTALL_PREFIX/lib"
	)

	opts.Add(
		'INCDIR'
		,"Location to install headers"
		,"$INSTALL_PREFIX/include"
	)

	opts.Add(
		'INSTALL_BIN'
		,"Location to install binaries"
		,"$INSTALL_PREFIX/bin"
	)
	
	opts.Add(PathOption(
		'INSTALL_ROOT'
		,"Root onto which installation should take place. Normally only for "
			+"use when building RPMs"
		,None
	))

opts.Add(BoolOption(
	'STATIC_LIBRARY'
	,"Build freesteam as a static library"
	,False
))

opts.Add(BoolOption(
	'ENABLE_SAT_P'
	, "Enable saturation curves in terms of pressure"+
		"(introduces some numerical problems in iterative cases if"+
		"you're not careful)"
	, True
))

opts.Add(BoolOption(
	'ENABLE_DEBUG_FLAG',"Add debug symbols to object code, for use with gdb etc", False
))

opts.Add(BoolOption(
	'WITH_CTESTS'
	,"Perform compile-time testing of the units of measurement code"
	,True
))

opts.Add(PathOption(
	'ASCEND_CONFIG'
	,"Location of the 'ascend-config' script"
	,default_ascend_config
))

opts.Add(PathOption(
	'ASCEND_SRCROOT'
	,"Location of top-level ASCEND source directory. Use this if ASCEND is not"
		+"actually installed on your system (eg you are testing/developing)."
	,None
))

opts.Add(
	'DISTTAR_NAME'
	,"Stem name of the tarball created by 'scons dist'."
	,"freesteam-"+version
)

opts.Add(
	'CXX'
	,"Your C++ compiler"
	,None
)

opts.Add(
	'SONAME_MAJOR'
	,"major version of the freesteam library"
	,soname_major
)

opts.Add(
	'SONAME_MINOR'
	,"Shared library minor version number (for use in installed file name). Should be '.0' for example."
	,soname_minor
)

opts.Add(
	'SONAME'
	,"'soname' to be assigned to the shared library. Should be 'freesteam.so.1' for example."
	,soname_default
)

if platform.system()=="Windows":
	opts.Add(
		'WIN_INSTALLER_NAME'
		,"Name of the installer .exe to create under Windows (minus the '.exe')"
		,"freesteam-"+version+"-py"+pyversion+".exe"
	)

opts.Update(env)

opts.Save('options.cache',env)

Help(opts.GenerateHelpText(env))

#-------------
# Apply the options to the environment

if env['ENABLE_SAT_P']:
	env.Append(CPPDEFINES=['ENABLE_SAT_P'])

if env['ENABLE_DEBUG_FLAG']:
	env.Append(CXXFLAGS=['-g'])
	env.Append(CFLAGS=['-g'])

#------------------------------------------------------
# CONFIGURATION TESTS

#-----------
# ISNAN DETECTION

#- - - 8< - - -
checknansrc = """/* Test for 'isnan' */
#include <math.h>

int main(void){
	int x = 1;
	int y;
	y = isnan(x);
	return 0;
}
#ifdef __MINGW32__
# error "MINGW32 isnan is no good for our purpose - a macro isnan is no good"
#endif
"""
#- - - 8< - - -

def CheckIsNan(context):
	context.Message( 'Checking for isnan...' )
	lastLIBS = context.env.get('LIBS')
	lastCPPFLAGS = context.env.get('CPPFLAGS')
	context.env.Append(LIBS = ['m'])
	tuple = context.TryRun(checknansrc,'.c')
	#print tuple
	if not tuple[0]:
		context.env.Replace(
			LIBS = lastLIBS
			, CPPFLAGS = lastCPPFLAGS
		)
		context.config_h = context.config_h +  "#undef HAVE_ISNAN\n"
	else:
		context.config_h = context.config_h +  "#define HAVE_ISNAN\n"

	context.Result(tuple[0])
	return tuple[0]

def CheckCppUnitConfig(context):
	context.Message( 'Checking cppunit-config...' )
	lastLIBS = context.env.get('LIBS')
	lastCPPFLAGS = context.env.get('CPPFLAGS')

#	for k in conf.env['ENV']:
#		print "ENV:",k,"=",env['ENV'][k]

	res = 1
	try:
		context.env.ParseConfig(cppunit_config_command)
	except IOError:
		res = 0
	except:
		res = 0

	if lastLIBS:
		context.env.Replace(LIBS=lastLIBS)
	else:
		context.env['LIBS'] = []

	if lastCPPFLAGS:
		context.env.Replace(CPPFLAGS=lastCPPFLAGS)
	else:
		context.env['CPPFLAGS'] = []
			
	context.config_h = context.config_h + "#define HAVE_CPPUNIT_CONFIG\n"
	context.Result(res)
	return res

#----------------
# SWIG

import os,re

need_fortran = False

def get_swig_version(env):
	cmd = env['SWIG']+' -version'
	(cin,coutcerr) = os.popen4(cmd)
	output = coutcerr.read()
	
	restr = "SWIG\\s+Version\\s+(?P<maj>[0-9]+)\\.(?P<min>[0-9]+)\\.(?P<pat>[0-9]+)\\s*$"
	expr = re.compile(restr,re.M);
	m = expr.search(output);
	if not m:
		return None
	maj = int(m.group('maj'))
	min = int(m.group('min'))
	pat = int(m.group('pat'))

	return (maj,min,pat)
	

def CheckSwigVersion(context):
	
	try:
		context.Message("Checking version of SWIG... ")
		maj,min,pat = get_swig_version(context.env)
	except:
		context.Result("Failed to detect version, or failed to run SWIG")
		return 0;
	
	if maj == 1 and (
			min > 3
			or (min == 3 and pat >= 24)
		):
		context.Result("ok, %d.%d.%d" % (maj,min,pat))
		return 1;
	else:
		context.Result("too old, %d.%d.%d" % (maj,min,pat))
		return 0;

#------------------------------------------------------
# ASCEND
 
def CheckAscend(context):
		context.Message("Checking for ascend-config... ")
		
		if context.env.get("ASCEND_CONFIG"):
			loc = "path"
			path = str(context.env.get("ASCEND_CONFIG"))
		elif platform.system()=="Windows":
			loc = "registry"	
			try:
				import _winreg
				x=_winreg.ConnectRegistry(None,_winreg.HKEY_LOCAL_MACHINE)
				y= _winreg.OpenKey(x, r'SOFTWARE\ASCEND' )
			except Exception,e:
				context.Result("failed registry check (%s)" % str(e))
				return 0
			BIN,_t = _winreg.QueryValueEx(y,"INSTALL_BIN")
			_winreg.CloseKey(y)
			_winreg.CloseKey(x)
			path = os.path.join(BIN,"ascend-config")
		else:
			context.Result("not found")
			return 0				
		
		if not os.path.isfile(path):
			context.Result("not found at '%s' (from %s)" % (str(path),loc))
			return 0

		context.env['ASCEND_CONFIG'] = path
		context.Result("ok, %s" % str(path))
		return 1
#	except Exception,e:
#		context.Result("failed (%s)" % str(e))
#		return 0

#------------------------------------------------------
# CONFIGURATION


conf = Configure(env
	, custom_tests = { 
		'CheckIsNan' : CheckIsNan
		,'CheckCppUnitConfig' : CheckCppUnitConfig
		,'CheckSwigVersion' : CheckSwigVersion
		,'CheckAscend' : CheckAscend
	} 
	, config_h = "config.h"
)

# Math library...

if conf.CheckLibWithHeader(['m','c'], 'math.h', 'C') == False:
	print 'Did not find libm.a or m.lib, exiting!'
	Exit(1)

# IsNan

conf.CheckIsNan()
# CppUnit...

have_cppunit = False

if conf.CheckCppUnitConfig():
	if conf.CheckLibWithHeader("cppunit","cppunit/TestCase.h","C++"):
		#print "CPPUNIT OK!"
		have_cppunit=True

# Python DEV files

env['HAVE_PYTHON'] = False
_havepy = conf.CheckHeader(distutils.sysconfig.get_python_inc()+"/Python.h")
if _havepy:
	env['HAVE_PYTHON'] = True

# Swig

env['HAVE_SWIG'] = False
_swigv = conf.CheckSwigVersion()
if _swigv:
	env['HAVE_SWIG'] = True

# ASCEND

if conf.CheckAscend():
	env['HAVE_ASCEND'] = True

env = conf.Finish()

# install

if platform.system()=="Windows":
	with_installer=1
else:
	with_installer=0
	without_installer_reason = "only possible under Windows"

#------------------------------------------------------
# BUILD THE STATIC OR SHARED LIBRARY


list = Split("""
	exception.cpp steamcalculator.cpp state.cpp region1.cpp region2.cpp 
	region3.cpp region4.cpp boundaries.cpp steamcalculatorexception.cpp zeroin.cpp 
	designbycontract.cpp units.cpp solver.cpp steamproperty.cpp satcurve.cpp b23curve.cpp 
	b13curve.cpp convergencetest.cpp solver2.cpp iapws95.cpp measurement.cpp
	surfacetension.cpp
	backwards.c
""")

lib_env = env.Clone()

if platform.system()=="Linux":
	lib_env.Append(LINKFLAGS=['-Wl,-soname,$SONAME'])

if lib_env.get('STATIC_LIBRARY'):
	lib = lib_env.Library('freesteam',list
		, LIBS = ['m']
	)
	libs = [lib]
else:
	lib = lib_env.SharedLibrary('freesteam',list
		, LIBS = ['m']
	)

	libs = [lib]

	# create local symlink for the soname stuff.
	if platform.system()=="Linux":
		if env.get('SONAME_MAJOR'):
			lib_env.Command("${SHLIBPREFIX}freesteam${SHLIBSUFFIX}${SONAME_MAJOR}",lib,Move("$TARGET","$SOURCE"))
			print "MAKING LINK, SONAME_MAJOR =",env.get('SONAME_MAJOR')
			liblink = lib_env.Command("${SHLIBPREFIX}freesteam${SHLIBSUFFIX}${SONAME_MAJOR}"
				,lib
				,"ln -s $SOURCE $TARGET"
			)
			libs.append(liblink)

#------------------------------------------------------
# Build the example program (build-time)

examplesrc = Split("example/example.cpp")

example = env.Program('example/example.cpp'
	, LIBS = ['freesteam']
	, LIBPATH='#'
	, CPPPATH='#'
	, CPPDEFINES=['FREESTEAM_NOT_INSTALLED']
)

env.Depends(example,libs)

env.Alias('example',[example])

#env.Program('testnan',['testnan.cpp']
#	, LIBS = 'm'
#	, CPPFLAGS = '-fsignaling-nans'
#)

#------------------------------------------------------
# SUBDIRECTORIES....

# Testing with CppUnit...

if have_cppunit:
	env.SConscript(['test/SConscript'],'env have_cppunit') #, 'cli/SConscript'
else:
	print "Skipping... Tests (no CppUnit found)"

# Compile-time C++ template testing

if env['WITH_CTESTS']:
	env.SConscript(['ctest/SConscript'],'env')

# Command-line-interface programs

env.SConscript(['cli/SConscript'],'env')

# EMSO hooks

if env['HAVE_EMSO']:
	env.SConscript(['emso/SConscript'],'env')
else:
	print "Skipping... EMSO hook libraries"

# Python hooks

install_python = []
install_python.append( env.SConscript(['python/SConscript'],'env') )

# ASCEND hooks

install_ascend = []
install_ascend.append( env.SConscript(['ascend/SConscript'],'env') )

#------------------------------------------------------
# Recipe for 'CHMOD' ACTION 	 
  	 
import SCons 	 
from SCons.Script.SConscript import SConsEnvironment 	 
SConsEnvironment.Chmod = SCons.Action.ActionFactory(os.chmod, 	 
	lambda dest, mode: 'Chmod("%s", 0%o)' % (dest, mode)) 	 
  	 
def InstallPerm(env, dest, files, perm): 	 
	obj = env.Install(dest, files) 	 
	for i in obj: 	 
		env.AddPostAction(i, env.Chmod(str(i), perm))
	return dest

def InstallPermAs(env, dest, filen, perm): 	 
	obj = env.InstallAs(dest, filen) 	 
	for i in obj: 	 
		env.AddPostAction(i, env.Chmod(str(i), perm))
	return dest
  	 
SConsEnvironment.InstallPerm = InstallPerm

# define wrappers 	 
SConsEnvironment.InstallProgram = lambda env, dest, files: InstallPerm(env, dest, files, 0755) 	 
SConsEnvironment.InstallHeader = lambda env, dest, files: InstallPerm(env, dest, files, 0644) 	 
SConsEnvironment.InstallLibrary = lambda env, dest, files: InstallPerm(env, dest, files, 0644) 	 
SConsEnvironment.InstallLibraryAs = lambda env, dest, files: InstallPermAs(env, dest, files, 0644) 	 

#------------------------------------------------------
# Symlink

SConsEnvironment.SymLink = Builder(action='ln -s $SOURCE $TARGET')

#------------------------------------------------------
# DOXYGEN DOCUMENTATION

#docs = env.Doxygen('Doxyfile')
#env.Alias('docs',[docs])

#---------------------------------------
# SUBSTITUTION DICTIONARY for .in files

subst_dict = {
	'@VERSION@':version
	,'@SONAME@':env['SONAME']
	,'@SONAME_MAJOR@':env['SONAME_MAJOR']
	,'@SONAME_MINOR@':env['SONAME_MINOR']
	,'@SHLIBSUFFIX@':env['SHLIBSUFFIX']
	,'@SHLIBPREFIX@':env['SHLIBPREFIX']
	,'@INSTALL_BIN@':env['INSTALL_BIN']
	,'@INSTALL_LIB@':env['LIBDIR']
	,'@INSTALL_INCLUDE@':env['INCDIR']
	,'@PYTHON@':sys.executable
}

env.Append(SUBST_DICT=subst_dict)

#------------------------------------------------------
# CREATE the SPEC file for generation of RPM packages

env.SubstInFile('freesteam.spec.in')
	
#------------------------------------------------------
# CREATE 'freesteam-config'

configscript = env.SubstInFile('freesteam-config.in')

#------------------------------------------------------
# INSTALLATION

try:
	umask = os.umask(022)
except OSError:
	# ignore on systems that don't support umask
	pass

if can_install:
	install_common = []
	
	libname = "$LIBDIR/$SONAME$SONAME_MINOR"
	if platform.system()=="Windows":
		install_lib = env.InstallLibrary("$INSTALL_ROOT$LIBDIR",[lib])
	else:
		install_lib = env.InstallLibraryAs("$INSTALL_ROOT"+libname, [lib])

		link1 = "$LIBDIR/${SHLIBPREFIX}freesteam$SHLIBSUFFIX"
		link2 = "$LIBDIR/$SONAME"

		install_link1 = None
		if env.subst(link1) != env.subst(libname):
			install_link1 = env.Command("${INSTALL_ROOT}"+link1,install_lib,"ln -s %s $TARGET" % libname)

		install_link2 = None
		if env.get("SONAME_MINOR"):
			install_link2 = env.Command("${INSTALL_ROOT}"+link2,install_lib,"ln -s %s $TARGET"%libname)
		
		install_common += [install_link1, install_link2]

	install_config = env.InstallProgram("$INSTALL_ROOT$INSTALL_BIN",[configscript])
	install_common += [install_lib,install_config]
	
	import glob
	files = glob.glob("*.h")

	if env.get('HAVE_SWIG') and env.get('HAVE_PYTHON'):
		files += glob.glob("*.i")

	#print files

	install_common += [env.InstallHeader("$INSTALL_ROOT$INCDIR/freesteam", files)]
		
	env.Alias('install',install_common + install_ascend + install_python)

#------------------------------------------------------
# WINDOWS INSTALLER

if not env.get('NSIS'):
	with_installer = False
	without_installer_reason = "NSIS not found"

if platform.system()=="Windows":
	if with_installer:
		env.Append(NSISDEFINES={
			'OUTFILE':"#dist/"+env['WIN_INSTALLER_NAME']
			,"VERSION":version
			,'PYVERSION':pyversion
		})
		installer = env.Installer('installer.nsi')
		Depends(installer,["python","ascend"])
		Depends(installer,[configscript])
		env.Alias('installer',installer)
	else:
		print "Skipping... Windows installer isn't being built:",without_installer_reason

#--------------------------------------------------------
# DISTRIBUTION TARBALL

env['DISTTAR_FORMAT']='bz2'
env.Append(
	DISTTAR_EXCLUDEEXTS=['.o','.os','.so','.a','.dll','.cc','.cache','.pyc','.cvsignore','.dblite','.log','.pl','.cli','.cpass','.0','.bak']
	, DISTTAR_EXCLUDEDIRS=['CVS','.svn','.sconf_temp', 'dist']
)

if env.get('DISTTAR_NAME'):
	tar = env.DistTar("dist/"+env.get('DISTTAR_NAME')
		, [env.Dir('#')]
	)
	env.Depends(tar,"freesteam.spec")

#--------------------------------------------------------
# DEFAULT TARGETS

default_targets = libs + ['example','cli','test']

if env.get('HAVE_PYTHON') and env.get('HAVE_SWIG'):
	default_targets.append('python')

#if env.get('HAVE_ASCEND'):
# above test doesn't work for case of ASCEND_SRCROOT
default_targets.append('ascend')

if env.get('NSIS'):
	default_targets.append('installer')

env.Default(default_targets)

print "Building targets:"," ".join([str(i) for i in BUILD_TARGETS])

# vim: set syntax=python:

