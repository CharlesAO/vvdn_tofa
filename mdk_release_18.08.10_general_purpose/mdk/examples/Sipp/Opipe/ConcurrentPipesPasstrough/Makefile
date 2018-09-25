#Activate the flag that disables debug mode build and keep release mode as default
BUILD_WITH_CODE_SIZE_OPTIMIZATION=yes
# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_SOC_REV ?= ma2450

ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155 ma2450 ma2455))
    $(error "Incorrect MV_SOC_REV value! This app runs only on ma2x5x!")
endif

MV_COMMON_BASE ?= ../../../../common
LEON_RT_BUILD = yes
# The leon code will we built for LOS(due to the directory name) but on debugger we will run on LRT
ComponentList_LOS := PipePrint
ComponentList_LRT := UnitTestVcs VcsHooks Opipe Fp16Convert PipePrint

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_PLATFORM = myriad2
# ------------------------------[ Components used ]--------------------------------#
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no

#-------------------------------[ Settings ]---------------------------------------#

#-------------------------------[ Local shave applications sources ]--------------------#
#No shave code

#--------------------------[ Shave applications section ]--------------------------#
#No shave code

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =00.50.23.0

# Verbose or not, uncomment for more verbosity
#ECHO=


#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building theApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options 

# Extra app related options
#CCOPT			+= -DDEBUG

CCOPT     += -D'CDMA_USE_FAST_MTX'
CCOPT_LRT += -D'CDMA_USE_FAST_MTX'
 
#Include the top makefile
CCOPT_LRT += -D'DEFAULT_HEAP_SIZE=512'
CCOPT     += -D'DEFAULT_HEAP_SIZE=512' 

#update for oPIPE buffers section
CCOPT     += -D'OPIPE_MEM_SECT=".cmx.data"'
CCOPT_LRT += -D'OPIPE_MEM_SECT=".cmx.data"'

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

TEST_HW_PLATFORM := "MOVISIM_MA2150_SIPP, MV0182_MA2150, MV0212_MA2450 "
TEST_TYPE        := "AUTO"
TEST_TAGS        := "MA2150, MA2450"
