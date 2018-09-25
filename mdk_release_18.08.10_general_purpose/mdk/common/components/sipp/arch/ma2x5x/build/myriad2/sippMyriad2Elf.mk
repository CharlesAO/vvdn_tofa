# Note: could create output directory like this:
# CREATE_OUT_DIR := $(shell mkdir -p ./output)

TARGET_RTOS_LOS ?= 0
TARGET_RTOS_LRT ?= 0
ifneq ($(MV_SIPP_OS_ORIG), yes)
ifeq ($(MV_SOC_OS), rtems)
TARGET_RTOS_LOS = 1
endif
ifeq ($(MV_SOC_OS_LRT), rtems)
TARGET_RTOS_LRT = 1
endif
endif

SIPP_APP_NUM_IMGS ?=1

#########################################################################
# Specify C headers search dirs:
#########################################################################
SIPP_DIR = $(MV_COMMON_BASE)/components/sipp
SIPP_BASE = $(SIPP_DIR)
# Sipp path for the Sipp Designer
SIPP_PATH_1 ?= $(SIPP_DIR)

# Directory path for ASM kernels
MV_SOC_REV_DIR := ma2x5x

#Includes C cources
ComponentList_LOS += sipp/arch/ma2x5x/src
ComponentList_LOS += UnitTestVcs VcsHooks
ComponentList_LRT += sipp/arch/ma2x5x/src
ComponentList_LRT += UnitTestVcs VcsHooks
ComponentList_SVE += sipp/arch/ma2x5x/src

ifeq ($(TARGET_RTOS_LOS),0)
ComponentList_LOS += sipp/arch/ma2x5x/PAL/BM/src
else
ComponentList_LOS += sipp/arch/ma2x5x/PAL/RTEMS/src
endif

ifeq ($(TARGET_RTOS_LRT),0)
ComponentList_LRT += sipp/arch/ma2x5x/PAL/BM/src
else
ComponentList_LRT += sipp/arch/ma2x5x/PAL/RTEMS/src
endif

#Leon includes
CCOPT        += -I$(SIPP_DIR)
CCOPT        += -I$(SIPP_DIR)/include
CCOPT        += -I$(SIPP_DIR)/arch/ma2x5x/include
CCOPT        += -I$(SIPP_DIR)/arch/ma2x5x/src/leon
CCOPT_LRT    += -I$(SIPP_DIR)
CCOPT_LRT    += -I$(SIPP_DIR)/include
CCOPT_LRT    += -I$(SIPP_DIR)/arch/ma2x5x/include
CCOPT_LRT    += -I$(SIPP_DIR)/arch/ma2x5x/src/leon
ifeq ($(TARGET_RTOS_LOS),0)
CCOPT        += -I$(SIPP_DIR)/arch/ma2x5x/PAL/BM/src/leon
else ifeq ($(TARGET_RTOS_LOS),1)
CCOPT        += -I$(SIPP_DIR)/arch/ma2x5x/PAL/RTEMS/src/leon
endif
ifeq ($(TARGET_RTOS_LRT),0)
CCOPT_LRT    += -I$(SIPP_DIR)/arch/ma2x5x/PAL/BM/src/leon
else ifeq ($(TARGET_RTOS_LRT),1)
CCOPT_LRT    += -I$(SIPP_DIR)/arch/ma2x5x/PAL/RTEMS/src/leon
endif

ifeq ($(MV_SOC_REV), ma2150)
   CCOPT                       += -DMA2150
   CCOPT_LRT                   += -DMA2150
   MVCCOPT                     += -DMA2150
   MVCCOPT_LRT                 += -DMA2150
else ifeq ($(MV_SOC_REV), ma2155)
   CCOPT                       += -DMA2155
   CCOPT_LRT                   += -DMA2155
   MVCCOPT                     += -DMA2155
   MVCCOPT_LRT                 += -DMA2155
else ifeq ($(MV_SOC_REV), ma2450)
   CCOPT                       += -DMA2450
   CCOPT_LRT                   += -DMA2450
   MVCCOPT                     += -DMA2450
   MVCCOPT_LRT                 += -DMA2450
else ifeq ($(MV_SOC_REV), ma2455)
   CCOPT                       += -DMA2455
   CCOPT_LRT                   += -DMA2455
   MVCCOPT                     += -DMA2455
   MVCCOPT_LRT                 += -DMA2455
endif

# MoviCompile options
MVCCOPT    += -I$(SIPP_DIR)/arch/ma2x5x/include
MVCCOPT    += -I$(SIPP_DIR)/arch/ma2x5x/src/leon
MVCCOPT    += -I$(SIPP_DIR)/include
MVCCOPT    += -I$(SIPP_DIR)

MVCCOPT_LRT    += -I$(SIPP_DIR)/arch/ma2x5x/include
MVCCOPT_LRT    += -I$(SIPP_DIR)/arch/ma2x5x/src/leon
MVCCOPT_LRT    += -I$(SIPP_DIR)/include
MVCCOPT_LRT    += -I$(SIPP_DIR)

ifeq ($(TARGET_RTOS_LOS),0)
MVCCOPT        += -I$(SIPP_DIR)/arch/ma2x5x/PAL/BM/src/leon
else ifeq ($(TARGET_RTOS_LOS),1)
MVCCOPT        += -I$(SIPP_DIR)/arch/ma2x5x/PAL/RTEMS/src/leon
endif
ifeq ($(TARGET_RTOS_LRT),0)
MVCCOPT    += -I$(SIPP_DIR)/arch/ma2x5x/PAL/BM/src/leon
else ifeq ($(TARGET_RTOS_LRT),1)
MVCCOPT    += -I$(SIPP_DIR)/arch/ma2x5x/PAL/RTEMS/src/leon
endif

#For inline asm, tell moviAsm where the location of "myriad2SippDefs.inc"
MVASM_INCLUDE += $(SIPP_DIR)/arch/ma2x5x/src/shaveM2

#Leon SIPP defs
# "You can use the -U option with gcc" in the test makefile to undefine
#  defs that are specified in command line (such as SIPP_VCS or SIPP_TEST_APP)
# Or, user can override defaults in this file:
CCOPT       += -DSIPP_NUM_IMGS=$(SIPP_APP_NUM_IMGS)
CCOPT       += -DSIPP_VCS
CCOPT       += -DSIPP_TEST_APP
CCOPT       += -DSIPP_CMX_POOL_SZ=32768
CCOPT       += -DBUFF_HUGE_SZ=128
CCOPT       += -DSIPP_SCRT_ENABLE_MLPI
CCOPT       += -DSIPP_NO_IRQ_BARRIER2
CCOPT_LRT   += -DSIPP_NUM_IMGS=$(SIPP_APP_NUM_IMGS)
CCOPT_LRT   += -DSIPP_VCS
CCOPT_LRT   += -DSIPP_TEST_APP
CCOPT_LRT   += -DSIPP_CMX_POOL_SZ=32768
CCOPT_LRT   += -DBUFF_HUGE_SZ=128
CCOPT_LRT   += -DSIPP_SCRT_ENABLE_MLPI
CCOPT_LRT   += -DSIPP_NO_IRQ_BARRIER2

CCOPT_LRT   += -DSIPP_RTOS=$(TARGET_RTOS_LRT) -DSIPP_NO_RTOS=0 -DSIPP_RTEMS=1
CCOPT       += -DSIPP_RTOS=$(TARGET_RTOS_LOS) -DSIPP_NO_RTOS=0 -DSIPP_RTEMS=1

#Set heap size to a smaller level size as it's only
#used in small amounts
CCOPT_LRT += -DDEFAULT_HEAP_SIZE=512
CCOPT     += -DDEFAULT_HEAP_SIZE=512


CCOPT       += -DBUGZILLA_22709 -DBUGZILLA_22593
CCOPT_LRT   += -DBUGZILLA_22709 -DBUGZILLA_22593
MVCCOPT     += -DBUGZILLA_22709 -DBUGZILLA_22593
MVCCOPT_LRT += -DBUGZILLA_22709 -DBUGZILLA_22593

#########################################################################
# Shave objects
#########################################################################

SippSvuObj  = $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x5x/src/shave/sippShave_shave.o
SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x5x/src/shave/sippSvuSync_shave.o
SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x5x/src/shave/sippShvDbg_shave.o

# Choose c- based protyping versions
ifneq ($(findstring SIPP_USE_C_PTR_GET_FUNCS,$(MVCCOPT)),)
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x5x/src/shave/sippIoPtrs_shave.o
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x5x/src/shave/sippHpad_shave.o
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x5x/src/shave/sippFilterAccesors_shave.o
else
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x5x/src/shave/sippHpadOpt_shave.o
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x5x/src/shave/sippFilterAccesorsOpt_shave.o
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x5x/src/shave/sippIoPtrsOpt_shave.o
endif

# This rule is added so that make picks up the correct mdk default target
# as this file is normally included in projects prior to generic.mK
default_target:
	$(MAKE) all

getSym:
	$(OBJDUMP) -t ./$(ELF_FILE) | sort > ./$(DirAppOutput)/$(APPNAME)_leon.sym
