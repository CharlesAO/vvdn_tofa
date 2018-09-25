# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# Verbose or not, common out for more verbosity
ECHO ?=@

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
#ShaveSymObject   = $(DirAppObjDir)/$(APPNAME).sym.o

MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

ComponentList_LOS    := PipePrint FatalExtension

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems


# Use this makefile variable to specify the USB device class protos to be included into the build
# Available USB device protos: protohid protomsc protondfu protovideo protovsc2
MV_USB_PROTOS = protowmc
# Uncomment the following line to use the debug build of USB libraries
#RTEMS_USB_LIB_BUILD_TYPE = debug


#TEST_TYPE     := AUTO

# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to Latest
#MV_TOOLS_VERSION = Latest

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

CCOPT     += -D'DEFAULT_HEAP_SIZE=8192'

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT += -DNAME=VALUE
#CCOPT += -MD
#Don't clear BSS. This speeds up RTEMS a log
#CCOPT += -DDONT_CLEAR_BSS
#CCOPT += -DDISABLE_LEON_ICACHE -DDISABLE_LEON_DCACHE

TEST_TAGS:= "MA2150, MA2450"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
