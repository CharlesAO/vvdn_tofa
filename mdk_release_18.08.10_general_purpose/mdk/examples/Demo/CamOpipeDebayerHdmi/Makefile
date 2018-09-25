# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_PLATFORM  ?= myriad2
MV_SOC_REV      ?= ma2450

#The Movidius dev kits are 2: MV0212 equipped with MA2450 and MV0182 equipped with
#other revisions( MA2150)
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif
LEON_RT_BUILD    ?= yes

MV_SOC_OS = rtems


ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155 ma2450 ma2455))
    $(error "Incorrect MV_SOC_REV value! This app runs only on ma2x5x!")
endif


# ------------------------------[ Components used ]--------------------------------#
ifeq ($(EVM_BOARD_USED),MV0182)
 ComponentList  = Board182
else
 ComponentList = MV0212
endif
ComponentList += UnitTestVcs VcsHooks Opipe Fp16Convert HdmiTxIte LcdGeneric CamGeneric CameraModules CameraModules/imx208 CameraModules/imx378 PipePrint
ComponentList_LOS += FatalExtension

#-------------------------------[ Local shave applications sources ]--------------------#
#No local shave applications for this example

#--------------------------[ Shave applications section ]--------------------------#
#No local shave applications for this example

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =

# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE   ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#No local applications build rules for this example

# -------------------------------- [ Build Options ] ------------------------------ #
CCOPT     += -D'CDMA_USE_FAST_MTX'
CCOPT_LRT += -D'CDMA_USE_FAST_MTX'

#Path to Includes
CCOPT     += -I'./leon/params'
CCOPT_LRT += -I'./leon/params'

# update for LCD ISR routing
CCOPT     += -DUse_LcdGeneric_on_LRT
CCOPT_LRT += -DUse_LcdGeneric_on_LRT

#update for oPIPE buffers section
CCOPT     += -D'OPIPE_MEM_SECT=".cmx.data"'
CCOPT_LRT += -D'OPIPE_MEM_SECT=".cmx.data"'

CCOPT_LRT += -D'IRQ_RATE_POW=2'

#Include the top makefile
CCOPT_LRT += -D'DEFAULT_HEAP_SIZE=512'
CCOPT     += -D'DEFAULT_HEAP_SIZE=512'

CCOPT_LRT += -D$(EVM_BOARD_USED)
CCOPT     += -D$(EVM_BOARD_USED)

TEST_TAGS := "MA2150, MA2450"
