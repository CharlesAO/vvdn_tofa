# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
APPNAME=test
VERS_STR      = 0.1.0-34
BLIS_VERSION_STRING=\"$(VERS_STR)\"
VERSION       = BLIS_VERSION_STRING=\"$(VERS_STR)\"
DirAppRoot = ..
DirAppOutput = output

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk

#-------------------------------[ Settings ]---------------------------------------#
MV_SOC_PLATFORM = myriad2
DEFAULT_HEAP_SIZE = 20480

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480))
LinkerScript = ../myriad2Custom_ma2x8x.ldscript
else
LinkerScript = ../myriad2Custom.ldscript
endif

LEON_APP_C_SOURCES  = $(wildcard $(DirAppRoot)/leon/testsuite/src/*.c)

# ------------------------------[ Components used ]--------------------------------#
ComponentList_LOS += UnitTestVcs VcsHooks
ComponentList_LRT += UnitTestVcs VcsHooks
ComponentList_SVE += UnitTestVcs VcsHooks
#No components used by BLISLeon
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no

BLISTestApp = $(DirAppOutput)/BLISTest

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(BLISTestApp).mvlib

define DEFINE_SHAVE_APPS
SHAVE$(1)_APPS = $(BLISTestApp).shv$(1)lib
endef

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150))
SHAVES_IDX = 0 1 2 3 4 5 6 7 8 9 10 11
$(foreach idx,$(SHAVES_IDX),$(eval $(call DEFINE_SHAVE_APPS,$(idx))))
else
SHAVES_IDX = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
$(foreach idx,$(SHAVES_IDX),$(eval $(call DEFINE_SHAVE_APPS,$(idx))))
endif


# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION = 00.50.43.10

# Verbose or not, uncomment for more verbosity
#ECHO=
# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE   = ../../../../../../../common

KERNEL_LIB_DIR = $(MV_COMMON_BASE)/components/kernelLib
FRAME_DIR = $(MV_COMMON_BASE)/components/BLIS/leon/frame

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

# Include BLIS create lib Makefile
include $(MV_COMMON_BASE)/components/BLIS/genblislib.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building theApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e startSGEMM -u startSGEMMTRSM_LL -u startSGEMMTRSM_LU -u startSGEMMTRSM_RU -u startSGEMMTRSM_RL --gc-sections
$(BLISTestApp).mvlib : $(PROJECT_SHAVE_LIBS) $(BLIS_LIB_SHAVE)
	$(ECHO) $(LD) -r $(ENTRYPOINTS) $(MVLIBOPT) $(BLIS_LIB_SHAVE) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options
# Extra app related options
CCOPT   += -DMYRIAD2
MVCCOPT += -DMYRIAD2

CCOPT   += -DBLIS -DVERSION -DBLIS_VERSION_STRING=\"$(VERS_STR)\" -DLEON_USE_REAL_NUMBERS_ONLY
CCOPT   += -Ileon/testsuite/src -Ishared -I$(MV_COMMON_BASE)/components/BLIS/leon/config/reference

# ------------------------------[ Extra Rules ]-------------------------------------------#

SourceDebugScript=../scripts/$(MV_SOC_PLATFORM)/moviDebug2/BLISTestAppTestSuite_2SHAVEs_mdbg2.tcl

TEST_HW_PLATFORM := "MOVISIM_MA2150_IGNORE, MOVISIM_MA2450_IGNORE, MV0182_MA2150, MV0212_MA2450, MV0235_MA2480"
TEST_TYPE := "AUTO"
TEST_TAGS := "MA2150, MDK_COMPONENTS, MA2450, MA2480"
TEST_TIMEOUT_SEC := 1200


AllLibs += --start-group $(BLIS_LIB_LEON) --end-group
