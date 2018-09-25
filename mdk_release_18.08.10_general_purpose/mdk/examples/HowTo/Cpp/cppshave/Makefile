# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450
# ------------------------------[ Components used ]--------------------------------#
#Component lists:
ComponentList_LOS += UnitTestVcs VcsHooks PipePrint
ComponentList_SVE := PipePrint

#Choosing if this project has shave components or not
SHAVE_COMPONENTS = yes

#-------------------------------[ Settings ]---------------------------------------#

#-------------------------------[ Local shave applications sources ]--------------------#
ShaveCpp = shave/cppshave
SHAVE_CPP_SOURCES_cppshave = $(wildcard $(DirAppRoot)/shave/*.cpp)
#Choosing ASM sources for the shave cppshave app on shave
#no asm files
#SHAVE_ASM_SOURCES_cppshave =

#Generating list of required generated assembly files for the cppshave app on shave
SHAVE_GENASMS_cppshave = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_cppshave))
#Generating required objects list from sources
SHAVE_cppshave_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_cppshave))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_cppshave) $(SHAVE_cppshave_OBJS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_cppshave)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(ShaveCpp).mvlib
SHAVE0_APPS = $(ShaveCpp).shv0lib


#--------------------------[ Shave applications section ]--------------------------#
#NO SHAVE CODE

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =

# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

CCOPT+=-Dcppshave0___crtinit=cppshave0__EP_crtinit
# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building theApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library

# -------------------------------- [ Build Options ] ------------------------------ #
ENTRYPOINTYS = -e _EP_crtinit --gc-sections
$(ShaveCpp).mvlib : $(SHAVE_cppshave_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTYS) $(MVLIBOPT) $(SHAVE_cppshave_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

# Extra app related options
#CCOPT			+= -DDEBUG
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, MA2450"
