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
ComponentList_LOS += vcsHooks UnitTest PipePrint

ComponentList_LRT += FatalExtension flic/core
ComponentList_LRT += vcsHooks UnitTest PipePrint

#Very "useful" shared folder convention (i.e. have to manually
#include files in there in the build)
LEON_APP_CPP_SOURCES += ./shared/tests.cpp
LEON_RT_CPP_SOURCES  += ./shared/tests.cpp

CCOPT     += -D'FLIC_2LEONS'
CCOPT_LRT += -D'FLIC_2LEONS'

UNIQUE_SYMBOLS += stubA skelB bLOS bLRT

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

TEST_OWNER := "andreil"
TEST_TYPE  :=  AUTO
TEST_TAGS  := "MA2150, MA2450, MA2480"