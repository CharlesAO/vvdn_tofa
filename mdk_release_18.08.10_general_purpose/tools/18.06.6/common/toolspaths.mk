#Set the name and version of the Sparc tools - force overwrites, so that the previous MDK settings don't take precedence
MV_GCC_TOOLS:=sparc-myriad-rtems
GCCVERSION:=6.3.0
SPARC_DIR:=$(MV_GCC_TOOLS)-$(GCCVERSION)

MV_TOOLS_MOVI_COMPILE_LIB_PATH := $(MV_TOOLS_BASE)/common/moviCompile/lib

MV_GCC_TOOLS_BASE ?= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)

MVLNK           = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviLink
MVASM           = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviAsm
MVDUMP          = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviDump
MVSIM           = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviSim
MVDBG1          = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviDebug
MVDBG2          = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviDebug2
MVDBGTCL        = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviDebugTcl
MVCONV          = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviConvert
MVSVR           = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviDebugServer
MVSCAN          = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/scan-build

MVCC            = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviCompile

# Sparc tools
CC              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-gcc
LD              = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-ld
OBJCOPY         = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-objcopy
AS              = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-as
AR              = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-ar
STRIP           = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-strip
OBJDUMP         = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-objdump
CXX             = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-g++
READELF         = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-readelf
CPP_FILT        = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-c++filt

MOVI_COMPILE_LDSCR = $(MV_TOOLS_DIR)/$(MV_TOOLS_VERSION)/common/moviCompile/ldscripts/shave_first_phase.ld

#define MVISAOPT
ifeq ($(MV_SOC_PLATFORM), myriad1)
    MVISAOPT      ?= ma1x00
else
    ifeq ($(MV_SOC_REV), ma2100)
        MVISAOPT   ?= ma2100
    else ifeq ($(MV_SOC_REV), ma2150)
        MVISAOPT   ?= ma2x5x
    else ifeq ($(MV_SOC_REV), ma2155)
        MVISAOPT   ?= ma2x5x
    else ifeq ($(MV_SOC_REV), ma2450)
        MVISAOPT   ?= ma2x5x
    else ifeq ($(MV_SOC_REV), ma2455)
        MVISAOPT   ?= ma2x5x
    else ifeq ($(MV_SOC_REV), ma2480)
        MVISAOPT   ?= ma2x8x
    else ifeq ($(MV_SOC_REV), ma2485)
        MVISAOPT   ?= ma2x8x
    else ifeq ($(MV_SOC_REV), ma2490)
        MVISAOPT   ?= ma2x9x
    else ifeq ($(MV_SOC_REV), ma2495)
        MVISAOPT   ?= ma2x9x
    endif
endif

# Path to RTEMS Libraries and header files
MV_RTEMS_LOS_LIB ?= $(MV_TOOLS_BASE)/common/rtems/prebuilt/release/$(MVISAOPT)/lib
MV_RTEMS_LRT_LIB ?= $(MV_TOOLS_BASE)/common/rtems/prebuilt/release/$(MVISAOPT)/lib

#It's ok to define MLNKOPT now
ifeq ($(TOOLS_VERSION_TYPE), $(OLDER_MOVI_COMPILE_FOLDER_STRUCT))
    MVLNKOPT += -I:$(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MV_SOC_PLATFORM)
else
    ifeq ($(TOOLS_VERSION_TYPE), $(NEWER_MOVI_COMPILE_FOLDER_STRUCT))
        MVLNKOPT += -I:$(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)
    endif
endif

#################################################################################
#     Common libraries needed by tools to link all functionality                #
#################################################################################
#Leon common libraries
# Support for RTEMS

ifeq ($(MV_SOC_OS), rtems)
  # Include RTEMS libraries LEON OS
  ifeq ($(MVISAOPT), ma2x8x)
    ifeq ($(MV_SOC_OS_LIBBSD), 1)
      DefaultSparcRTEMSLibs += $(MV_RTEMS_LOS_LIB)/libbsd.a
    endif
  endif
  DefaultSparcRTEMSLibs += $(MV_RTEMS_LOS_LIB)/librtemsbsp.a
  DefaultSparcRTEMSLibs += $(MV_RTEMS_LOS_LIB)/librtemscpu.a
  DefaultSparcRTEMSLibs += $(MV_RTEMS_LOS_LIB)/start.o

  # Include Newlib Libraries LEON OS
  DefaultSparcRTEMSLibs += $(wildcard $(MV_GCC_TOOLS_BASE)/$(MV_GCC_TOOLS)/lib/*.a)
  DefaultSparcRTEMSLibs += $(wildcard $(MV_GCC_TOOLS_BASE)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/*.a)
  # Include crti, crtn, crtbegin and crtend for RTEMS
  MV_RTEMS_PROLOGUE = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/crti.o
  MV_RTEMS_PROLOGUE += $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/crtbegin.o
  MV_RTEMS_EPILOGUE = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/crtend.o
  MV_RTEMS_EPILOGUE += $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/crtn.o
else
  # Not used for bare-metal
  MV_RTEMS_PROLOGUE =
  MV_RTEMS_EPILOGUE =
endif

ifeq ($(MV_SOC_OS_LRT), rtems)
  # Include RTEMS Libraries LEON RT
  ifeq ($(MVISAOPT), ma2x8x)
    ifeq ($(MV_SOC_OS_LRT_LIBBSD), 1)
      DefaultSparcRTEMSLibsRT += $(MV_RTEMS_LRT_LIB)/libbsd.a
    endif
  endif
  DefaultSparcRTEMSLibsRT  += $(MV_RTEMS_LRT_LIB)/librtemsbsp.a
  DefaultSparcRTEMSLibsRT  += $(MV_RTEMS_LRT_LIB)/librtemscpu.a
  DefaultSparcRTEMSLibsRT  += $(MV_RTEMS_LRT_LIB)/start.o

  # Include Newlib Libraries LEON RT
  DefaultSparcRTEMSLibsRT  += $(wildcard $(MV_GCC_TOOLS_BASE)/$(MV_GCC_TOOLS)/lib/*.a)
  DefaultSparcRTEMSLibsRT  += $(wildcard $(MV_GCC_TOOLS_BASE)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/*.a)
  # Include crti, crtn, crtbegin and crtend for RTEMS
  MV_RTEMS_PROLOGUE_LRT = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/crti.o
  MV_RTEMS_PROLOGUE_LRT += $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/crtbegin.o
  MV_RTEMS_EPILOGUE_LRT = $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/crtend.o
  MV_RTEMS_EPILOGUE_LRT += $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/crtn.o
else
  # Not used for bare-metal
  MV_RTEMS_PROLOGUE_LRT =
  MV_RTEMS_EPILOGUE_LRT =
endif

CompilerANSILibs  = $(MV_TOOLS_BASE)/common/moviCompile/lib/$(MVISAOPT)/mlibcxx.a
CompilerANSILibs += $(MV_TOOLS_BASE)/common/moviCompile/lib/$(MVISAOPT)/mlibVecUtils.a

CompilerANSILibs += --start-group
CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibc_lite.a
CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibc_lite_lgpl.a
CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibm.a
CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibcrt.a
CompilerANSILibs += --end-group
