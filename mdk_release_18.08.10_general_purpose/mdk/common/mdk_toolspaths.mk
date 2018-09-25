#Makefile containing the same config information as in
#$(MV_TOOLS_BASE)/common/toolspaths.mk; present also in mdk/common because of
#compatibility issues with older tools that do not have
#$(MV_TOOLS_BASE)/common/toolspaths.mk

OLDER_MOVI_COMPILE_FOLDER_STRUCT := 0
NEWER_MOVI_COMPILE_FOLDER_STRUCT := 1

#VPU2 tools need thsi overwrite same as toolspaths.
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
SPARC_DIR:=$(MV_GCC_TOOLS)-$(GCCVERSION)
endif

TOOLS_VERSION_TYPE := $(shell if [ -d $(MV_TOOLS_BASE)/common/moviCompile/libraries ]; then echo $(OLDER_MOVI_COMPILE_FOLDER_STRUCT); elif [ -d $(MV_TOOLS_BASE)/common/moviCompile/lib ]; then echo $(NEWER_MOVI_COMPILE_FOLDER_STRUCT); fi)

ifeq ($(TOOLS_VERSION_TYPE), $(OLDER_MOVI_COMPILE_FOLDER_STRUCT))
    MV_TOOLS_MOVI_COMPILE_LIB_PATH := $(MV_TOOLS_BASE)/common/moviCompile/libraries
else
    ifeq ($(TOOLS_VERSION_TYPE), $(NEWER_MOVI_COMPILE_FOLDER_STRUCT))
        MV_TOOLS_MOVI_COMPILE_LIB_PATH := $(MV_TOOLS_BASE)/common/moviCompile/lib
    endif
endif

# Extract the prebuilts' prevision from its path
PREBUILT_REV = $(lastword $(subst /, ,$(shell find $(call RESOLVE_TARGET_PATH,$(MV_TOOLS_BASE)/common/rtems/prebuilt) -type d -name "ma2150")))

MV_GCC_TOOLS_BASE ?= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)

MVASM     		= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviAsm
MVDUMP    		= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviDump
MVSIM     		= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviSim
MVDBG1     		= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviDebug
MVDBG2     		= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviDebug2
MVDBGTCL  		= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviDebugTcl
ifeq ($(MV_CPU_ARCH),armv8)
MVCONV          = ~/WORK/Tools/Releases/General/00.87.1/linux64/bin/moviConvert
else
MVCONV  		= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviConvert
endif
ifeq ($(DO_I_KNOW_SECRET_RTL_PEOPLE_HANDSHAKE),yes)
MVCONV64        = ~/WORK/keembay/bin/moviConvert64
else
MVCONV64        = echo
endif
MVSVR     		= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviDebugServer

MVCC      		= $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviCompile


ifeq ($(MV_CPU_ARCH),sparc)
#Aaaand just as we're used to: VPU2 tools creator masters and onwards want to bend the rules
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
# Sparc tools
CC              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-gcc
LD              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-ld
OBJCOPY         = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-objcopy
AS              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-as
AR              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-ar
STRIP           = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-strip
OBJDUMP         = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-objdump
CXX             = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-g++
READELF         = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-readelf
CPP_FILT        = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/bin/$(MV_GCC_TOOLS)-c++filt
else
# Sparc tools
CC              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)-gcc
LD              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)-ld
OBJCOPY         = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)-objcopy
AS              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)-as
AR              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)-ar
STRIP           = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)-strip
OBJDUMP         = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)-objdump
CXX             = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)-g++
READELF         = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)-readelf
CPP_FILT        = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)-c++filt
endif
else ifeq ($(MV_CPU_ARCH),armv8)
# ARM tools
CC              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARM_GCC_TOOLS)-gcc
LD              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARM_GCC_TOOLS)-ld
OBJCOPY         = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARM_GCC_TOOLS)-objcopy
AS              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARM_GCC_TOOLS)-as
AR              = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARM_GCC_TOOLS)-ar
STRIP           = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARM_GCC_TOOLS)-strip
OBJDUMP         = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARM_GCC_TOOLS)-objdump
CXX             = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARM_GCC_TOOLS)-g++
READELF         = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARM_GCC_TOOLS)-readelf
CPP_FILT        = $(CCACHE) $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARM_GCC_TOOLS)-c++filt
endif

MOVI_COMPILE_LDSCR = $(MV_TOOLS_DIR)/$(MV_TOOLS_VERSION)/common/moviCompile/ldscripts/shave_first_phase.ld

ifeq ($(TOOLS_VERSION_TYPE), $(NEWER_MOVI_COMPILE_FOLDER_STRUCT))
#define MVISAOPT
    ifeq ($(MV_SOC_PLATFORM), myriad2)
      ifeq ($(MV_SOC_REV), ma2150)
        MVISAOPT   ?= ma2x5x
      else ifeq ($(MV_SOC_REV), ma2155)
        MVISAOPT   ?= ma2x5x
      else ifeq ($(MV_SOC_REV), ma2450)
        MVISAOPT   ?= ma2x5x
      else ifeq ($(MV_SOC_REV), ma2455)
        MVISAOPT   ?= ma2x5x
      else ifeq ($(MV_SOC_REV), ma2480)
        MVISAOPT   ?= ma2x5x
      else ifeq ($(MV_SOC_REV), ma2490)
        MVISAOPT   ?= ma2x5x
      else
        $(error "$(MV_SOC_REV) no MVISAOPT defined for this sec. revision")
      endif
    else
      $(error "Only myriad2 platforms are supported by this MDK")
    endif
endif

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
# Path to RTEMS Libraries and header files
ifeq ($(MV_SOC_OS), rtems)
  ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155 ma2450 ma2455))
    MV_RTEMS_LOS_LIB ?= $(MV_TOOLS_BASE)/common/rtems/prebuilt/ma2x5x/lib
  else
      ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2180 ma2185 ma2480 ma2485))
        MV_RTEMS_LOS_LIB ?= $(MV_TOOLS_BASE)/common/rtems/prebuilt/ma2x8x/lib
      else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
        MV_RTEMS_LOS_LIB ?= $(MV_TOOLS_BASE)/common/rtems/prebuilt/ma2x9x/lib
      else
      $(error "$(MV_SOC_REV) has no prebuilt rtems library defined")
      endif
  endif

  # Include RTEMS libraries LEON OS
  DefaultSparcRTEMSLibs += $(MV_RTEMS_LOS_LIB)/librtemsbsp.a
  DefaultSparcRTEMSLibs += $(MV_RTEMS_LOS_LIB)/librtemscpu.a
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
  ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155 ma2450 ma2455))
    MV_RTEMS_LRT_LIB ?= $(MV_TOOLS_BASE)/common/rtems/prebuilt/ma2x5x/lib
  else
      ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2180 ma2185 ma2480 ma2485))
        MV_RTEMS_LRT_LIB ?= $(MV_TOOLS_BASE)/common/rtems/prebuilt/ma2x8x/lib
      else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
        MV_RTEMS_LRT_LIB ?= $(MV_TOOLS_BASE)/common/rtems/prebuilt/ma2x9x/lib
      else
      $(error "$(MV_SOC_REV) has no prebuilt rtems library defined")
      endif
  endif

  # Include RTEMS Libraries LEON RT
  DefaultSparcRTEMSLibsRT  += $(MV_RTEMS_LRT_LIB)/librtemsbsp.a
  DefaultSparcRTEMSLibsRT  += $(MV_RTEMS_LRT_LIB)/librtemscpu.a
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

ifeq ($(TOOLS_VERSION_TYPE), $(OLDER_MOVI_COMPILE_FOLDER_STRUCT))
    CompilerANSILibs = $(MV_TOOLS_BASE)/common/moviCompile/libraries/$(MV_SOC_PLATFORM)/mlibcxx.a
    CompilerANSILibs += $(MV_TOOLS_BASE)/common/moviCompile/libraries/$(MV_SOC_PLATFORM)/mlibVecUtils.a
else
    ifeq ($(TOOLS_VERSION_TYPE), $(NEWER_MOVI_COMPILE_FOLDER_STRUCT))
        CompilerANSILibs = $(MV_TOOLS_BASE)/common/moviCompile/lib/$(MVISAOPT)/mlibcxx.a
        CompilerANSILibs += $(MV_TOOLS_BASE)/common/moviCompile/lib/$(MVISAOPT)/mlibVecUtils.a
    endif
endif

CompilerANSILibs += --start-group
ifeq ($(TOOLS_VERSION_TYPE), $(OLDER_MOVI_COMPILE_FOLDER_STRUCT))
    CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MV_SOC_PLATFORM)/mlibc_lite.a
    CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MV_SOC_PLATFORM)/mlibm.a
    CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MV_SOC_PLATFORM)/mlibcrt.a
else
    ifeq ($(TOOLS_VERSION_TYPE), $(NEWER_MOVI_COMPILE_FOLDER_STRUCT))
        CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibc_lite.a
        CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibm.a
        CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibcrt.a
    endif
endif
CompilerANSILibs += --end-group
