MDK_BASE = ../../../../../../../..
MV_SOC_REV ?= ma2150

ifeq ($(MV_SOC_REV), ma2150)
 MVSOC         = -DMA2150 -DBUGZILLA_22709 -DBUGZILLA_22593
else ifeq ($(MV_SOC_REV), ma2155)
 MVSOC         = -DMA2155 -DBUGZILLA_22709 -DBUGZILLA_22593
else ifeq ($(MV_SOC_REV), ma2450)
 MVSOC         = -DMA2450 -DBUGZILLA_22709 -DBUGZILLA_22593
else ifeq ($(MV_SOC_REV), ma2455)
 MVSOC         = -DMA2455 -DBUGZILLA_22709 -DBUGZILLA_22593
endif

SIPP_BASE     = $(MDK_BASE)/common/components/sipp
MDK_MVSOC_INC = $(MDK_BASE)/common/drivers/myriad2/socDrivers/leon/bm/arch/ma2x5x/include

TARGET_RTOS ?= 0

SIPP_PKG_BASE = $(MDK_BASE)/packages/movidius/sipp/arch/ma2x5x/swSipp
HWMODELS_PKG_BASE = $(MDK_BASE)/packages/movidius/sipp/arch/ma2x5x/hwModels

ifndef UNAME
UNAME := $(shell uname -o)
endif

ifeq "$(UNAME)" "GNU/Linux"
# Linux platform detected. Select appropriate path
LIB_PREBUILD_DIR = $(SIPP_PKG_BASE)/lib/linux
else
# Not Linux
LIB_PREBUILD_DIR = $(SIPP_PKG_BASE)/lib/cygwin
endif

SIPP_OBJS =     sippDbg.o \
    sippError.o \
    sippApi.o \
    sippApiInternal.o \
    sippScheduleIsr.o \
    sippIsr.o \
    sippManagerApi.o \
    sippAccessScheduler.o \
    sippHWSessionControl.o \
    sippPipeSessionControl.o \
    sippPipelineValidate.o \
    sippCoreApi.o \
    sippCoreGlobals.o \
    sippCoreUtils.o \
    sippCoreHw.o \
    sippPlatformAbstractionLayer.o \
    sippHeap.o \
    sippMem.o \
    sippMemLineBuffer.o \
    sippCmxDmaIf.o \
    sippShaveIf.o \
    sippTestCommon.o \
    sippShave.o \
    sippIoPtrs.o \
    sippFilterAccesors.o \
    sippHpad.o \
    sippUtils.o \
    sippHwChromaDns.o \
    sippHwColComb.o \
    sippHwCommon.o \
    sippHwConv.o \
    sippHwDebayer.o \
    sippHwDogLtm.o \
    sippHwEdge.o \
    sippHwGenChroma.o \
    sippHwHarris.o \
    sippHwLsc.o \
    sippHwLumaDns.o \
    sippHwLut.o \
    sippHwMedian.o \
    sippHwMipiRx.o \
    sippHwMipiTx.o \
    sippHwPolyFir.o \
    sippHwRaw.o \
    sippHwSigma.o \
    sippHwUnsharp.o \
    sippAnalysePipema2x5x.o \
    sippOPipeSchedulingEntity.o \
    sippGenericRuntime.o \
    sippGenericRuntimema2x5x.o \
    sippGenericSchApi.o \
    sippGenericSchReq.o \
    sippGenericSchWrite.o \
    sippGenericSchDebug.o \
    sippMLPIRuntime.o \
    sippMLPIRuntimema2x5x.o \
    sippMLPISchApi.o \
    sippMLPISchReq.o \
    sippMLPISchWrite.o \
    sippMLPISchDebug.o \
    sippOPipeRuntime.o \
    sippOPipeSchApi.o \
    wrapperSem.o

COMMON_OBJS = half.o

INCFLAGS = -I$(SIPP_BASE)/include \
		-I$(HWMODELS_PKG_BASE)/include \
		-I$(HWMODELS_PKG_BASE)/include/sippMipiTxStates/ \
		-I$(HWMODELS_PKG_BASE)/include/sippStrategies \
		-I$(HWMODELS_PKG_BASE)/include/sippBuilders \
		-I$(HWMODELS_PKG_BASE)/include/sippDecorators \
		-I$(HWMODELS_PKG_BASE)/include/sippVisitors \
		-I$(HWMODELS_PKG_BASE)/include/cmxdmaInc \
		-I$(MDK_BASE)/common/drivers/myriad2/socDrivers/leon/bm/include \
		-I$(MDK_BASE)/common/swCommon/pcModel/half \
		-I$(MDK_BASE)/common/swCommon/include \
		-I$(MDK_BASE)/common/shared/include \
		-I$(MDK_MVSOC_INC) \
		-I$(SIPP_BASE)/arch/ma2x5x/src/leon \
		-I$(SIPP_BASE)/arch/ma2x5x/include \
		-I$(SIPP_BASE)/include \
		-I$(SIPP_BASE)/arch/ma2x5x/testUtils \
		-I$(SIPP_BASE)/arch/ma2x5x/PAL/PC

CFLAGS  += -Wall -g -O0 -m32 -fpermissive -Wno-unknown-pragmas -Wno-comment -fopenmp $(MVSOC)
#CFLAGS  += -Wall -g -O3 -m32 -fpermissive -Wno-unknown-pragmas -Wno-comment -fopenmp $(MVSOC)
CFLAGS  += -DSIPP_RTOS=$(TARGET_RTOS) -DSIPP_NO_RTOS=0 -DSIPP_RTEMS=1 -D_MV_TYPES_H_
LDFLAGS += -Wall $(LIBS) -Wl,-rpath=$(LD_LIBRARY_PATH)
LIBS    += -lpthread -lm

CFLAGS  += -DSIPP_SCRT_ENABLE_MLPI -DSIPP_NO_IRQ_BARRIER2 -DSIPP_SCRT_ENABLE_OPIPE

######################################################
# BUILD OPTION COMPILER FLAGS

######################################################

# Rules for compiling SIPP framework C files
CC_CMD_SIPP = c++ $(CFLAGS) $(INCFLAGS) -c $<

%.o:	$(SIPP_BASE)/arch/ma2x5x/src/leon/%.c
	$(CC_CMD_SIPP)
%.o:	$(SIPP_BASE)/arch/ma2x5x/src/shave/%.c
	$(CC_CMD_SIPP)
%.o:	$(SIPP_BASE)/arch/ma2x5x/PAL/PC/%.c
	$(CC_CMD_SIPP)
%.o:	$(SIPP_BASE)/arch/ma2x5x/testUtils/%.cpp
	$(CC_CMD_SIPP)
%.o:	$(MDK_BASE)/common/swCommon/pcModel/half/%.cpp
	$(CC_CMD_SIPP)

all:	libsipp.a

libsipp.a:	$(SIPP_OBJS) $(COMMON_OBJS)
	ar r libsipp.a $(SIPP_OBJS) $(COMMON_OBJS)
	cp libsipp.a $(LIB_PREBUILD_DIR)

clean:
	rm -f libsipp.a *.o

MDK_PROJ_STRING ?= "MDK_FALSE"
TEST_TAGS := "MA2150"
getTestType:

isMdkProject:
	@echo $(MDK_PROJ_STRING)
