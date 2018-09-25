# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk

MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450
LEON_RT_BUILD = yes

MV_SOC_OS = rtems

ComponentList_LOS := PipePrint FatalExtension UnitTestVcs VcsHooks
ComponentList_LRT := PipePrint
#-------------------------------[ Local applications sources ]--------------------#
#Just include the apps own makefile bits
include ./shave/diff_sharing/diff_sharing_sources.mk
include ./shave/global_sharing/global_sharing_sources.mk

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(diff_sharing).mvlib $(global_sharing).mvlib $(calling_conv_add).mvlib
SHAVE0_APPS = $(diff_sharing).shv0lib
SHAVE1_APPS = $(diff_sharing).shv1lib
#"uniq" libraries are libraries the user defines for himself which keep
#some symbols unique so that they may be shared between many apps
SHAVE2_APPS = $(global_sharing).shv2uniqlib
SHAVE3_APPS = $(global_sharing).shv3uniqlib

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =00.50.37.11

# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE   ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local applications build rules ]------------------#
include ./shave/diff_sharing/diff_sharing_rules.mk
include ./shave/global_sharing/global_sharing_rules.mk

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
CCOPT			+= -DDEBUG

TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, MA2450"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450, MOVISIM_MA2150, MOVISIM_MA2450"
