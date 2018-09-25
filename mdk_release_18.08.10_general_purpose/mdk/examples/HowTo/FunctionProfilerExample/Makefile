# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# Verbose or not, common out for more verbosity
ECHO ?=@

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

LEON_RT_BUILD = yes

DirLDScrCommon = $(MV_COMMON_BASE)/scripts/ld/myriad2collection
#LinkerScript    =  ./scripts/ld/linkcmds
#LinkerScript    =  ./scripts/ld/rtems_memory_map.ldscript

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

#LEON_ENDIAN =

# ------------------------------[ Components used ]--------------------------------#
#Component lists:
ComponentList_LOS += UnitTestVcs VcsHooks PipePrint FatalExtension
ComponentList_LRT += PipePrint
ComponentList_SVE += PipePrint

#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no


#-------------------------------[ Local shave applications sources ]--------------------#

#Choosing C sources the SimpleSipp application on shave
SimpleSippApp = shave/SimpleShave
SHAVE_C_SOURCES_SimpleSipp = $(wildcard $(DirAppRoot)/shave/*.c)
SHAVE_CPP_SOURCES_SimpleSipp = $(wildcard $(DirAppRoot)/shave/*.cpp)
#Choosing ASM sources for the shave SimpleSipp app on shave
#no asm files
#SHAVE_ASM_SOURCES_SimpleSipp =

#Generating list of required generated assembly files for the SimpleSipp app on shave
SHAVE_GENASMS_SimpleSipp = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_SimpleSipp))
SHAVE_GENASMS_SimpleSipp += $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_SimpleSipp))
#Generating required objects list from sources
SHAVE_SimpleSipp_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_SimpleSipp))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_SimpleSipp) $(SHAVE_SimpleSipp_OBJS) $(SHAVE0_APPS) $(SHAVE_APP_LIBS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_SimpleSipp)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(SimpleSippApp).mvlib
# declare here all fifty shaves of stray
SHAVE0_APPS = $(SimpleSippApp).shv0lib
SHAVE1_APPS = $(SimpleSippApp).shv1lib
SHAVE2_APPS = $(SimpleSippApp).shv2lib

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =Latest

# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../common
#MV_RTEMS_BUILD_DIR=$(MV_COMMON_BASE)/utils/RTEMSbuild


# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk




#MV_RTEMS_LOS_LIB = $(MV_RTEMS_BUILD_DIR)/$(MV_GCC_TOOLS)/myriad2/lib
#MV_RTEMS_LRT_LIB = $(MV_RTEMS_BUILD_DIR)/$(MV_GCC_TOOLS)/myriad2/lib

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the SimpleSippApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e shave_zero -u shave_one -u shave_two --gc-sections
$(SimpleSippApp).mvlib : $(SHAVE_SimpleSipp_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_SimpleSipp_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@


# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT += -DNAME=VALUE
#CCOPT += -MD
#Don't clear BSS. This speeds up RTEMS a log
#CCOPT += -DDONT_CLEAR_BSS
#CCOPT += -DDISABLE_LEON_ICACHE -DDISABLE_LEON_DCACHE

TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, MA2450"
TEST_TIMEOUT_SEC := 900

#run:
#	echo All tests passed
