#################################################################################
#                         Toolchain                                             #
#################################################################################
#To bring your project to GCC 4.4.2 from 3.4.4 you need to
#Remove any function prototypes that have unknown length arrays like:
#void myfunc(unsigned int vararray[][]);
#and make them valid like: void myfunc(unsigned int **vararray);
# and add the *(COMMON) section to your .bss sections in the ldscript of your application
#Like so:
#  .bss ALIGN(8) (NOLOAD) : {
#    __bss_start = ALIGN(8);
#    *(.bss*8) *(.bss*4) *(.bss*2) *(.bss*1)
#    . = ALIGN(8); *(.bss)
#    . = ALIGN(8); *(.bss.*)
#    . = ALIGN(8); *(.bss*)
#    . = ALIGN(8); *(COMMON)
#    __bss_end = ALIGN(8);
#  }

# L1 cache settings for LEON OS
DISABLE_LEON_ICACHE 		?=no
DISABLE_LEON_DCACHE 		?=no

# L1 cache settings for LEON RT
DISABLE_LEON_ICACHE_LRT 	?=no
DISABLE_LEON_DCACHE_LRT 	?=no

# L1 cache settings for LEON NN
DISABLE_LEON_ICACHE_LNN 	?=no
DISABLE_LEON_DCACHE_LNN 	?=no

#Select if is a debug or release
MV_BUILD_CONFIG                 ?=release
BUILD_WITH_CODE_SIZE_OPTIMIZATION	?=no
#######################################################################################
#                           BEGIN common components makefiles                         #
#######################################################################################

# profilers
include $(MV_COMMON_BASE)/components/profiler/profiler.mk

#######################################################################################
#                           END common components makefiles                           #
#######################################################################################




#######################################################################################
#                           BEGIN DEBUG - RELEASE CONFIGURATION                       #
#######################################################################################

ifneq "y" "$(CONFIG_BUILD_SYSTEM_NEXTGEN)"
ifeq ($(MV_BUILD_CONFIG) $(BUILD_WITH_CODE_SIZE_OPTIMIZATION), debug no)
	SHAVEDEBUG                  =yes
	DEBUG                       =yes
	DEBUG_LRT                   =yes
else
	SHAVEDEBUG                  ?=no
	DEBUG                       ?=no
	DEBUG_LRT                   ?=no
endif
ifeq ($(MV_BUILD_CONFIG) $(BUILD_WITH_CODE_SIZE_OPTIMIZATION), debug yes)
$(info NOTE - The current project does not support unoptimized code size builds due to memory constraints)
endif
else
	SHAVEDEBUG                  =$(if $(filter y, $(CONFIG_BUILD_DEBUG_SHAVE)),yes,no)
	DEBUG                       =$(if $(filter y, $(CONFIG_BUILD_DEBUG_LOS)),yes,no)
	DEBUG_LRT                   =$(if $(filter y, $(CONFIG_BUILD_DEBUG_LRT)),yes,no)
endif
MVDBG		= $(MVDBG2)

#Leon optimization level used for LOS and LRT
ifeq ($(DEBUG), yes)
	LEONOPTLEVEL?= -O0
	ARMOPTLEVEL?= -O0
	# Debug flag for LOS
	CCOPT += -DDEBUG
else
	LEONOPTLEVEL?= -Os
	ARMOPTLEVEL?= -O0
endif

ifeq ($(DEBUG_LRT), yes)
	LEONOPTLEVEL_LRT?= -O0

	# Debug flag for LRT
	CCOPT_LRT += -DDEBUG
else
	LEONOPTLEVEL_LRT?= -Os
endif

ifeq ($(DEBUG_LNN), yes)
	LEONOPTLEVEL_LNN?= -O0

	# Debug flag for LRT
	CCOPT_LNN += -DDEBUG
else
	LEONOPTLEVEL_LNN?= -Os
endif

ifeq ($(SHAVEDEBUG), yes)
	MVCCOPT += -DDEBUG

	#These refer to moviCompile optimization levels
	MVCC_OPT_LEVEL ?= -O0
else
	#These refer to moviCompile optimization levels
	MVCC_OPT_LEVEL ?= -O3
	#as moviCompile gets more and more optimizations, setting LSU preference
	#becomes a must for higher optimization levels to avoid undesired port arbitration
	MVCC_STORE_LOAD_POLICY ?= -mlsu-load-policy=prefer-lsu0 -mlsu-store-policy=prefer-lsu0
	MVCCOPT += $(MVCC_STORE_LOAD_POLICY)
endif

ifeq ($(SHAVE_NN_DEBUG), yes)
	MVCC_NN_OPT += -DDEBUG

	#These refer to moviCompile optimization levels
	MVCC_NN_OPT_LEVEL ?= -O0
else
	#These refer to moviCompile optimization levels
	MVCC_NN_OPT_LEVEL ?= -O3
	#as moviCompile gets more and more optimizations, setting LSU preference
	#becomes a must for higher optimization levels to avoid undesired port arbitration
	MVCC_NN_STORE_LOAD_POLICY ?= -mlsu-load-policy=prefer-lsu0 -mlsu-store-policy=prefer-lsu0
	MVCC_NN_OPT += $(MVCC_STORE_LOAD_POLICY)
endif

#moviCompile debug options
MV_DBG_INFO_SHAVE   ?= -g

MVCCDEBUG           += $(MV_DBG_INFO_SHAVE)

MV_DBG_INFO_LOS ?= -gdwarf-4 -gstrict-dwarf -g3
MV_DBG_INFO_LRT ?= -gdwarf-4 -gstrict-dwarf -g3

##########################################################################################
#                            END DEBUG - RELEASE CONFIGURATION                           #
##########################################################################################


# Select C level standard. Default is not specified
LEON_C_STD_CHOICE           ?= -std=gnu11
LEON_CPP_STD_CHOICE         ?= -std=gnu++11

ifeq ($(MV_SOC_PLATFORM),myriad2)
   LEON_ENDIAN         	       	?=LEND
# Options for     LEON OS
   CCOPT               		+= -DMYRIAD2
   MVCCOPT             		+= -DMYRIAD2

# Options for LEON RT
   CCOPT_LRT               	+= -DMYRIAD2 -DLEON_RT

# Options for LEON NN
   CCOPT_LNN               	+= -DMYRIAD2 -DLEON_NN

# TODO nextgen clean-up - remove this block once nextgen becomes default as
# it's superseded by CONFIG_STARTUP_CODE_XXX kconfig setting
ifneq "y" "$(CONFIG_BUILD_SYSTEM_NEXTGEN)"
   ifeq ($(LEON_STARTUP_MASK),LRT)
    MV_DEFAULT_START_PROC_ID    ?=LRT
   else
    MV_DEFAULT_START_PROC_ID    ?=LOS
   endif
endif
else
   $(error "Invalid platform, please set MV_SOC_PLATFORM myriad2")
endif

ifeq "y" "$(CONFIG_BUILD_SYSTEM_NEXTGEN)"
ifdef CONFIG_TARGET_SOC
MV_SOC_REV ?=$(CONFIG_TARGET_SOC)
endif
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
else ifeq ($(MV_SOC_REV), ma2480)
   CCOPT                       += -DMA2480
   CCOPT_LRT                   += -DMA2480
   MVCCOPT                     += -DMA2480
   MVCCOPT_LRT                 += -DMA2480
else ifeq ($(MV_SOC_REV), ma2485)
   CCOPT                       += -DMA2485
   CCOPT_LRT                   += -DMA2485
   MVCCOPT                     += -DMA2485
   MVCCOPT_LRT                 += -DMA2485
else ifeq ($(MV_SOC_REV), ma2490)
   CCOPT                       += -DMA2490
   CCOPT_LRT                   += -DMA2490
   CCOPT_LNN                   += -DMA2490
   MVCCOPT                     += -DMA2490
   MVCCOPT_LRT                 += -DMA2490
   MVCCOPT_LNN                 += -DMA2490
else ifeq ($(MV_SOC_REV), ma2490_nn)
   CCOPT                       += -DMA2490 -no-fpu
   MVCCOPT                     += -DMA2490 -cv:ma2x9x:nn
else ifeq ("y","$(CONFIG_BUILD_SYSTEM_NEXTGEN)")
$(warning Target SOC is not yet configured)
else
   $(error Unrecognised MV_SOC_REV: $(MV_SOC_REV))
endif

# Use a common define to identify all MyriadX variants
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2180 ma2185))
   CCOPT+=-DMA2X8X
   CCOPT_LRT+=-DMA2X8X
   MVCCOPT+=-DMA2X8X
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
   CCOPT+=-DMA2X9X
   CCOPT_LRT+=-DMA2X9X
   CCOPT_LNN+=-DMA2X9X
   MVCCOPT+=-DMA2X9X
endif

# This flag causes the build system to direct moviCompile to
# directly generate object files rather than .asmgen files
# It is currently off by default pending the successful resolution of
# bug #23028
MVCC_SKIP_ASM?=no

# The following setting disables the moviAsm code analyser
# for the moviAsm stage of compiled code. It is not generally
# recommended to disable this analyser but this flag is defaulted on
# to retain backwards compatibility with existing build system default
# settings
MVCC_DISABLE_CODE_ANALYSER?=yes

# Use this for building paths
GCCVERSION          ?= 6.3.0
ARMV8_GCCVERSION    ?= 6.3.0

# L1 Cache options LEON OS
ifeq ($(DISABLE_LEON_ICACHE),yes)
	CCOPT               += -DDISABLE_LEON_ICACHE
endif

ifeq ($(DISABLE_LEON_DCACHE),yes)
	CCOPT               += -DDISABLE_LEON_DCACHE
endif

# L1 Cache options LEON RT
ifeq ($(DISABLE_LEON_ICACHE_LRT),yes)
	CCOPT_LRT               += -DDISABLE_LEON_ICACHE
endif

ifeq ($(DISABLE_LEON_DCACHE_LRT),yes)
	CCOPT_LRT               += -DDISABLE_LEON_DCACHE
endif

# L1 Cache options LEON NN
ifeq ($(DISABLE_LEON_ICACHE_LNN),yes)
	CCOPT_LNN               += -DDISABLE_LEON_ICACHE
endif

ifeq ($(DISABLE_LEON_DCACHE_LNN),yes)
	CCOPT_LNN               += -DDISABLE_LEON_DCACHE
endif

# setup multi trap vectors for rtems apps for both leons
# Trap model LEON OS + RTEMS
ifeq ($(MV_SOC_OS),rtems)
   ifeq ($(MV_SOC_PLATFORM), myriad2)
	MV_LEON_TRAP_TYPE = mvt
   endif
endif

ifeq ($(MV_LEON_TRAP_TYPE),mvt)
  CCOPT += -DLEON_MVT
endif

# Trap modek LEON RT + RTEMS
ifeq ($(MV_SOC_OS_LRT),rtems)
   ifeq ($(MV_SOC_PLATFORM), myriad2)
	MV_LEON_TRAP_TYPE_LRT = mvt
   endif
endif

#Leon NN only bare-metal for now
MV_LEON_TRAP_TYPE_LNN = svt

ifeq ($(MV_LEON_TRAP_TYPE_LRT),mvt)
  CCOPT_LRT += -DLEON_MVT
endif

ifeq ($(MV_LEON_TRAP_TYPE_LNN),mvt)
  CCOPT_LNN += -DLEON_MVT
endif

# Endianess selection
ifeq ($(LEON_ENDIAN),LEND)
  LD_ENDIAN_OPT	        ?=-EL
  LD_ENDIAN_OPT_LRT	    ?=-EL
  LD_ENDIAN_OPT_LNN	    ?=-EL
 ifeq ($(MV_CPU_ARCH), sparc)
	  LD_OFORMAT     = elf32-littlesparc
 else ifeq ($(MV_CPU_ARCH), armv8)
   ifeq ($(MV_CPU_ARCH_64BIT), yes)
	  LD_OFORMAT     = elf64-littleaarch64
   else
	  LD_OFORMAT     = elf32-littlearm
   endif
 endif
 ifeq ($(MV_CPU_ARCH_LRT), sparc)
	  LD_OFORMAT_LRT = elf32-littlesparc
	  LD_OFORMAT_LNN = elf32-littlesparc
 else ifeq ($(MV_CPU_ARCH_LRT), armv7)
	  LD_OFORMAT_LRT = elf32-littlearm
	  LD_OFORMAT_LNN = elf32-littlearm
 endif

else
	LD_ENDIAN_OPT	        ?=-EB
    LD_ENDIAN_OPT_LRT	    ?=-EB
    LD_ENDIAN_OPT_LNN	    ?=-EB

  ifeq ($(MV_CPU_ARCH), sparc)
	  LD_OFORMAT     = elf32-sparc
  else ifeq ($(MV_CPU_ARCH), armv8)
    ifeq ($(MV_CPU_ARCH_64BIT), yes)
      LD_OFORMAT     = elf64-bigaarch64
    else
      LD_OFORMAT     = elf32-bigarm
    endif
  endif
  ifeq ($(MV_CPU_ARCH_LRT), sparc)
    LD_OFORMAT_LRT = elf32-sparc
    LD_OFORMAT_LNN = elf32-sparc
  else ifeq ($(MV_CPU_ARCH_LRT), armv7)
    LD_OFORMAT_LRT = elf32-bigearm
    LD_OFORMAT_LNN = elf32-bigearm
  endif
endif

ifeq ($(MAKEDEPEND),yes)
  CCOPT     += -MD -MP
  CCOPT_LRT += -MD -MP
  CCOPT_LNN += -MD -MP
  MVCCOPT   += -MD -MP
  MVCC_NN_OPT   += -MD -MP
endif

####################################################################################
####################################################################################
#                    Platform Specific Configuration                               #
####################################################################################
# Moved here for being used to build paths
MV_GCC_TOOLS ?= sparc-myriad-rtems
ifeq ($(MV_CPU_ARCH_64BIT),yes)
MV_ARM_GCC_TOOLS ?= aarch64-myriad-elf
else
MV_ARM_GCC_TOOLS ?= arm-myriad-rtems
endif
ifndef UNAME
UNAME:=$(shell uname -o)
endif

ifeq ($(UNAME),GNU/Linux)
  FULL_PLATFORM := $(shell uname -a)
  ifeq ($(findstring x86_64,$(FULL_PLATFORM)),x86_64)
    DETECTED_PLATFORM =linux64
  else
    $(error Linux32 platform is no longer supported.)
  endif
  ifeq ($(MV_SOC_OS), rtems)
    CCOPT += -D__RTEMS__
  endif
  ifeq ($(MV_SOC_OS_LRT), rtems)
    CCOPT_LRT += -D__RTEMS__
  endif
#  #No RTEMS support for LeonNN yet
#  ifeq ($(MV_SOC_OS_LNN), rtems)
#    CCOPT_LNN += -D__RTEMS__
#  endif

  SPARC_DIR?=$(MV_GCC_TOOLS)-$(GCCVERSION)/bin
  ARMV8_DIR?=$(MV_ARM_GCC_TOOLS)-$(GCCVERSION)/bin
else
  DETECTED_PLATFORM =win32
  ifeq ($(MV_SOC_OS), rtems)
    CCOPT += -D__RTEMS__
  endif
  ifeq ($(MV_SOC_OS_LRT), rtems)
    CCOPT_LRT += -D__RTEMS__
  endif
#  #No RTEMS support for LeonNN yet
#  ifeq ($(MV_SOC_OS_LNN), rtems)
#    CCOPT_LNN += -D__RTEMS__
#  endif

  SPARC_DIR?=$(MV_GCC_TOOLS)-$(GCCVERSION)
  #allow user to set path as cygwin or DOS
  ifneq ($(MV_TOOLS_DIR),)
     #MV_TOOLS_DIR:=$(shell cygpath -m "$(MV_TOOLS_DIR)")
     MV_TOOLS_DIR:=$(shell cygpath "$(MV_TOOLS_DIR)")
  endif
endif

ifeq ($(MV_CPU_ARCH),armv8)
MV_TOOLS_VERSION ?= Latest_193274
#MV_TOOLS_VERSION ?= Latest_from_svn
else
MV_TOOLS_VERSION ?= 18.06.6
endif

MV_TOOLS_DIR     ?= $(HOME)/WORK/Tools/Releases/General

# Variable for internal use only by this makefile
MV_TOOLS_BASE    = $(MV_TOOLS_DIR)/$(MV_TOOLS_VERSION)

MV_TOOLS_PATHS         = $(MV_TOOLS_BASE)/common/toolspaths.mk
ifeq ("$(wildcard $(MV_TOOLS_PATHS))","")
    MV_TOOLS_PATHS     = $(MV_COMMON_BASE)/mdk_toolspaths.mk
else

ifeq ($(MV_CPU_ARCH),armv8)
    MV_TOOLS_PATHS     = $(MV_COMMON_BASE)/mdk_toolspaths.mk
endif
#The tools don't seem to know anything about VPU2 building so moving to internal
#definitions for VPU2 enabled ma2490 SoC
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
    MV_TOOLS_PATHS     = $(MV_COMMON_BASE)/mdk_toolspaths.mk
endif
endif

ifneq (1,$(words [$(MV_TOOLS_PATHS)]))
        $(error Wrong MV_TOOLS_PATHS, GNU Make can not handle files with spaces in their names. Do not use spaces ! Also avoid using special characters like '. , ! ? >')
endif

#any extra options which anyone would like to pass to moviDebugServer
#one could use this for example when connecting multiple boards to the
#same PC using different -ftdiJtag:[JTAG_ID] commands when running make start_server
MV_SRV_EXTRA_OPT   ?=

include $(MV_COMMON_BASE)/resolvetargetpath.mk

include $(MV_TOOLS_PATHS)

CCACHE := $(shell if [ -z "$(MV_USE_CCACHE)" ]; then echo ""; else which ccache 2>/dev/null || echo ""; fi)

# this macro is intended to be used inside the compilation recipes and it'll
# output and informative message about ccache only the very first time it's
# invoked
define PRINT_USING_CCACHE
	$(ECHO) [[ -z "$(PRINT_CCACHE_USAGE)" ]] && [[ ! -z "$(CCACHE)" ]] && echo ccache was found on your system and it will be used to enhance the build time || true
	$(eval PRINT_CCACHE_USAGE := no)
endef

####################################################################################
#                    End of Platform Specific Configuration                        #
####################################################################################

####################################################################################
#                    Mandatory common flags for some of the tools                  #
####################################################################################

# Current Warning level for LOS and LRT
WARN            ?= -Wall -Wextra
WARN_LRT        ?= -Wall -Wextra
WARN_SVE        ?= -Wall -Wextra


# Leon Tools common flags
ODOPT            += -xdsrw
SDOPT            += -t
# Linker options
# Linker options for both leons
LDOPT_TRACE      ?=
ifeq ($(MV_CPU_ARCH), sparc)
LDOPT            += $(LD_ENDIAN_OPT) -O9 $(LDOPT_TRACE) --gc-sections -warn-common $(patsubst %,-L %,$(DirSparcDefaultLibs))
else ifeq ($(MV_CPU_ARCH), armv8)
#Workaround until GC issue with SHAVE code is resolved for ARM
ifneq ($(SHAVE_APP_LIBS),)
	LDOPT            += $(LD_ENDIAN_OPT) --oformat $(LD_OFORMAT) -O9 $(LDOPT_TRACE) --no-gc-sections -M -warn-common $(patsubst %,-L %,$(DirARMv8DefaultLibs))
else
	LDOPT            += $(LD_ENDIAN_OPT) --oformat $(LD_OFORMAT) -O9 $(LDOPT_TRACE) --gc-sections -M -warn-common $(patsubst %,-L %,$(DirARMv8DefaultLibs))
endif
endif

LDOPT_SEARCH_PATH += -L $(DirAppRoot) -L $(DirAppRoot)/scripts -L $(DirLDScript)
ifeq ($(MV_SOC_REV),ma2490)
  LDOPT_SEARCH_PATH += -L $(DirLDScrCommon)/arch/ma2490/
endif


#include first the arch folder, followed by the common source dir
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2450 ma2455))
LDOPT_SEARCH_PATH += -L $(DirLDScrCommon)/arch/ma2450
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2150 ma2155))
LDOPT_SEARCH_PATH += -L $(DirLDScrCommon)/arch/ma2150
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2480 ma2485))
LDOPT_SEARCH_PATH += -L $(DirLDScrCommon)/arch/ma2480
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2490))
LDOPT_SEARCH_PATH += -L $(MDK_INSTALL_DIR_ABS)/../system/linking/ldScripts/myriad2memorySections
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2490_nn))
LDOPT_SEARCH_PATH += -L $(MDK_INSTALL_DIR_ABS)/../system/linking/ldScripts/myriad2memorySections
else
   $(error Unrecognised MV_SOC_REV: $(MV_SOC_REV))
endif
LDOPT_SEARCH_PATH += -L $(DirLDScrCommon)

#Apply LDOPT_SEARCH_PATH to LDOPT
LDOPT += $(LDOPT_SEARCH_PATH)

# For now we specify ma2480 as the mcpu version for MA2x9x*
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2490 ma2490_nn))
# This override is primarily needed for the gcc toolchain as it
# hasn't yet been updated for MA2x9x
MV_MCPU		  =  ma2480
else
MV_MCPU		  =  $(MV_SOC_REV)
endif

# Compilation options for LEON OS
CCOPT            += -Werror-implicit-function-declaration -ffunction-sections -fno-common -fdata-sections -fno-builtin-isinff $(MV_DBG_INFO_LOS) $(WARN)
#$(MV_SOC_REV)
ifeq ($(MV_CPU_ARCH), sparc)
	CCOPT            += $(LEONOPTLEVEL) -mcpu=$(MV_MCPU)
endif
ifeq ($(MV_CPU_ARCH), armv8)
ifeq ($(NO_NEON),yes)
	CCOPT            += $(ARMOPTLEVEL) -mcpu=cortex-a53+nosimd -Wno-type-limits -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-format -Wno-incompatible-pointer-types
else
	CCOPT            += $(ARMOPTLEVEL) -mcpu=cortex-a53 -Wno-type-limits -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-format -Wno-incompatible-pointer-types
endif
endif

# Compilation options for LEON LRT
CCOPT_LRT        += -Werror-implicit-function-declaration

CCOPT_LRT        += $(LEONOPTLEVEL_LRT) -mcpu=$(MV_MCPU) -ffunction-sections -fno-common -fdata-sections -fno-builtin-isinff $(MV_DBG_INFO_LRT) $(WARN_LRT)

# Compilation options for LEON LNN
CCOPT_LNN        += -Werror-implicit-function-declaration
CCOPT_LNN        += $(LEONOPTLEVEL_LNN) -mcpu=$(MV_MCPU) -ffunction-sections -fno-common -fdata-sections -fno-builtin-isinff $(MV_DBG_INFO_LNN) $(WARN_LNN)


#C only compilation flags
#standard choice
CONLY_OPT        += $(LEON_C_STD_CHOICE)

#CPP only compilation flags
#standard choice
CPPONLY_OPT      += $(LEON_CPP_STD_CHOICE)

ifneq ($(strip $(shaveXApps)),)
firstPhaseLinkScript = $(MV_COMMON_BASE)/scripts/ld/shaveDynamicLoad/dyndata_shave_first_phase.ldscript
else
firstPhaseLinkScript = $(MOVI_COMPILE_LDSCR)
endif

#First phase SHAVE linking script
MVLIBOPT += -T $(firstPhaseLinkScript) -Ur -O9 --demangle $(LD_ENDIAN_OPT)

# Common usage option for VERBOSE=yes builds
ifeq ($(VERBOSE),yes)
 ECHO =
endif

#Options for dynamic loading linking
LDDYNOPT =-L . -L ./scripts --nmagic -s
LDDYNOPT+=-T $(MV_COMMON_BASE)/scripts/ld/$(MV_SOC_PLATFORM)_dynamic_shave_slice.ldscript

LDSYMOPT =-L . -L ./scripts --nmagic
LDSYMOPT+=-T $(MV_COMMON_BASE)/scripts/ld/$(MV_SOC_PLATFORM)_dynamic_shave_slice.ldscript

V_APP_LDDYNOPT =-L . -L ./scripts --nmagic -s
V_APP_LDDYNOPT+=-T $(MV_COMMON_BASE)/scripts/ld/shaveDynamicLoad/shaveDynLoadBin_App.ldscript

V_APP_LDSYNOPT =-L . -L ./scripts --nmagic
V_APP_LDSYNOPT+=-T $(MV_COMMON_BASE)/scripts/ld/shaveDynamicLoad/shaveDynLoadBin_App.ldscript

V_GRP_LDDYNOPT =-L . -L ./scripts --nmagic -s
V_GRP_LDDYNOPT+=-T $(MV_COMMON_BASE)/scripts/ld/shaveDynamicLoad/shaveDynLoadBin_Group.ldscript

V_GRP_LDSYNOPT =-L . -L ./scripts --nmagic
V_GRP_LDSYNOPT+=-T $(MV_COMMON_BASE)/scripts/ld/shaveDynamicLoad/shaveDynLoadBin_Group.ldscript

ASOPT    		 += $(CC_INCLUDE)

ifeq ($(DISABLE_PRINTF), yes)
 CCOPT += -DNO_PRINT
endif

ifeq ($(DISABLE_PRINTF_LRT), yes)
 CCOPT_LRT += -DNO_PRINT
endif

ifeq ($(DISABLE_PRINTF_LNN), yes)
 CCOPT_LNN += -DNO_PRINT
endif

# Extra options LOS
CCOPT += $(CCOPT_EXTRA)

# Extra options LRT
CCOPT_LRT += $(CCOPT_EXTRA_LRT)

# Extra options LNN
CCOPT_LNN += $(CCOPT_EXTRA_LNN)

# Linker script for both leons
LDOPT += $(LDOPT_EXTRA)
LDOPT += -T $(LinkerScript)

EXTRA_MLIBS_PATH ?=

#Extra C++ options to pass to moviCompile
MVCCPPOPT ?=

#These are arguments that will be passed to moviAsm for assembly files handwritten
#preferably like -opt:X. If nothing passed optimization level is considered 0. No need for -opt:0 because it is implicit
MVASM_HAND_OPT_LEVEL ?=
#These are arguments that will be passed to moviAsm for assembly files generated by moviCompile
#things like -optc
MVASM_COMPILED_OPT_LEVEL ?=
#enable code checker
MVASMCODECHECKER ?=
#Movicompile stack options
#MVCCSTACK ?= -globalstack

MVASMOPT  += -noFinalSlotCompression
# Shave tools common flags

#Debug options for moviAsm too
mvAsmAsmfilesOpt  = -g

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2185 ma2180 ma2150 ma2155))
MVCVOPT          ?= $(MV_SOC_REV)
else
#No support from moviAsm for VPU2 SHAVE in the available tools yet
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
MVCVOPT          ?= ma2480
else
MVCVOPT          ?= $(MV_SOC_REV)
endif
endif

MVASMOPT         += -cv:$(MVCVOPT)  $(MVASMCODECHECKER)
MVASMOPT         += -noSPrefixing
# prevent moviAsm: WARNING 20: .NOWARNEND has no effect at line xxx
MVASMOPT         += -nowarn:20

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2480 ma2485))
# This is a very short term override to test new moviCompile
# This special handling will be removed by end aug 2016 assuming all is working
# For now, we need to set cpu to ma2150 for GCC and ma2480 for MoviCompile
MV_MCPU_MOVICOMPILE = ma2480
else
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2490))
#No support in available tools for VPU2 SHAVE support so using older variant
MV_MCPU_MOVICOMPILE = ma2480
else
MV_MCPU_MOVICOMPILE = $(MV_MCPU)
endif
endif

MVCCOPT          += -mcpu=$(MV_MCPU_MOVICOMPILE) $(MVCC_OPT_LEVEL) $(MVCC_OPT_LEVEL) -ffunction-sections $(MVCCSTACK) $(MVCOMPILEINLINESTATUS) $(MVCCDEBUG) $(WARN_SVE)
MVCC_NN_OPT          += -mcpu=$(MV_MCPU_MOVICOMPILE) $(MVCC_NN_OPT_LEVEL) -ffunction-sections $(MVCCSTACK) $(MVCOMPILEINLINESTATUS) $(MVCCDEBUG) $(WARN_SVE)

ifeq ($(MVCC_SKIP_ASM),yes)
MVCCOPT          += -c
MVCC_NN_OPT          += -c
else
MVCCOPT          += -S
MVCC_NN_OPT          += -S
endif

MVCCOPT += -fno-exceptions
MVCC_NN_OPT += -fno-exceptions
#Need to define this later to keep parameters better aligned
MVLNKOPT         ?= -cv:$(MVCVOPT)
MVDBGOPT         ?= -cv:$(MVCVOPT) $(DDRINIT_SWITCH)$(DdrInitElf) $(MVDBG_EXTRA_OPT)

MVSIMOPT         += -cv:$(MVCVOPT) -leon
MVCMDOPT         ?=
MVCMDOPT         += -ddrSize:$(DRAM_SIZE_MB)
MVSIMOUTOPT      ?=
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2490))
ifeq ($(MV_CPU_ARCH),armv8)
MVCONVOPT        ?= -cv:$(MV_SOC_REV) -initOnlyForDdrSections -mvcmdInit:$(DdrInitHeaderMvcmd)
else
MVCONVOPT        ?= -cv:ma2480 -initOnlyForDdrSections -mvcmdInit:$(DdrInitHeaderMvcmd)
endif
else
MVCONVOPT        ?= -cv:$(MV_SOC_REV) -initOnlyForDdrSections -mvcmdInit:$(DdrInitHeaderMvcmd)
endif
CCOPT            += -DDRAM_SIZE_MB=$(DRAM_SIZE_MB)

MbinSymPrefix    ?= SVE0_

#Shave common libraries
CommonMlibFile   = $(DirAppOutput)/swCommon
ProjectShaveLib  = $(DirAppOutput)/$(APPNAME).mvlib

#Group loading MVLIB options. Slightly modified version of standard MVLIBOPT to include -z muldefs
LdCommandLineFileHeader = $(MVLIBOPT) -z muldefs --gc-sections

# Path to folder containing .S files with RTEMS startup
MV_RTEMS_ASM_STARTUP_CODE ?= $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/rtems/asm/*.S)

#if there are shave components then put the Project shave library on the list of dependencies
ifeq ($(SHAVE_COMPONENTS),yes)
PROJECT_SHAVE_LIBS += $(ProjectShaveLib)
endif

#Provide this to the users so they can use it in their applications if they include common shave code
PROJECT_SHAVE_LIBS += $(CommonMlibFile).mvlib
#Provide this to the users so they can use it in their applications if they include mbin shave code
PROJECT_SHAVE_MBINLIBS = $(CommonMlibFile).mlib

#This will need to hold shvXlib files to work
#AllLibs  = $(DirAppOutput)/swCommon.shv0lib $(DirAppOutput)/swCommon.shv1lib
#AllLibs += $(DirAppOutput)/swCommon.shv2lib $(DirAppOutput)/swCommon.shv3lib
#AllLibs += $(DirAppOutput)/swCommon.shv4lib $(DirAppOutput)/swCommon.shv5lib
#AllLibs += $(DirAppOutput)/swCommon.shv6lib $(DirAppOutput)/swCommon.shv7lib

ifndef LinkerScript
ifeq "y" "$(CONFIG_BUILD_SYSTEM_NEXTGEN)"
	ifeq "y" "$(CONFIG_USE_CUSTOM_LDSCRIPT)"
		LinkerScript ?= $(APPDIR)/$(call unquote,$(CONFIG_CUSTOM_SCRIPT_REL_PATH))
	else
		ifeq "y" "$(CONFIG_HAS_SHAVE_DYN_APPS)"
				LinkerScript     ?= $(DirLDScrCommon)/$(MV_SOC_PLATFORM)_SDL_default_memory_map.ldscript
		else
				LinkerScript     ?= $(DirLDScrCommon)/$(MV_SOC_PLATFORM)_default_memory_map_elf.ldscript
		endif
	endif
else # "y" CONFIG_BUILD_SYSTEM_NEXTGEN
ifeq ($(MV_CPU_ARCH),armv8)
        ARMV8LinkerScriptSource ?= $(DirLDScrCommonPP)/myriad2collection/link_csrc_arm_aarch64.ld
else
    ifdef  ENABLE_LINKER_PREPROCESS
        ifeq ($(filter $(notdir $(wildcard $(DirLDScript)/*.lds.S)), custom.lds.S), custom.lds.S)
            LinkerScriptSource ?= $(DirLDScript)/custom.lds.S
        else
            LinkerScriptSource ?= $(DirLDScrCommonPP)/$(MV_SOC_PLATFORM)/$(MV_SOC_PLATFORM)_default.lds.S
        endif
        LinkerScript ?= $(DirAppOutput)/$(MV_SOC_PLATFORM)_default.lds
    else
        ifeq ($(MV_SOC_REV),ma2490)
            ifeq ($(filter $(notdir $(wildcard $(DirLDScript)/*.ldscript)), custom.ldscript), custom.ldscript)
                LinkerScript     ?= $(DirLDScript)/custom.ldscript
    	    else
                LinkerScript     ?= $(DirLDScrCommon)/arch/ma2490/$(MV_SOC_PLATFORM)_default_memory_map_elf.ldscript
    	    endif
				else
            ifeq ($(filter $(notdir $(wildcard $(DirLDScript)/*.ldscript)), custom.ldscript), custom.ldscript)
                LinkerScript     ?= $(DirLDScript)/custom.ldscript
            else ifeq ($(filter $(notdir $(wildcard $(DirLDScript)/*.ldscript)), $(APPNAME).ldscript), $(APPNAME).ldscript)
                LinkerScript     ?= $(DirLDScript)/$(APPNAME).ldscript
                $(error You are using a deprecated way to give a customized .ldscript. Please, either \
                  rename your ldscript file to "custom.ldscript", or set the "LinkerScript" variable in \
                  your makefile! )
            else
                ifneq ($(strip $(shaveXApps)),)
                    LinkerScript     ?= $(DirLDScrCommon)/$(MV_SOC_PLATFORM)_SDL_default_memory_map.ldscript
                else
                    LinkerScript     ?= $(DirLDScrCommon)/$(MV_SOC_PLATFORM)_default_memory_map_elf.ldscript
                endif
            endif
        endif
    endif
endif
endif # y CONFIG_BUILD_SYSTEM_NEXTGEN
endif # defined LinkerScript

####################################################################################
#                    End of Mandatory common flags for some of the tools           #
####################################################################################

####################################################################################
#                    MDK Utilitites                                                #
####################################################################################
# Converter into VCD files utility
MVDBGCONV       = $(MV_COMMON_BASE)/utils/dbgConverter/MoviDebugConverter

# MCCI USB Resource Compiler
ifeq ($(DETECTED_PLATFORM),linux64)
   USBRC_PLATFORM = linux64
else
   USBRC_PLATFORM = $(DETECTED_PLATFORM)
endif
USBRC           = $(MV_COMMON_BASE)/utils/usbrc/bin/$(USBRC_PLATFORM)/usbrc

#TODO move this logic to /packages/movidius/usb/subdir.mk once the legacy build system will be remove
USBRCOPT += -dpapi20 -Rs
ifeq ($(CONFIG_USB_FORCE_HS), $(filter $(CONFIG_USB_FORCE_HS), yes y))
   USBRCOPT += -chipinfo $(MV_COMMON_BASE)/utils/usbrc/chipinfo_hs.urc
else
   USBRCOPT += -chipinfo $(MV_COMMON_BASE)/utils/usbrc/chipinfo.urc
endif


####################################################################################
#                    End of MDK Utilitites                                         #
####################################################################################
