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
ComponentList += shaveFifoComm FatalExtension
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = yes

#-------------------------------[ Settings ]---------------------------------------#

#-------------------------------[ Local shave applications sources ]--------------------#

#Choosing C sources the helloShave application on shave
HelloShaveApp = shave/helloShave
SHAVE_C_SOURCES_helloShave = $(wildcard $(DirAppRoot)/shave/*.c)
#Choosing ASM sources for the shave helloShave app on shave
#no asm files
#SHAVE_ASM_SOURCES_helloShave =

#Generating list of required generated assembly files for the helloShave app on shave
SHAVE_GENASMS_helloShave = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_helloShave))
#Generating required objects list from sources
SHAVE_helloShave_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_helloShave))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_helloShave) $(SHAVE_helloShave_OBJS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_helloShave)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(HelloShaveApp).mvlib
SHAVE0_APPS = $(HelloShaveApp).shv0lib
SHAVE1_APPS = $(HelloShaveApp).shv1lib
SHAVE2_APPS = $(HelloShaveApp).shv2lib
SHAVE3_APPS = $(HelloShaveApp).shv3lib
SHAVE4_APPS = $(HelloShaveApp).shv4lib
SHAVE5_APPS = $(HelloShaveApp).shv5lib
SHAVE6_APPS = $(HelloShaveApp).shv6lib
SHAVE7_APPS = $(HelloShaveApp).shv7lib
SHAVE8_APPS = $(HelloShaveApp).shv8lib
SHAVE9_APPS = $(HelloShaveApp).shv9lib
SHAVE10_APPS = $(HelloShaveApp).shv10lib
SHAVE11_APPS = $(HelloShaveApp).shv11lib

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION ?=

# Verbose or not, uncomment for more verbosity
#ECHO ?=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the 001_HelloWorldShave application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e masterEntry -u fifoCommMasterRun -u fifoCommSlaveRun -u slaveInitEntry -u slaveEntry --gc-sections
$(HelloShaveApp).mvlib : $(SHAVE_helloShave_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(SHAVE_helloShave_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT			+= -DDEBUG

TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150,TCL_MA2150, MA2450"
