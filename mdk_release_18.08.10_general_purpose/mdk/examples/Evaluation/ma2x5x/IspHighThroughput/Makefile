# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

#The Movidius dev kits are 2: MV0212 equipped with MA2450 and MV0182 equipped with
#other revisions( MA2150)
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif

ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155 ma2450 ma2455))
    $(error "Incorrect MV_SOC_REV value! This app runs only on ma2x5x!")
endif

# ------------------------------[ Components used ]--------------------------------#
ifeq ($(EVM_BOARD_USED),MV0182)
 ComponentList  = Board182
else
 ComponentList = MV0212
endif

ComponentList += UnitTestVcs VcsHooks Opipe Fp16Convert PipePrint

#No components used by HelloWorldLeon
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no

MY_RESOURCE = $(MV_COMMON_BASE)/../resources/Studio_D65_800_IMX214_2_002.raw

#-------------------------------[ Settings ]---------------------------------------#
LEON_STARTUP_MASK ?= LRT

CCOPT     += -D'CDMA_USE_FAST_MTX'
CCOPT_LRT += -D'CDMA_USE_FAST_MTX'

#Path to Includes
CCOPT     += -I'./leon/params'
CCOPT_LRT += -I'./leon/params'

#Include the top makefile
CCOPT_LRT += -D'DEFAULT_HEAP_SIZE=512'
CCOPT     += -D'DEFAULT_HEAP_SIZE=512'

CCOPT     += -D'INPUT_IMAGE="$(MY_RESOURCE)"'
CCOPT_LRT += -D'INPUT_IMAGE="$(MY_RESOURCE)"'

CCOPT_LRT += -D$(EVM_BOARD_USED)
CCOPT     += -D$(EVM_BOARD_USED)

#-------------------------------[ Local shave applications sources ]---------------#
#No shave code

#--------------------------[ Shave applications section ]--------------------------#
#NO SHAVE CODE

#-------------------------------[ Settings ]---------------------------------------#

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =

# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE ?= ../../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------#
#Describe the rule for building theApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library

# Extra app related options
#CCOPT			+= -DDEBUG

TEST_HW_PLATFORM := "MOVISIM_MA2150_SIPP, MV0182_MA2150, MV0212_MA2450 "
TEST_TYPE        := "AUTO"
TEST_TAGS        := "MA2150, MA2450, LONG_RUN_TEST"

