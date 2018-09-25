MDK_BASE = $(LINUX_BUILD_BASE)/../../../../../../..

ifeq ($(MV_SOC_REV), ma2150)
 MVSOC         = -DMA2150 -DBUGZILLA_22709
else ifeq ($(MV_SOC_REV), ma2155)
 MVSOC         = -DMA2155
else ifeq ($(MV_SOC_REV), ma2450)
 MVSOC         = -DMA2450
else ifeq ($(MV_SOC_REV), ma2455)
 MVSOC         = -DMA2455
endif

MDK_SIPP_INC = $(MDK_BASE)/common/components/sipp
MDK_MVSOC_INC = $(MDK_BASE)/common/drivers/myriad2/socDrivers/leon/bm/arch/ma2x5x/include

TARGET_RTOS ?= 0

SIPP_PKG_BASE = $(MDK_BASE)/packages/movidius/sipp/arch/ma2x5x
HWMODELS_PKG_INC_BASE = $(SIPP_PKG_BASE)/hwModels/include
HWMODELS_PKG_LIB_BASE = $(SIPP_PKG_BASE)/hwModels/lib
SWSIPP_PKG_LIB_BASE = $(SIPP_PKG_BASE)/swSipp/lib

ifndef UNAME
UNAME := $(shell uname -o)
endif
ifeq ($(UNAME),GNU/Linux)
  # Linux platform detected. Select appropriate path
  HWMODELS_LIB_PREBUILD_DIR = $(HWMODELS_PKG_LIB_BASE)/linux
  SWSIPP_LIB_PREBUILD_DIR = $(SWSIPP_PKG_LIB_BASE)/linux
  # also correctly specify 32-bit compilation on 64-bit platforms
  FULL_PLATFORM := $(shell uname -a)
  ifeq ($(findstring x86_64,$(FULL_PLATFORM)),x86_64)
    X86_64_CFLAGS := -m32
    # FIXME this one should be tested but usually 64-bit systems offer 32-bit
    # libraries on lib32 directory
    X86_64_LDFLAGS := -L/usr/lib32
  else
    X86_64_CFLAGS := ''
    X86_64_LDFLAGS := ''
  endif
else
  # Not Linux
  HWMODELS_LIB_PREBUILD_DIR = $(HWMODELS_PKG_LIB_BASE)/cygwin
  SWSIPP_LIB_PREBUILD_DIR = $(SWSIPP_PKG_LIB_BASE)/cygwin
endif

# detect and activate CCACHE
CCACHE := $(shell if [ -z "$(MV_USE_CCACHE)" ]; then echo ""; else which ccache 2>/dev/null || echo ""; fi)
ifneq (, $(CCACHE))
  $(info ccache was found on your system and it will be used to enhance the build time )
endif

# here we use GNU make CXX which is by default g++ but it can be overridden if
# we wrote it like this
CXX := $(CCACHE) $(CXX)

#Sipp GUI variables for extracting paths to the filters and object names
MKNAME = $(shell  find ../../leon -type f -iname "*.sipp" -print0|cut -d'/' -f4|cut -d'.' -f1)
PATH_FILTER += $(shell grep 'filters' ../sipp/$(MKNAME).mk | grep -v 'asm' | awk -F/ 'BEGIN {OFS = "/";} {print "$(SIPP_BASE)/"$$2,$$3}')
PATH_SHAVE  += $(shell grep 'filters' ../sipp/$(MKNAME).mk | grep -v 'asm' | awk -F/ 'BEGIN {OFS = "/";} {print "$(SIPP_BASE)/"$$2,$$3,$$4}')
OBJS = $(shell grep 'filters' ../sipp/$(MKNAME).mk | grep -v 'asm' |awk -F/ '{print $$5}'| sed 's/...$$/.o/')


INCFLAGS += -I$(HWMODELS_PKG_INC_BASE) \
		-I$(HWMODELS_PKG_INC_BASE)/sippMipiTxStates/ \
		-I$(HWMODELS_PKG_INC_BASE)/sippStrategies \
		-I$(HWMODELS_PKG_INC_BASE)/sippBuilders \
		-I$(HWMODELS_PKG_INC_BASE)/sippDecorators \
		-I$(HWMODELS_PKG_INC_BASE)/sippVisitors \
		-I$(MDK_BASE)/common/drivers/myriad2/socDrivers/leon/bm/include \
		-I$(MDK_BASE)/common/shared/include \
		-I$(MDK_BASE)/common/swCommon/include \
		-I$(MDK_BASE)/common/swCommon/pcModel/half \
		-I$(MDK_SIPP_INC) \
		-I$(MDK_SIPP_INC)/include \
		-I$(MDK_SIPP_INC)/arch/ma2x5x/include \
		-I$(MDK_SIPP_INC)/arch/ma2x5x/src/leon \
		-I$(MDK_SIPP_INC)/arch/ma2x5x/PAL/PC \
		-I$(MDK_MVSOC_INC)

CFLAGS := $(X86_64_CFLAGS) -Wall -Wno-unknown-pragmas -O0 -g -fpermissive -fopenmp $(MVSOC)

CFLAGS += -DSIPP_RTOS=$(TARGET_RTOS) -DSIPP_NO_RTOS=0 -DSIPP_RTEMS=1 -D_MV_TYPES_H_

LDFLAGS := -L$(SWSIPP_LIB_PREBUILD_DIR) -lsipp -L$(HWMODELS_LIB_PREBUILD_DIR) $(X86_64_LDFLAGS) -lhwmodels -lpthread $(INCFLAGS)

VPATH += $(SIPP_APP_SRC_DIRS)

all:	$(SIPP_APP)

$(SIPP_APP):	$(SIPP_APP_OBJS)
	$(CXX) $(CFLAGS) -o $(SIPP_APP) $(SIPP_APP_OBJS) $(LDFLAGS)

%.o:	%.c
	$(CXX) $(CFLAGS) -c $< $(INCFLAGS)

%.o:	%.cpp
	$(CXX) $(CFLAGS) -c $< $(INCFLAGS)

xxxx:
	@echo $(SWSIPP_LIB_PREBUILD_DIR)

.PHONY:	reallyclean
reallyclean: clean
	rm -f $(SIPP_APP) $(SIPP_APP).exe filt_out*.raw pcDumpSchedule.hh pipeline.dot sippLayout.html isp_out_*.raw isp_out_*.raw out_*.rgb

.PHONY:	clean
clean: oclean
	rm -f *.o $(SIPP_APP) $(SIPP_APP).exe filt_out*.raw pcDumpSchedule.hh pipeline.dot myriad2SippDefs.inc sippLayout.html out_*.raw out_*.rgb

.PHONY:	oclean
oclean:
#findRes := $(shell )
	find $(SIPP_APP_SRC_DIRS) -name '*.o' -exec rm -f {} \;

# vim: set expandtab:ts=2:sw=2
