#Activate the flag that disables the default debug mode build because of larger code sections. Code section is larger on ma2150
BUILD_WITH_CODE_SIZE_OPTIMIZATION=yes
#-------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
APPNAME = fish_eye

MV_COMMON_BASE ?= ../../../common

# Uncomment for more build output verbosity
#ECHO=

# ------------------------------[ Components used ]--------------------------------#
ComponentList = UnitTestVcs VcsHooks imageWarp kernelLib/MvCV/kernels/warpMeshExpand kernelLib/MvCV/include kernelLib/MvCV/kernels/warpMeshSample8bit kernelLib/MvCV/kernels/warpMeshSample8bitStride

# Change to "yes" if you want to use Shave code from a component such as the kernels
# from MvCV. Option "no" speeds up compilation by not including this code.
SHAVE_COMPONENTS = yes

#-------------------------------[ Build overrides ]--------------------------------#
# Overrides that need to be set before including generic.mk
DirLDScrCommon = $(MV_COMMON_BASE)/scripts/ld/$(MV_SOC_PLATFORM)memorySections

#-------------------------------[ Settings ]---------------------------------------#
MV_SOC_PLATFORM = myriad2

#----------------------------- [ fish_eye ] --------------------#
SHAVE_CPP_SOURCES_fish_eye += $(wildcard $(DirAppRoot)/shave/*.cpp)
SHAVE_CPP_SOURCES_fish_eye += $(wildcard $(DirAppRoot)/shave/*/*/*.cpp)
SHAVE_GENASMS_fish_eye = $(patsubst %.cpp, $(DirAppObjBase)%.asmgen, $(SHAVE_CPP_SOURCES_fish_eye))
SHAVE_fish_eye_OBJS = $(patsubst $(DirAppObjBase)%.asmgen, $(DirAppObjBase)%_shave.o, $(SHAVE_GENASMS_fish_eye))

#MVCC_INCLUDE += $(patsubst %,-I %, $(wildcard $(DirAppRoot)/shave/kernels/*))

#------------------------------[ Test data ]---------------------------------------#

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(APPNAME).mvlib
SHAVE0_APPS = $(APPNAME).shv0lib

#Don't use it because the application contains warnings
#DISABLE_ERROR_ON_WARNINGS ?=yes
# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

ENTRYPOINTS += -e imageWarp --gc-sections

$(APPNAME).mvlib : $(SHAVE_fish_eye_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(SHAVE_fish_eye_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

PROJECTCLEAN += $(SHAVE_fish_eye_OBJS) $(SHAVE_GENASMS_fish_eye)
PROJECTINTERM += $(SHAVE_GENASMS_fish_eye)

#--------------------------------- [ Build Options ] ------------------------------#
# Don't stop function inlining
MVCOMPILEINLINESTATUS =
MVCCPPOPT+=-DIMAGEWARP_USE_DMA_ENGINE
#------------------------------- [ Extra dependancies] ----------------------------#

#-----------------------------------[ Extra Rules ]--------------------------------#
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE := "AUTO"
TEST_TAGS := "MA2150, MDK_COMPONENTS"
