MV_SOC_REV     ?= ma2480
MAKEDEPEND     ?= no
MV_COMMON_BASE ?= ../../../../../common

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

ifeq ($(MV_SOC_REV), ma2480)
LinkerScript = $(MV_COMMON_BASE)/components/flic/common/ldscripts/arch/2x8x/custom_ma2x8x_ddr.ldscript
endif

# Debug alloc options
CCOPT += -D'ALLOC_DEBUG'
CCOPT += -D'ALLOC_MAX=256'
CCOPT += -D'ALLOC_PRINT=0'

# Component lists:
ComponentList_LOS += flic/core flic/common/rtemsCfg/cfg00 UnitTest

#FLIC settings
CCOPT += -D'DEFAULT_APP_CLOCK_KHZ=600000'

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

TEST_OWNER := "andreil"
TEST_TYPE  :=  AUTO
TEST_TAGS  := "MA2150, MA2450, MA2480, CDK"
TEST_HW_PLATFORM := "MV0212_MA2450, MV0235_MA2480"
