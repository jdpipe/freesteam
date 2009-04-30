#!/usr/bin/env python
# This is a build script that makes use of the SCONS build system, see
# http://www.scons.org/. This script is frequently tested on both Windows
# (via MinGW/MSYS) and Linux, and should work in both of these environments.
# We don't currently propose to support building freesteam from MSVS but
# that shouldn't be necessary, as you should be able to use the MinGW-generated
# DLL even with MSVS, because it only uses C code.

#version number for this copy of freesteam
version = "2.0"

# Set up some platform-specific defaults

import platform, sys, distutils.sysconfig
if platform.system()=="Windows":
	default_emso_location = "c:\\Program Files\\EMSO\\interface"
	default_prefix = 'c:/MinGW'
	python_exe = sys.executable
	default_python = distutils.sysconfig.get_python_lib()
else:
	default_emso_location = None
	default_prefix = '/usr/local'
	python_exe = "/usr/bin/env python"
	default_python = distutils.sysconfig.get_python_lib()

# SONAME related flags, for linux shared object versioning. Because this is a
# re-write of freesteam with a new API, we will bump the soname to .1

if platform.system()=="Windows":
	soname_major = ""
	soname_minor = ""
else:
	soname_major = ".1"
	soname_minor = ".0"

soname_default = "${SHLIBPREFIX}freesteam${SHLIBSUFFIX}${SONAME_MAJOR}"

# Configuration options for the build. You can set these on the command-line
# eg using "scons GCOV=1 ..."

opts = Options()

opts.Add(BoolOption(
	'GCOV'
	,"Build for coverage testing using gcov."
	,False
))

opts.Add(BoolOption(
	'DEBUG'
	,"Build with debug data for use with GDB."
	,False
))

opts.Add(PathOption(
	'INSTALL_PREFIX'
	,"Base directory for install, see also INSTALL_LIB and INSTALL_INCLUDE"
	,default_prefix
))

opts.Add(
	'INSTALL_LIB'
	,"Location to install library"
	, "$INSTALL_PREFIX/lib"
)

opts.Add(
	'INSTALL_INCLUDE'
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

opts.Add(PathOption(
	'INSTALL_PYTHON'
	,"Directory for installation of Python extensions"
	,"%s" % default_python
))

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

opts.Add(
	'DISTTAR_NAME'
	,"Stem name of the tarball created by 'scons dist'. So for 'freesteam-aaa.tar.bz2', set this to 'freesteam-aaa'."
	,"freesteam-"+version
)

# Set up the 'tools' the SCONS will need access to , eg compilers
# and create the SCONS 'environment':

import os
tools = ['swig','ascend','substinfile','gsl','tar','disttar']
if os.environ.has_key('OSTYPE') and os.environ['OSTYPE']=='msys':
	env = Environment(ENV=os.environ
		, toolpath = ['scons']
		, tools = ['mingw']+tools
	)
else:
	env = Environment(
		toolpath=['scons']
		,tools=['default']+tools
	)

# Some utility tools for installing file with suitable permissions
  	 
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
SConsEnvironment.InstallProgram = lambda env, dest, files: InstallPerm(env, dest, files, 0755) 	 
SConsEnvironment.InstallHeader = lambda env, dest, files: InstallPerm(env, dest, files, 0644) 	 
SConsEnvironment.InstallLibrary = lambda env, dest, files: InstallPerm(env, dest, files, 0644) 	 
SConsEnvironment.InstallLibraryAs = lambda env, dest, files: InstallPermAs(env, dest, files, 0644)

# Add configuration options to the 'environment'

opts.Update(env)

# Flags to give some more warning output from the GCC compiler

env.Append(CFLAGS=['-Wall','-W','-Wconversion','-Wimplicit'])

# Flags for coverage testing (will apply to all code built in 'env')

if env.get('GCOV'):
	env.Append(
		CFLAGS=['-fprofile-arcs','-ftest-coverage']
		,LIBS=['gcov']
	)

if env.get('DEBUG'):
	env.Append(
		CFLAGS=['-g']
	)

# Create config.h including version number

subst_dict = {
	'@VERSION@':version
	, '@PYTHON@' : python_exe
	, '@INSTALL_BIN@':env['INSTALL_BIN']
	, '@INSTALL_INCLUDE@':env['INSTALL_INCLUDE']
	, '@INSTALL_LIB@':env['INSTALL_LIB']
}

env.Append(SUBST_DICT=subst_dict)

configh = env.SubstInFile('config.h.in')

configscript = env.SubstInFile('freesteam-config.in')

install_config = env.InstallProgram("${INSTALL_ROOT}$INSTALL_BIN",configscript)

# Here is the list of all of the source files that will go into
# the freesteam DLL/SO.

srcs = ["region1.c", "region2.c", "region3.c", "region4.c", "steam_ph.c"
	,"steam.c", "backwards.c","b23.c", "common.c", "derivs.c", "zeroin.c"
	,"steam_ps.c", "solver2.c","steam_pT.c","steam_pu.c","steam_Ts.c"
	,"bounds.c"
]

# Create a sub-environment with linking to libm for mathematics routines

lib_env = env.Clone()
lib_env.Append(
	LIBS = ['m']
)
lib_env.Append(
	LIBS = env['GSL_LIBS']
	,LIBPATH = env['GSL_LIBPATH']
	,CPPPATH = env['GSL_CPPPATH']
)

# Create the shared library

if platform.system()=="Linux":
	lib_env.Append(LINKFLAGS=['-Wl,-soname,$SONAME'])

lib = lib_env.SharedLibrary("freesteam",srcs)

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


# Store a reference to this library so that we can reference it from
# SConscript files in subdirectories

env['libfreesteam'] = lib

# Create an environment variable into which locations of installed files can be
# stored.

env['installedfiles'] = []

# Build the Python API for freesteam

env.SConscript(["python/SConscript"],'env')

# Build the ASCEND API for freesteam (TODO move this into the ASCEND project
# once it becomes stable.

env.SConscript(["ascend/SConscript"],'env')

# Create the test program. Currently we're not using any unit testing library;
# this is currently just simple bespoke code.

prog_env = env.Clone()
prog_env.Append(
	LIBS=['freesteam']
	,LIBPATH=['#']
	,LINKFLAGS=['-Wl,-rpath,'+str(Dir('#'))]
)

prog_env.Program("test","test.c")

#-------------------------------------------------------------------------------
# Install files, if requested.

print "INSTALL_LIB =",env.get('INSTALL_LIB')
print "INSTALL_PREFIX =",env.get('INSTALL_PREFIX')
print "INSTALL_ROOT =",env.get('INSTALL_ROOT')

try:
	umask = os.umask(022)
except OSError:
	# ignore on systems that don't support umask
	pass


libname = "${INSTALL_LIB}/$SONAME$SONAME_MINOR"
if platform.system()=="Windows":
	install_lib = env.InstallLibrary("$INSTALL_ROOT$INSTALL_LIB",[lib])
else:
	install_lib = env.InstallLibraryAs("${INSTALL_ROOT}"+libname, [lib])

	link1 = "$INSTALL_LIB/${SHLIBPREFIX}freesteam$SHLIBSUFFIX"
	link2 = "$INSTALL_LIB/$SONAME"

	install_link1 = None
	if env.subst(link1) != env.subst(libname):
		install_link1 = env.Command("${INSTALL_ROOT}"+link1,install_lib,"ln -s %s $TARGET" % libname)

	install_link2 = None
	if env.get("SONAME_MINOR"):
		install_link2 = env.Command("${INSTALL_ROOT}"+link2,install_lib,"ln -s %s $TARGET"%libname)
	
	env['installedfiles'] += [install_link1, install_link2]

env['installedfiles'] += [install_lib, install_config]

import glob
headerfiles = glob.glob("*.h")

if env.get('HAVE_SWIG') and env.get('HAVE_PYTHON'):
	headerfiles += glob.glob("python/*.i")

#---------------
# DISTRIBUTION TARBALL

env['DISTTAR_FORMAT']='bz2'
env.Append(
	DISTTAR_EXCLUDEEXTS=['.o','.os','.so','.0','.1','.2','.3'
		,'.a','.dll','.lib','.cc','.cache'
		,'.pyc','.cvsignore','.dblite','.log','.pl','.out','.exe','.aux','.idx'
		,'.toc','.lof','.lot','.mm','.warnings','.tm2','.swp',',tmp','.gz'
		,'.bz2','.7z','.deb','.dsc','.changes'
	]
	, DISTTAR_EXCLUDEDIRS=['CVS','.svn','.sconf_temp', 'dist','debian']
)

tar = env.DistTar("dist/"+env['DISTTAR_NAME']
	, [env.Dir('#')]
)

#------------------------------------------------------
# DEBIAN TARBALL for use with Build Service

import glob
deb_files = glob.glob('debian/*.install')
deb_files += glob.glob('debian/*.docs')
deb_files += glob.glob('debian/*.dirs')
deb_files += glob.glob('debian/*.man')
deb_files += glob.glob('debian/*.manpages')
deb_files += ['debian/%s' % s for s in ['rules','control','changelog','compat','copyright','dirs']]

deb_tar = env.Tar(
	'dist/debian.tar.gz'
	,deb_files
	,TARFLAGS = ['cz']
)

#---------------------------------------------------
env['installedfiles'] += [env.InstallHeader("${INSTALL_ROOT}$INSTALL_INCLUDE/freesteam", headerfiles)]
	
env.Alias('install',env['installedfiles'])

#------------------------------------------------------
# WINDOWS INSTALLER



#------------------------------------------------------
# DEFAULT TARGETS

default_targets =['python','ascend']

env.Default(default_targets)

print "Building targets:"," ".join([str(i) for i in BUILD_TARGETS])

