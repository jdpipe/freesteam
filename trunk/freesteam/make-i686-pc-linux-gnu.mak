#
# Platform-specific options for i686-pc-linux-gnu
#

EXE_SUFFIX = 
SO_SUFFIX = .so

EMSO_LD_SONAME = -soname=$(EMSO_NAME)
EES_LD_SONAME = -soname=$(EES_NAME)

SO_LD = $(LD)