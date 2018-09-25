# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))
MV_SOC_PLATFORM = myriad2
MV_SOC_REV ?= ma2450

# ------------------------------[ Build overrides ]--------------------------------#

ComponentList_LOS += UnitTestVcs VcsHooks PipePrint

# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to Latest
#MV_TOOLS_VERSION =00.50.28.0

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE ?= ../../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT += -DNAME=VALUE

TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE := "AUTO"
TEST_TAGS:="MA2150,MA2155, MA2450"
