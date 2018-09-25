#-------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
APPNAME = SimpleCrossCompilableCVPipe

#-------------------------------[ Build overrides ]--------------------------------#
# Overrides that need to be set before including generic.mk
MV_SOC_REV      ?= ma2450
MV_COMMON_BASE  ?= ../../../../common

# Uncomment for more build output verbosity
#ECHO=

# ------------------------------[ Components used ]--------------------------------#
ComponentList_LOS  = UnitTest VcsHooks PipePrint
ComponentList_SVE = kernelLib/MvCV/include

# Change to "yes" if you want to use Shave code from a component such as the kernels
# from MvCV. Option "no" speeds up compilation by not including this code.
SHAVE_COMPONENTS = no

#-------------------------------[ Settings ]---------------------------------------#

#----------------------------- [ SimpleCrossCompilableCVPipe ] --------------------#
SHAVE_CPP_SOURCES_SimpleCrossCompilableCVPipe += $(wildcard $(DirAppRoot)/shave/*.cpp)
SHAVE_CPP_SOURCES_SimpleCrossCompilableCVPipe += $(wildcard $(DirAppRoot)/shave/*/*/*.cpp)
SHAVE_GENASMS_SimpleCrossCompilableCVPipe = $(patsubst %.cpp, $(DirAppObjBase)%.asmgen, $(SHAVE_CPP_SOURCES_SimpleCrossCompilableCVPipe))
SHAVE_SimpleCrossCompilableCVPipe_OBJS = $(patsubst $(DirAppObjBase)%.asmgen, $(DirAppObjBase)%_shave.o, $(SHAVE_GENASMS_SimpleCrossCompilableCVPipe))

MVCC_INCLUDE += $(patsubst %,-I %, $(wildcard $(DirAppRoot)/shave/kernels/*))

#------------------------------[ Test data ]---------------------------------------#

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(APPNAME).mvlib
SHAVE0_APPS = $(APPNAME).shv0lib

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_SimpleCrossCompilableCVPipe_OBJS) $(SHAVE_GENASMS_SimpleCrossCompilableCVPipe)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_SimpleCrossCompilableCVPipe)

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

ENTRYPOINTS += -e SimpleCrossCompilableCVPipeInit -u SimpleCrossCompilableCVPipeRun --gc-sections

$(APPNAME).mvlib : $(SHAVE_SimpleCrossCompilableCVPipe_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(SHAVE_SimpleCrossCompilableCVPipe_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

#--------------------------------- [ Build Options ] ------------------------------#
# Don't stop function inlining
MVCOMPILEINLINESTATUS =

#------------------------------- [ Extra dependancies] ----------------------------#

#-----------------------------------[ Extra Rules ]--------------------------------#

TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, MA2450"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450, MOVISIM_MA2150, MOVISIM_MA2450"
