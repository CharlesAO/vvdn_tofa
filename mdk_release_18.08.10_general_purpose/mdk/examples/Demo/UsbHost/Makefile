# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# Verbose or not, common out for more verbosity
ECHO ?=@

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk

MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

ComponentList_LOS    := PipePrint FatalExtension

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

# ------------------------------[ USB configuration ]--------------------------------#
# Device configuration:
#   Use MV_USB_PROTOS makefile variable to specify the USB device class protos to be
#   included into the build.
#   Available USB device protocols: protohid protomsc protondfu protovideo protovsc2
#   Example:
#     MV_USB_PROTOS = protovideo protohid # device using hid and video classes
#
# Host configuration:
#   Configure USB as host by setting RTEMS_USB_HOST_BUILD variable
#   By default host mode is disabled (RTEMS_USB_HOST_BUILD is set to no)
#   Example:
#     RTEMS_USB_HOST_BUILD = yes # USB is configured as host
#
# Note: only one configuration (either host or device) can be active at any time

# Device configuration
#MV_USB_PROTOS = protovideo
# Host configuration
RTEMS_USB_HOST_BUILD = yes

# Uncomment the following line to use the debug build of USB libraries
#RTEMS_USB_LIB_BUILD_TYPE = debug

# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to Latest
#MV_TOOLS_VERSION = Latest

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
CCOPT += -DRTEMS_USB_HOSTAPP_CREATE
CCOPT += -DRTEMS_USB_HOST_BIND_FILE_SYSTEM

TEST_TYPE := MANUAL
TEST_TAGS := "MA2150, MA2450"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"

