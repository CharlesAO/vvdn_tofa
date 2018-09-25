#-------------------------------[ App ]--------------------------------------------#
# This can be overridden here if something different is required
APPNAME = AsmOptimisation

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

# Uncomment for more build output verbosity
#ECHO=

# ------------------------------[ Components used ]--------------------------------#
ComponentList = UnitTest VcsHooks kernelLib/MvCV

# Change to "yes" if you want to use Shave code from a component such as the kernels 
# from MvCV. Option "no" speeds up compilation by not including this code.
SHAVE_COMPONENTS = no

#-------------------------------[ Build overrides ]--------------------------------#
# Overrides that need to be set before including generic.mk

#-------------------------------[ Settings ]---------------------------------------#
MV_SOC_PLATFORM = myriad2

#----------------------------- [ AsmOptimisation ] --------------------#
#Choosing C/CPP sources for the shave AsmOptimisation app on shave
SHAVE_CPP_SOURCES_AsmOptimisation += $(wildcard $(DirAppRoot)/shave/*.cpp)
SHAVE_CPP_SOURCES_AsmOptimisation += $(wildcard $(DirAppRoot)/shave/*/*.cpp)
#Choosing ASM sources for the shave AsmOptimisation app on shave
SHAVE_ASM_SOURCES = $(wildcard $(DirAppRoot)/shave/*/*.asm)

SHAVE_GENASMS_AsmOptimisation = $(patsubst %.cpp, $(DirAppObjBase)%.asmgen, $(SHAVE_CPP_SOURCES_AsmOptimisation))
SHAVE_AsmOptimisation_OBJS = $(patsubst $(DirAppObjBase)%.asmgen, $(DirAppObjBase)%_shave.o, $(SHAVE_GENASMS_AsmOptimisation)) \
					$(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES))


MVCC_INCLUDE += $(patsubst %,-I %, $(wildcard $(DirAppRoot)/shave/kernel/))
PROJECTCLEAN += $(SHAVE_AsmOptimisation_OBJS) $(SHAVE_GENASMS_AsmOptimisation)
PROJECTINTERM += $(SHAVE_GENASMS_AsmOptimisation)
#------------------------------[ Test data ]---------------------------------------#

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(APPNAME).mvlib
SHAVE0_APPS = $(APPNAME).shv0lib

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

ENTRYPOINTS += -e AsmOptimisationInit -u AsmOptimisationRun --gc-sections

$(APPNAME).mvlib : $(SHAVE_AsmOptimisation_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(SHAVE_AsmOptimisation_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@



#--------------------------------- [ Build Options ] ------------------------------#
# Don't stop function inlining
MVCOMPILEINLINESTATUS = 

#------------------------------- [ Extra dependancies] ----------------------------#

#-----------------------------------[ Extra Rules ]--------------------------------#
TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, MA2450"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450, MOVISIM_MA2150, MOVISIM_MA2450"
