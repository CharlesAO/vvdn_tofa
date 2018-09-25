# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

#The Movidius dev kits are 2: MV0212 equipped with MA2450 and MV0182 equipped with
#other revisions(MA2150)
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk


# ------------------------------[ Components used ]--------------------------------#
ifeq ($(EVM_BOARD_USED),MV0182)
 ComponentList_LOS  = Board182
else 
 ComponentList_LOS = MV0212
endif
ComponentList_LOS += CameraModules CameraModules/imx208 LcdGeneric CamGeneric sipp/include sipp/arch/$(MV_SOC_REV)/include PipePrint
SHAVE_COMPONENTS = no
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
MV_COMMON_BASE  ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#No local applications build rules for this example


# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options
CCOPT_LRT += -D$(EVM_BOARD_USED)
CCOPT     += -D$(EVM_BOARD_USED)

# Extra app related options

TEST_TAGS:= "MA2150, MA2450"

