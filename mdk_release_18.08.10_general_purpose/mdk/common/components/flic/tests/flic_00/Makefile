MV_SOC_REV     ?= ma2480
MAKEDEPEND     ?= no
MV_COMMON_BASE ?= ../../../../../common

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

ifeq ($(MV_SOC_REV), ma2480)
LinkerScript = $(MV_COMMON_BASE)/components/flic/common/ldscripts/arch/2x8x/custom_ma2x8x_ddr.ldscript
endif

# Component lists:
ComponentList_LOS += flic/core flic/common/rtemsCfg/cfg00

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

TEST_OWNER := "andreil"
TEST_TYPE  :=  MANUAL
TEST_TAGS  := "MA2150, MA2450, MA2480"
