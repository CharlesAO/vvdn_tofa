MV_SOC_REV     ?= ma2480
MAKEDEPEND     ?= no
MV_COMMON_BASE ?= ../../../../../common

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

ifeq ($(MV_SOC_REV), ma2480)
LinkerScript = $(MV_COMMON_BASE)/components/flic/common/ldscripts/arch/2x8x/custom_ma2x8x_ddr.ldscript
endif

#Component lists:
ComponentList_LOS += flic/common/rtemsCfg/cfg00
ComponentList_LOS += flic/core
ComponentList_LOS += flic/types
ComponentList_LOS += VcsHooks UnitTest

#FLIC settings
CCOPT += -D'DEFAULT_APP_CLOCK_KHZ=600000'

CCOPT += -D'DEF_POOL_SECT = ".cmx_direct.data"'
CCOPT += -D'DEF_POOL_SZ   = 4*1024'

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

TEST_OWNER := "andreil"
TEST_TYPE  :=  AUTO
TEST_TAGS  := "MA2150, MA2450, MA2480, CDK"
TEST_HW_PLATFORM := "MV0212_MA2450, MV0235_MA2480"
