# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_PLATFORM = myriad2
MV_SOC_REV ?= ma2450
LEON_RT_BUILD = yes

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE ?= ../../../common

# ------------------------------[ Components used ]--------------------------------#
# This provides the unit test framework and hooks into the VCS environment
ComponentList_LOS := PipePrint FatalExtension
ComponentList_LRT := PipePrint

#Choosing if this project has shave components or not
SvHAVE_COMPONENTS = no

#--------------------------[ Include common makefile rules ]--------------------------
include $(MV_COMMON_BASE)/generic.mk

#--------------------------[ Rules to build Shave Libraries ]--------------------------

# This rule defines a library (relocatable elf) containing all the Objects required for the specific
# shave application. This library may then be instantiated one or more times on differnent shaves (as shvXlibs)

CCOPT += -D'USE_CMX_DMA_NEW_DRIVER=1'
CCOPT_LRT += -D'USE_CMX_DMA_NEW_DRIVER=1'
MVCCOPT += -D'USE_CMX_DMA_NEW_DRIVER=1'

TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE := "MANUAL"
TEST_TAGS:= "MA2150, MA2450, MDK_DRIVERS"

