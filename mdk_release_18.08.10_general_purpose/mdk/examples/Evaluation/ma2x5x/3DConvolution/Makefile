#Activate the flag that disables the default debug mode build because of larger code sections. Code section is larger on ma2150 ma2450
BUILD_WITH_CODE_SIZE_OPTIMIZATION=yes
# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk

MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

# ------------------------------[ Components used ]--------------------------------#
#Component lists:
#No components used by mvTensor
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = yes

ComponentList_LOS := VcsHooks UnitTest Fp16Convert PipePrint
ComponentList_SVE += kernelLib/MvCV
MVASMOPT += -a
#-------------------------------[ Local shave applications sources ]--------------------#

#Choosing C sources the mvTensor application on shave
MvTensorApp = shave/small4Dconv
SHAVE_C_SOURCES_mvTensor = $(wildcard $(DirAppRoot)/shave/*.c)

#Generating list of required generated assembly files for the mvTensor app on shave
SHAVE_GENASMS_mvTensor = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_mvTensor))
#SHAVE_GENASMS_mvTensor += $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_mvTensor))
#Generating required objects list from sources
SHAVE_mvTensor_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_mvTensor))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_mvTensor) $(SHAVE_mvTensor_OBJS) $(SHAVE0_APPS) $(SHAVE_APP_LIBS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_mvTensor)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(MvTensorApp).mvlib
SHAVE0_APPS    = $(MvTensorApp).shv0lib
SHAVE1_APPS    = $(MvTensorApp).shv1lib
SHAVE2_APPS    = $(MvTensorApp).shv2lib
SHAVE3_APPS    = $(MvTensorApp).shv3lib
SHAVE4_APPS    = $(MvTensorApp).shv4lib
SHAVE5_APPS    = $(MvTensorApp).shv5lib
SHAVE6_APPS    = $(MvTensorApp).shv6lib
SHAVE7_APPS    = $(MvTensorApp).shv7lib
SHAVE8_APPS    = $(MvTensorApp).shv8lib
SHAVE9_APPS    = $(MvTensorApp).shv9lib
SHAVE10_APPS   = $(MvTensorApp).shv10lib
SHAVE11_APPS   = $(MvTensorApp).shv11lib

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release

# Verbose or not, uncomment for more verbosity
#ECHO ?=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the MvTensorApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e start  --gc-sections
$(MvTensorApp).mvlib : $(SHAVE_mvTensor_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_mvTensor_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@


# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options


# ------------------------------[ Extra Rules ]-------------------------------------------#

TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, MA2450"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
