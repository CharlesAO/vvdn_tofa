ECHO ?= @
MV_SOC_REV      ?= ma2480
MV_COMMON_BASE  ?= ../../../../../common

ifeq ($(MV_SOC_REV), ma2480)
LinkerScript = ./config/custom_ma2x8x.ldscript
endif

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS     = rtems
MV_SOC_OS_LRT = rtems
LEON_RT_BUILD = yes

ComponentList_LOS += FatalExtension flic/core flic/common/rtemsCfg/cfg00
ComponentList_LRT += FatalExtension flic/core flic/common/rtemsCfg/cfg00

CCOPT     += -D'FLIC_2LEONS'
CCOPT_LRT += -D'FLIC_2LEONS'

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

TEST_OWNER := "andreil"
TEST_TYPE  :=  MANUAL
TEST_TAGS  := "MA2150, MA2450, MA2480"
