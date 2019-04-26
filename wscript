import os
from waflib import Logs

###########Waf options################

NAME = 'TemplateCpp'
VERSION = '1.0'

top = '.'
out = 'build'

############Helpers###################
stars = '*'*20

def _loadTools(ctx):
	ctx.load('compiler_cxx')
	ctx.load('asm')
	ctx.load('bison')
	ctx.load('flex')

def _preBuild(ctx):
	Logs.info('Pre-build...')
	Logs.info('Pre-build complete.')

def _postBuild(ctx):
	Logs.info('Post-build...')
	Logs.info('Post build complete.')

######### Commands ####################
def options(ctx):
	Logs.info('Loading options....')
	_loadTools(ctx)
	ctx.add_option('-d','--debug', dest='debug', default=False, action='store_true', help='Debug mode')
	ctx.add_option('-s', '--symbols',dest='symbols',default=False,action='store_true',help='Debug symbols')


def configure(ctx):
	Logs.info('Configuring....')
	_loadTools(ctx)
	#platform checks

	ctx.env.INCLUDES += ['.', 'src']
	ctx.env.CXXFLAGS = [
			'-std=c++11',
			'-Wall',
			'-Wextra',
			'-pedantic',
			]

	#include symbols for debugging or when explicitely asked
	if ctx.options.debug or ctx.options.symbols:
		ctx.env.CXXFLAGS.append('-g3')

	#Debugging
	if ctx.options.debug:
		ctx.msg('Build environment', '%s DEBUG %s' % (stars, stars), color='RED')

		ctx.env.CXXFLAGS.append('-O0')
		ctx.env.DEFINES.append('DEBUG')
		ctx.env.ENVIRONMENT = 'debug'
	else:
		ctx.msg('Build environment', '%s RELEASE %s' % (stars, stars), color='BOLD')

		ctx.env.CXXFLAGS.append('-O3')
		ctx.env.DEFINES.append('NDEBUG') #Causes asserts to compile out: http://www.cplusplus.com/reference/cassert/assert/
		ctx.env.ENVIRONMENT = 'release'

	#Setup libraries
	ctx.env.LIB = [
		#'pthread',
		]

def build(ctx):
	ctx.add_pre_fun(_preBuild)
	ctx.add_post_fun(_postBuild)
	ctx.recurse('src')

def test(ctx):
	stars = '*'*30
	Logs.info('%s Running Unit Tests %s' % (stars, stars))
	os.system("build/test/WerkTest")

def hello(ctx):
	stars = '*'*30
	Logs.info('%s Running hello %s' % (stars, stars))
	os.system("build/bin/HelloWorld")

def profile(ctx):
	stars = '*'*30;
	Logs.info('%s Running Profiles %s' % (stars, stars))
	os.system("build/bin/WerkProfile")

