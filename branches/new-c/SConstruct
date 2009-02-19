#!/usr/bin/env python

opts = Options()

opts.Add(BoolOption(
	'GCOV'
	,"Build for coverage testing using gcov."
	,False
))

# Building with MinGW

import os
tools = ['swig','ascend']
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

opts.Update(env)

srcs = ["region1.c", "region2.c", "region3.c", "region4.c", "steam_ph.c"
	,"steam.c", "backwards.c","b23.c", "common.c", "derivs.c"
]

env.Append(CFLAGS=['-Wall','-W','-Wconversion'])

if env.get('GCOV'):
	env.Append(
		CFLAGS=['-fprofile-arcs','-ftest-coverage']
		,LIBS=['gcov']
	)

lib_env = env.Clone()
lib_env.Append(
	LIBS = ['m']
)

lib = lib_env.SharedLibrary("freesteam",srcs)

env['libfreesteam'] = lib

env.SConscript(["python/SConscript"],'env')

env.SConscript(["ascend/SConscript"],'env')

prog_env = env.Clone()
prog_env.Append(
	LIBS=['freesteam']
	,LIBPATH=['#']
	,LINKFLAGS=['-Wl,-rpath,'+str(Dir('#'))]
)

prog_env.Program("test","test.c")

