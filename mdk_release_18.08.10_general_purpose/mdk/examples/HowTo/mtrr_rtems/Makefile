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

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

ComponentList_LOS += PipePrint FatalExtension UnitTestVcs VcsHooks
# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to Latest
#MV_TOOLS_VERSION ?=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk


# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT +=


TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, MA2450"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450, MOVISIM_MA2150, MOVISIM_MA2450"
