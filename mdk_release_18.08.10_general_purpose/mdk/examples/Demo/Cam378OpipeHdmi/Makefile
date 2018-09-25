# ---------------------------------[ App ]-------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ---------------------------[ Build overrides ]-------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_PLATFORM  ?= myriad2
MV_SOC_REV        = ma2450

LEON_RT_BUILD    ?= yes
MV_SOC_OS         = rtems

ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2450))
   $(error "Incorrect MV_SOC_REV value! This app runs only on ma2450!")
endif

# -----------------------------[ Components used ]-----------------------------#
ComponentList     += MV0212 UnitTestVcs VcsHooks Opipe Fp16Convert \
                     HdmiTxIte LcdGeneric CamGeneric CameraModules \
                     CameraModules/imx378 PipePrint

ComponentList_LOS += FatalExtension

#----------------------[ Local shave applications sources ]--------------------#
#No local shave applications for this example

#-------------------------[ Shave applications section ]-----------------------#
#No local shave applications for this example

# -----------------------------[ Tools overrides ]-----------------------------#
# Hardcode tool version here if needed, otherwise defaults
# to latest stable release
#MV_TOOLS_VERSION =

# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location
# (but allow user to override in environment)
MV_COMMON_BASE ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#--------------------[ Local shave applications build rules ]------------------#
#No local applications build rules for this example

# ----------------------------- [ Build Options ] -----------------------------#
CCOPT      += -D'CDMA_USE_FAST_MTX'
CCOPT_LRT  += -D'CDMA_USE_FAST_MTX'

#Path to Includes
CCOPT      += -I'./leon/params'
CCOPT_LRT  += -I'./leon/params'

# update for LCD ISR routing
CCOPT      += -DUse_LcdGeneric_on_LRT
CCOPT_LR   += -DUse_LcdGeneric_on_LRT

#update for oPIPE buffers section
CCOPT      += -D'OPIPE_MEM_SECT=".cmx.data"'
CCOPT_LRT  += -D'OPIPE_MEM_SECT=".cmx.data"'

#Include the top makefile
CCOPT_LRT  += -D'DEFAULT_HEAP_SIZE=1024'
CCOPT      += -D'DEFAULT_HEAP_SIZE=1024'

TEST_TAGS  := "MA2450"
