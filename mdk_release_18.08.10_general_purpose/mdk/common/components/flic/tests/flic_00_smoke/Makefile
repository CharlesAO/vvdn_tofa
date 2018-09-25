MV_SOC_REV      ?= ma2480
MAKEDEPEND      ?= no

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

ifeq ($(MV_SOC_REV), ma2480)
LinkerScript = $(MV_COMMON_BASE)/components/flic/common/ldscripts/arch/2x8x/custom_ma2x8x_ddr.ldscript
endif

#Component lists:
ComponentList_LOS += VcsHooks UnitTest flic/core
ComponentList_LOS += flic/types FatalExtension PipePrint

MV_COMMON_BASE  ?= ../../../../../common
# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#FLIC settings
CCOPT += -D'DEFAULT_APP_CLOCK_KHZ=600000'

CCOPT += -D'DEF_POOL_SECT = ".bss"'
CCOPT += -D'DEF_POOL_SZ   = 8*1024'
CCOPT += -D'CANCEL_IO_PARENT_CHECK'

TEST_TYPE := AUTO
TEST_TAGS  := "MA2150, MA2450, MA2480, CDK"
TEST_HW_PLATFORM := "MV0212_MA2450, MV0235_MA2480"
