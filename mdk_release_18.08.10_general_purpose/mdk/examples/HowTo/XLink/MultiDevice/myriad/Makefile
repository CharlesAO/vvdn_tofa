# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE ?= ../../../../../common

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_OS = rtems
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV ?= ma2450
RTEMS_BUILD_NAME = b-prebuilt

USE_USB_VSC ?= yes
ifeq ($(USE_USB_VSC),yes)
LEON_APP_URC_SOURCES +=  $(wildcard $(MV_COMMON_BASE)/components/USB_VSC/*leon/*.urc)
CCOPT += -D'USE_USB_VSC'
ComponentList_LOS += USB_VSC
MV_USB_PROTOS = protovsc2
# Use this makefile variable to specify the USB device class protos to be included into the build
# Available USB device protos: protohid protomsc protondfu protovideo protovsc2
else
LEON_APP_URC_SOURCES += $(wildcard $(MV_COMMON_BASE)/components/USB_CDC/*leon/*.urc)
CCOPT += -D'USE_USB_CDC'
ComponentList_LOS += USB_CDC
MV_USB_PROTOS = protowmc
endif

LEON_APP_C_SOURCES +=$(wildcard ./leon/*.c)
LEON_APP_C_SOURCES +=$(wildcard ../shared/*.c)
LEON_INCLUDE_PATH += ./leon/
LEON_INCLUDE_PATH += ../shared/

ComponentList_LOS += FatalExtension
ifeq ($(MV_SOC_REV), ma2480)
    LinkerScript = customMX.ldscript
endif

DirLDScrCommon ?= $(MV_COMMON_BASE)/scripts/ld/$(MV_SOC_PLATFORM)memorySections
ComponentList += PipePrint XLink

# Ensure that the we are using the correct rtems libs etc
#RTEMS_USB_LIB_BUILD_TYPE = debug

CCOPT_EXTRA = -D'USE_SOFTWARE=1'
CCOPT_EXTRA = -D'USB_XLINK_MEM_POOL_SIZE=10*1024*1024'
# ------------------------------[ Tools ]------------------------------------------#

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk
CCOPT += -D'DEFAULT_HEAP_SIZE=8192'

# Strip the image
stripimg:
	$(STRIP) -g $(DirAppOutput)/$(APPNAME).elf

usbLinkDebug: SourceDebugScript = ./scripts/usbLink_script.scr
usbLinkDebug: debug arg1 = USB

TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450, MV0235_MA2480"
TEST_TYPE := "MANUAL"
TEST_TAGS := "MA2150, MA2450, MA2480, MDK_COMPONENTS"
TEST_TIMEOUT_SEC := 90