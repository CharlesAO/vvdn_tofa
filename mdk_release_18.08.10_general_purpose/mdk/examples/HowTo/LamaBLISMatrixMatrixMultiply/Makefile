# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))
VERS_STR      = 0.1.0-34
BLIS_VERSION_STRING=\"$(VERS_STR)\"
VERSION       = BLIS_VERSION_STRING=\"$(VERS_STR)\"

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk

MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

DEFAULT_HEAP_SIZE = 20480

LinkerScript = myriad2Custom.ldscript

# ------------------------------[ Components used ]--------------------------------#
ComponentList_LOS += UnitTestVcs VcsHooks PipePrint
#No components used by BLISLeon
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no

#-------------------------------[ Local shave applications sources ]--------------------#

#Choosing C sources the WarpAffine application on shave
LamaBLISMatrixMatrixMultiply = shave/BLISTest

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(LamaBLISMatrixMatrixMultiply).mvlib
SHAVE0_APPS    = $(LamaBLISMatrixMatrixMultiply).shv0lib

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION = 00.50.43.10

# Verbose or not, uncomment for more verbosity
#ECHO=
# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE   ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

# Include BLIS create lib Makefile
include $(MV_COMMON_BASE)/components/BLIS/genblislib.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building theApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e startSGEMM -u startSGEMMTRSM_LL -u startSGEMMTRSM_LU -u startSGEMMTRSM_RU -u startSGEMMTRSM_RL --gc-sections
$(LamaBLISMatrixMatrixMultiply).mvlib : $(PROJECT_SHAVE_LIBS) $(BLIS_LIB_SHAVE)
	$(ECHO) $(LD) -r $(ENTRYPOINTS) $(MVLIBOPT)  $(BLIS_LIB_SHAVE) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
CCOPT   += -DMYRIAD2
MVCCOPT += -DMYRIAD2
#CCOPT	+= -DDEBUG
CCOPT   += -DBLIS -DVERSION -DBLIS_VERSION_STRING=\"$(VERS_STR)\" -DLEON_USE_REAL_NUMBERS_ONLY
CCOPT   += -Ishared -I$(MV_COMMON_BASE)/components/BLIS/leon/config/reference
# ------------------------------[ Extra Rules ]-------------------------------------------#

MAT_SIZE ?= 2048
MAT_SEED ?= 1

AllLibs += --start-group $(BLIS_LIB_LEON) --end-group

matrix:
	gcc -o PCmatrixGenerator/multiplyNxN PCmatrixGenerator/multiplyNxN.c
	./PCmatrixGenerator/multiplyNxN $(MAT_SIZE) $(MAT_SEED)

TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, LONG_RUN_TEST, MA2450"
