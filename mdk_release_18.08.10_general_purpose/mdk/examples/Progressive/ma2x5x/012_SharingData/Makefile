# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
#ShaveSymObject   = $(DirAppObjDir)/$(APPNAME).sym.o

MV_SOC_PLATFORM = myriad2
MV_SOC_REV      = ma2450

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems
LEON_RT_BUILD = yes

ComponentList_LOS := UnitTestVcs VcsHooks PipePrint
ComponentList_LRT := UnitTestVcs VcsHooks PipePrint
ComponentList_LOS += FatalExtension

CCOPT += -DRESOURCE_FOLDER=\"$(MV_COMMON_BASE)/../resources/\"

#------[ Local shave applications sources ]-----#
SmallData_HEAPSIZE = 10*1024
SmallData_STACKSIZE = 6*1024

BigData_HEAPSIZE  = 36384
# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to Latest
#MV_TOOLS_VERSION = 00.50.42.5

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#------------------------[ Local shave applications build rules ]------------------#

# ------------------------------- [ Build Options ] ------------------------------ #
# App related build options

TEST_TYPE        := "AUTO"
TEST_TAGS        := "MDK_DRIVERS, MDK_COMPONENTS, MA2450, MOVISIM_LONGER_RUN_TEST_MA2450"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450, MOVISIM_MA2150, MOVISIM_MA2450"
