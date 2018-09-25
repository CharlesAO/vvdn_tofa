# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

# ------------------------------[ Components used ]--------------------------------#
#Component lists:
ComponentList_LOS += UnitTestVcs VcsHooks PipePrint FatalExtension

#Choosing if this project has shave components or not
SHAVE_COMPONENTS=no

#-------------------------------[ Settings ]---------------------------------------#

#-------------------------------[ Local shave applications sources ]--------------------#
MYRIAD = myriad2Asm

#Choosing C sources the ExVec application on shave
ExVecApp = shave/exvec
SHAVE_C_SOURCES_exvec = $(wildcard $(DirAppRoot)/shave/*.c)

#Generating list of required generated assembly files for the hello app on shave
SHAVE_GENASMS_exvec = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_exvec))
#Generating required objects list from sources
SHAVE_exvec_OBJS =  $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_exvec))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_exvec) $(SHAVE_exvec_OBJS) $(SHAVE_APP_LIBS) $(SHAVE0_APPS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_exvec)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(ExVecApp).mvlib
SHAVE0_APPS = $(ExVecApp).shv0lib

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =00.50.23.0

# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the ExVec application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e _EP_start  --gc-sections
$(ExVecApp).mvlib : $(SHAVE_exvec_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_exvec_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@


# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT			+= -DDEBUG
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, MA2450"
