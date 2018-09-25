# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
# APPNAME ?= $(notdir $(shell pwd))

# Verbose or not, common out for more verbosity
ECHO=@

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_OS = rtems
MV_SOC_PLATFORM =myriad2
MV_SOC_REV=ma2450


# Use this makefile variable to specify the USB device class protos to be included into the build
# Available USB device protos: protohid protomsc protondfu protovideo protovsc2
MV_USB_PROTOS = protondfu

# Ensure that the we are using the correct rtems libs etc
#RTEMS_USB_LIB_BUILD_TYPE = debug
ComponentList   +=  FatalExtension
#CCOPT_EXTRA = -D'USE_SOFTWARE=1'

# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to Latest

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options
