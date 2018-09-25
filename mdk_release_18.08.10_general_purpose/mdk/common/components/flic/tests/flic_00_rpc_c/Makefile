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

ComponentList_LOS += FatalExtension flic/core
ComponentList_LOS += vcsHooks UnitTest

ComponentList_LRT += FatalExtension flic/core
ComponentList_LRT += vcsHooks UnitTest

CCOPT     += -D'FLIC_2LEONS' -D'MAX_PLUGS_PER_PIPE=40'
CCOPT_LRT += -D'FLIC_2LEONS' -D'MAX_PLUGS_PER_PIPE=40'

ifeq ($(MV_SOC_REV), ma2480)
CCOPT     += -D'DYN_IRQ_ASSIGNMENT=1'
CCOPT_LRT += -D'DYN_IRQ_ASSIGNMENT=1'
endif

UNIQUE_SYMBOLS += osStub osSkel rtStub rtSkel

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

TEST_OWNER := "andreil"
TEST_TYPE  :=  AUTO
TEST_TAGS  := "MA2150, MA2450, MA2480, CDK"
TEST_HW_PLATFORM := "MV0212_MA2450, MV0235_MA2480"
