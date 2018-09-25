# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# Verbose or not, common out for more verbosity
ECHO=@

MV_SOC_REV      ?= ma2450

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
#ShaveSymObject   = $(DirAppObjDir)/$(APPNAME).sym.o
MV_SOC_OS = rtems
MV_SOC_PLATFORM = myriad2
DirLDScrCommon = $(MV_COMMON_BASE)/scripts/ld/$(MV_SOC_PLATFORM)memorySections

# ------------------------------[ Components used ]--------------------------------#
ComponentList = PipePrint FramePump FatalExtension

ifeq ($(MV_SOC_REV),ma2450)
 ComponentList += MV0212
else
 ComponentList += Board182
endif

#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no

# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to Latest
#MV_TOOLS_VERSION = Latest

#-------------------------------[ MDK COMMON ]---------------------------------------#
# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE:=../../../common
include ../../../common/generic.mk

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

CCOPT += -DCONFIG_TEARING_CHECK
#CCOPT += FPUMP_INVALIDATE_FRAME_CACHE

# Extra app related options
#CCOPT += -DNAME=VALUE
#CCOPT += -MD
#Don't clear BSS. This speeds up RTEMS a lot
#CCOPT += -DDONT_CLEAR_BSS
#CCOPT += -DFRAME_PUMP_DEBUG

# Ensure that the we are using the correct rtems libs etc
#LEON_ENDIAN =

TEST_TYPE := MANUAL
TEST_HW_PLATFORM := "MV0182_MA2150,  MV0212_MA2450"
TEST_TAGS := "MA2150, MA2450"
