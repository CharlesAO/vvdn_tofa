##########################################################################
#                          GENERAL SETTINGS                              #
##########################################################################

#Some safe default setting: errors everywhere, and a nice app name

#verbose or not
export ECHO ?= @

# Disable workaround for now as it also suppresses
# error messages (awaiting fix for bug 19031)
#DUMP_NULL = > /dev/null
# Specify Default App name as being the same as the directory it is in
# Primary makefile can override
ifndef APPNAME
APPNAME := $(basename $(notdir $(CURDIR)))
ifneq (1,$(words [$(APPNAME)]))
        $(error Wrong appName, GNU Make can not handle files with spaces in their names. Do not use spaces ! Also avoid using special characters like '. , ! ? >')
endif
endif

# Variable that will disable moviconvert execution for the application elf file
MV_DISABLE_APP_BOOT_CAPABILITY ?= no

#disable shave app debug context information
SHAVE_APP_ENABLE_DEBUG_CONTEXT ?= NO
ifneq ("$(SHAVE_APP_ENABLE_DEBUG_CONTEXT)","NO")
  CCOPT += -DSHAVE_APP_ENABLE_DEBUG_CONTEXT
  CCOPT_LRT += -DSHAVE_APP_ENABLE_DEBUG_CONTEXT
endif

# System name
ifndef UNAME
UNAME := $(shell uname -o)
endif

#Just make sure we have bin/bash as shell
ifneq ($(wildcard /bin/bash),)
    SHELL:=/bin/bash
    export SHELLOPTS:=errexit:pipefail
endif

ifneq "y" "$(CONFIG_BUILD_SYSTEM_NEXTGEN)"
MV_SOC_REV ?= ma2450
endif
MA2150_COMPAT ?= no

SOC_REV_DIR_SUFFIX :=
ifeq ($(MA2150_COMPAT),yes)
SOC_REV_DIR_SUFFIX := _ma2150_compat
else
    ifneq ($(MA2150_COMPAT),no)
        $(error Invalid MA2150_COMPAT: must be yes/no)
    endif
endif

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155))
    SOC_REV_DIR := ma2150 ma215x ma2x5x $(SOC_REV_DIR)
else
    ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2450 ma2455))
        SOC_REV_DIR := ma2450 ma245x$(SOC_REV_DIR_SUFFIX) ma2x5x $(SOC_REV_DIR)
    else
        ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2480 ma2485))
            SOC_REV_DIR := ma248x ma2x8x $(SOC_REV_DIR)
        else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2490))
            SOC_REV_DIR := ma249x ma2x9x $(SOC_REV_DIR)
        else
        $(error "The $(MV_SOC_REV) product is no longer supported by MDK")
        endif
    endif
endif

MV_COMMON_BASE      ?= ../../common
TMPDIR              ?= $(DirAppOutput)/tmp
MV_SOC_PLATFORM     ?= myriad2

# CPU Architecture definition for each core
ifeq ($(MV_SOC_REV),ma2490)
MV_CPU_ARCH         ?= sparc
else
MV_CPU_ARCH         ?= sparc
endif
MV_CPU_ARCH_64BIT   ?= no
MV_CPU_ARCH_LRT     ?= sparc
LEON_WITH_ARM_APP   ?= no

ifeq ($(MV_CPU_ARCH),sparc)
#include this rule only if specifically intended by user
ifeq ($(LEON_WITH_ARM_APP),yes)
#define the default ARM application, if any
ARM_ELF += ./armApp/output/armApp.elf
#improvements should be made here to force this target to allways rebuild
./armApp/output/armApp.elf : 
	make -C armApp/ clean
	make -C armApp/ all
endif
endif

ifneq ($(MV_CPU_ARCH),$(filter $(MV_CPU_ARCH),sparc armv8 ))
  $(error "MV_CPU_ARCH=$(MV_CPU_ARCH) is not supported for OS core")
endif

ifneq ($(MV_CPU_ARCH_LRT),$(filter $(MV_CPU_ARCH_LRT),sparc ))
  $(error "MV_CPU_ARCH_LRT=$(MV_CPU_ARCH_LRT) is not supported for RT core")
endif

# OS definition for each leon
MV_SOC_OS           ?= none
MV_SOC_OS_LRT       ?= none
#RTEMS not supported yet for Leon NN
MV_SOC_OS_LNN       ?= none

# Libbsd include definition for each Leon
MV_SOC_OS_LIBBSD        ?= 0
MV_SOC_OS_LRT_LIBBSD    ?= 0
#no need for BSD for Leon NN

ifeq ($(MV_SOC_OS), rtems)
    BOARD_OS_TYPE = rtems
else
    BOARD_OS_TYPE = bm
endif

ifeq ($(MV_SOC_OS_LRT), rtems)
    BOARD_OS_TYPE_LRT = rtems
else
    BOARD_OS_TYPE_LRT = bm
endif

# This is used to keep compatibility with the old flow.
# We haven't use any ifdef because the user of the old flow may use only one of this variable on his application
LEON_COMPONENT_PATHS_LOS         += $(LEON_COMPONENT_PATHS)
LEON_COMPONENT_PATHS_LOS         += $(LEON_COMPONENT_PATHS_CPP)
LEON_COMPONENT_PATHS_BM_LOS      += $(LEON_COMPONENT_PATHS_BM)
LEON_COMPONENT_PATHS_BM_LOS      += $(LEON_COMPONENT_PATHS_CPP_BM)
LEON_COMPONENT_PATHS_RTEMS_LOS   += $(LEON_COMPONENT_PATHS_RTEMS)
LEON_COMPONENT_PATHS_RTEMS_LOS   += $(LEON_COMPONENT_PATHS_CPP_RTEMS)
LEON_COMPONENT_HEADERS_PATHS_LOS += $(LEON_COMPONENT_HEADERS_PATHS)
LEON_COMPONENT_HEADERS_LOS       += $(LEON_COMPONENT_HEADERS)
LEON_COMPONENT_HEADERS_BM_LOS    += $(LEON_COMPONENT_HEADERS_BM)
LEON_COMPONENT_HEADERS_RTEMS_LOS += $(LEON_COMPONENT_HEADERS_RTEMS)

LEON_COMPONENT_PATHS_LRT         += $(LEON_COMPONENT_PATHS)
LEON_COMPONENT_PATHS_LRT         += $(LEON_COMPONENT_PATHS_CPP)
LEON_COMPONENT_PATHS_BM_LRT      += $(LEON_COMPONENT_PATHS_BM)
LEON_COMPONENT_PATHS_BM_LRT      += $(LEON_COMPONENT_PATHS_CPP_BM)
LEON_COMPONENT_HEADERS_PATHS_LRT += $(LEON_COMPONENT_HEADERS_PATHS)
LEON_COMPONENT_HEADERS_LRT       += $(LEON_COMPONENT_HEADERS)
LEON_COMPONENT_HEADERS_BM_LRT    += $(LEON_COMPONENT_HEADERS_BM)

ARM_COMPONENT_PATHS         += $(ARM_COMPONENT_PATHS_CPP)
# For compoatibility with old the build flow that required a ComponentList instead of one per scope.
ifdef ComponentList
ifneq ($(MV_SOC_REV), ma2490)
ComponentList_LRT += $(ComponentList)
ComponentList_LOS += $(ComponentList)
ComponentList_SVE += $(ComponentList)
else
ComponentList_LRT += $(ComponentList)
ComponentList_LOS += $(ComponentList)
ComponentList_LNN += $(ComponentList)
ComponentList_ARM += $(ComponentList)
ComponentList_SVE += $(ComponentList)
endif
endif

SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION ?= NO

ifeq ($(SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION), YES)
ComponentList_LOS += MemoryManager MyriadMemInit
ComponentList_LRT += MemoryManager MyriadMemInit
ComponentList_LNN += MemoryManager MyriadMemInit
endif

MV_COMPONENTS_DIR   = $(MV_COMMON_BASE)/components
MV_PACKAGES_BASE    ?=$(MV_COMMON_BASE)/../packages/movidius
MV_DRIVERS_BASE     ?=$(MV_COMMON_BASE)/drivers

ifneq ($(MV_SOC_REV), ma2490)
MV_SWCOMMON_BASE    ?=$(MV_COMMON_BASE)/swCommon
MV_SWCOMMON_IC      ?=$(MV_COMMON_BASE)/swCommon/$(MV_SOC_PLATFORM)
MV_LEON_LIBC_BASE   ?=$(MV_COMMON_BASE)/libc/$(MV_SOC_PLATFORM)/leon
else
ifeq ($(MV_CPU_ARCH),armv8)
MV_SWCOMMON_BASE    ?=$(MV_COMMON_BASE)/swCommon/arch/ma2x9x/ARMv8-A
else 
MV_SWCOMMON_BASE    ?=$(MV_COMMON_BASE)/swCommon/arch/ma2x9x
endif
#And furthermore since we don't need separate SHAVE common just because of different processors
#For SHAVE in this "3 or more" types of processors we introduce an additional SW_COMMON definiton for SHAVE to be used
#where SHAVE code is needed
MV_SWCOMMON_SHAVE_BASE ?= $(MV_COMMON_BASE)/swCommon
#MV_SWCOMMON_IC      ?=$(MV_COMMON_BASE)/swCommon/$(MV_SOC_PLATFORM)/arch/ma2x9x
endif

ifeq ($(MV_CPU_ARCH),sparc)
MV_LEON_LIBC_BASE   ?=$(MV_COMMON_BASE)/libc/$(MV_SOC_PLATFORM)/leon/arch/ma2x9x
else ifeq ($(MV_CPU_ARCH),armv8)
MV_ARM_LIBC_BASE   ?=$(MV_COMMON_BASE)/libc/$(MV_SOC_PLATFORM)/ARMv8-A
endif
MV_SHARED_BASE      ?=$(MV_COMMON_BASE)/shared
MV_EXTRA_DATA       ?=$(MV_COMMON_BASE)/../resources
MV_EXTRA_DATA 	    ?=$(realpath $(MDK_INSTALL_DIR)/../resources)
export MV_EXTRA_DATA

MDK_BASE            ?=$(realpath $(MV_COMMON_BASE)/../)

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2450 ma2455 ma2480 ma2485))
DRAM_SIZE_MB        ?=512
else
DRAM_SIZE_MB        ?=128
endif
SHAVE_COMPONENTS    ?=no
MAKEDEPEND          ?=yes

# The following parameter specifies the default assumption
# for the refclk0 input oscillator on your board.
# The default is based on the chosen oscillator for MV0182
# This parameter is used by moviDebug in order to correctly
# setup the DRAM PLL
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485))
MV_REFCLK0_KHZ      ?=25000
else
MV_REFCLK0_KHZ      ?=12000
endif

# Trap model for each leon
MV_LEON_TRAP_TYPE       ?=svt
MV_LEON_TRAP_TYPE_LRT   ?=svt

LEON_RT_BUILD       ?=no
LEON_STARTUP_MASK   ?=LOS

#Add LeonNN in there
LEON_NN_BUILD       ?=no

RTEMS_BSP           ?=myriad2

ifeq ($(UNAME),GNU/Linux)
MV_LOCAL_OBJ        ?= yes
else
MV_LOCAL_OBJ        ?= no
endif


# default Directories
DirAppRoot           ?= .
DirLDScript          ?= $(DirAppRoot)/config
ifeq ($(MV_SOC_PLATFORM),myriad2)
ifeq "y" "$(CONFIG_BUILD_SYSTEM_NEXTGEN)"
  dir-ld-scr-common-$(CONFIG_LD_SCRIPT_MEMORY_SECTIONS) = memorySections
  dir-ld-scr-common-$(CONFIG_LD_SCRIPT_COLLECTION) = collection
  DirLDScrCommon       ?= $(MV_COMMON_BASE)/scripts/ld/$(MV_SOC_PLATFORM)$(dir-ld-scr-common-y)
else # "y" CONFIG_BUILD_SYSTEM_NEXTGEN
  DirLDScrCommon       ?= $(MV_COMMON_BASE)/scripts/ld/$(MV_SOC_PLATFORM)memorySections
endif # "y" CONFIG_BUILD_SYSTEM_NEXTGEN
else
DirLDScrCommon       ?= $(MV_COMMON_BASE)/scripts/ld
endif
DirLDScrCommonPP     ?= $(MV_COMMON_BASE)/scripts/ld
DirDbgScrCommon      ?= $(MV_COMMON_BASE)/scripts/debug
DirAppOutput         ?= $(DirAppRoot)/output
DirAppObjDir         ?= $(DirAppOutput)/obj
DirAppMbinDir        ?= $(DirAppOutput)/mbin
DirAppSymDir         ?= $(DirAppOutput)/sym
DirOutputLst	     ?= $(DirAppOutput)/lst
DirOutputReport      ?= $(DirAppOutput)/report
DirAppScripts        ?= $(DirAppRoot)/scripts
flashSCR             ?= $(DirAppOutput)/default_flash_script_$(MV_SOC_PLATFORM).scr
eraseSCR             ?= $(DirAppOutput)/default_erase_script_$(MV_SOC_PLATFORM).scr
DefaultLinkerGenerateRule ?= yes
DefaultFlashScript   ?= $(DirDbgScrCommon)/default_flash_$(MV_SOC_PLATFORM)_mdbg2.scr
DefaultEraseScript   ?= $(DirDbgScrCommon)/default_erase_$(MV_SOC_PLATFORM)_mdbg2.scr
DefaultEmmcFlashScript   ?= $(DirDbgScrCommon)/default_emmc_flash_$(MV_SOC_PLATFORM)_mdbg2.scr
DefaultEmmcEraseScript   ?= $(DirDbgScrCommon)/default_erase_emmc_$(MV_SOC_PLATFORM)_mdbg2.scr
DDRDefaultEraseScript ?= $(DirDbgScrCommon)/default_erase_$(MV_SOC_PLATFORM)_ddr_mdbg2.scr
FLASH_SCRIPT_OPT  ?= --script $(flashSCR)
FLASH_SCRIPT_ERASE_OPT  ?= --script $(eraseSCR)

DirAppRelativeMdk    ?= $(subst $(MDK_BASE),,$(realpath .))

ifneq "y" "$(CONFIG_BUILD_SYSTEM_NEXTGEN)"
include $(MV_COMMON_BASE)/ddrinit.mk
endif

# Trace engine options
TRACE_BUFFER_NAME         ?= memDbgLogBuffer
#default memory size of 2 Kb. This is to be used in projects using the memory logger.
TRACE_BUFFER_SIZE         ?= 2048
OUTPUT_TRACE_BUFFER_NAME  ?= $(DirAppOutput)/buf2test
TRACE_SYSTEM_EVENTS_HDR   = $(MV_COMMON_BASE)/swCommon/include/dbgLogEvents.h
#by default we do not set a local events header
LOCAL_TRACE_EVENTS_HDR    ?=

# Converter Tool options
OUTPUT_VCD_FILE     ?= $(DirAppOutput)/outputFile.vcd
OUTPUT_CSV_FILE     ?= $(DirAppOutput)/outputFile.csv

# By default objects are built alongside the c files
# However when MV_LOCAL_OBJ=yes, then they are placed into
# a tree which mirrors the mdk under $(DirObjBase)
# (Typically ouput/obj_base)
ifeq ($(MV_LOCAL_OBJ),yes)
   DirObjBase   ?= $(DirAppOutput)/obj_base
else
   DirObjBase   ?=
endif

# Only set the $(DirAppObjBase) if $(DirObjBase) has a value
# $(DirObjBase) is the app location within the obj_base tree
# e.g. ouput/obj_base/examples/portable/HowTo/SharingExample
ifeq ($(DirObjBase), )
   DirAppObjBase    ?=
else
   DirAppObjBase    ?= $(DirObjBase)$(DirAppRelativeMdk)/
endif

# Default files
OutputFile		 ?= $(DirAppOutput)/$(APPNAME).elf
MvCmdfile    	 ?= $(DirAppOutput)/$(APPNAME).mvcmd

#Folder that holds default Sparc-elf-gcc libs
DirSparcDefaultLibs  ?= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION) \
                        $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/lib
#Folder that holds default libs for armv8 compiler
DirARMv8DefaultLibs  ?= $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(MV_ARM_GCC_TOOLS)-$(GCCVERSION)/lib/gcc/$(MV_ARM_GCC_TOOLS)/$(GCCVERSION)\
                        $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(MV_ARM_GCC_TOOLS)-$(GCCVERSION)/$(MV_ARM_GCC_TOOLS)/lib \
	                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(MV_ARM_GCC_TOOLS)-$(GCCVERSION)/$(MV_ARM_GCC_TOOLS)/libc/lib \
	                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(MV_ARM_GCC_TOOLS)-$(GCCVERSION)/$(MV_ARM_GCC_TOOLS)/libc/usr/lib

#Any default libs specified. Need to be in the form of "-l[libraryname]"
ifeq ($(MV_SOC_PLATFORM),myriad2)
#Extended libs only supported for Myriad2
DefaultARMv8GccLibs  ?= --start-group -lgcc -lc -lstdc++ -lsupc++ -lm --end-group
DefaultSparcGccLibs  ?= --start-group -lgcc -lc -lstdc++ -lsupc++ -lm --end-group
else
DefaultSparcGccLibs  ?= --start-group -lgcc --end-group
endif

# Provide default Entry point for statically linked shave applications
DefaultStaticProjectShaveEntryPoints ?= -e Entry

ifneq ($(SHV_DEFAULT_STACK_SIZE),)
   CCOPT+=-DSHV_DEFAULT_STACK_SIZE=$(SHV_DEFAULT_STACK_SIZE)
   CCOPT_LRT+=-DSHV_DEFAULT_STACK_SIZE=$(SHV_DEFAULT_STACK_SIZE)
   CCOPT_LNN+=-DSHV_DEFAULT_STACK_SIZE=$(SHV_DEFAULT_STACK_SIZE)
   MVCCOPT +=-DSHV_DEFAULT_STACK_SIZE=$(SHV_DEFAULT_STACK_SIZE)
   MVCC_NN_OPT +=-DSHV_DEFAULT_STACK_SIZE=$(SHV_DEFAULT_STACK_SIZE)
endif

ifneq ($(DEFAULT_HEAP_SIZE),)
   CCOPT+=-DDEFAULT_HEAP_SIZE=$(DEFAULT_HEAP_SIZE)
   CCOPT_LRT+=-DDEFAULT_HEAP_SIZE=$(DEFAULT_HEAP_SIZE)
   CCOPT_LNN+=-DDEFAULT_HEAP_SIZE=$(DEFAULT_HEAP_SIZE)
endif

# This adds the ability for the user to add additional components
# to an application while building "make clean all ComponentListExtra=Blah"
ifneq ($(ComponentListExtra),)
   ComponentList_LOS  += $(ComponentListExtra)
   ComponentList_ARM  += $(ComponentListExtra)
   ComponentList_LRT  += $(ComponentListExtra)
   ComponentList_LNN  += $(ComponentListExtra)
   ComponentList_SVE += $(ComponentListExtra)
endif

#SHAVE applications to consider in the default SHAVE group
MV_DEFAULT_SHAVE_GROUP_APPS +=

#Names of desired SHAVE library groups. By default this will include the $(DEFAULT_SHAVE_GROUP)
#we use a $(eval ...) later to build up the list of group variables vrom this one
MV_SHAVE_GROUPS += DEFAULT_SHAVE_GROUP

#First autodetect the applications which are placed in default positions
SHAVE_DEFAULT_DYN_SEARCH_PATH ?= shaveDynApps
AUTO_DETECTED_SHAVE_DEFAULT_DYN_APPS := $(filter-out $(SHAVE_DEFAULT_DYN_SEARCH_PATH)/subdir.mk,$(wildcard $(SHAVE_DEFAULT_DYN_SEARCH_PATH)/*))

ifeq ($(SHAVE_COMPONENTS),yes)
SHAVE_DEFAULT_COMPONENT_DYN_SEARCH_PATH ?= compShvDynApps
AUTO_DETECTED_SHAVE_DEFAULT_COMPONENT_DYN_APPS += $(foreach component,$(ComponentList_SVE), $(wildcard $(MV_COMPONENTS_DIR)/$(component)/$(SHAVE_DEFAULT_COMPONENT_DYN_SEARCH_PATH)/*))
endif


#Build the default applications list
define cDefaultDynApps_template
$(1)App = $$(SHAVE_DEFAULT_DYN_SEARCH_PATH)/$(1)
MV_DEFAULT_SHAVE_GROUP_APPS += $$(DirAppOutput)/$$($(1)App)
endef

basenamenotdir=$(basename $(notdir $(1)))

$(foreach app,$(AUTO_DETECTED_SHAVE_DEFAULT_DYN_APPS),$(eval $(call cDefaultDynApps_template,$(call basenamenotdir,$(app)))))

ifeq ($(SHAVE_COMPONENTS),yes)
#Build the default component applications list
define cDefaultCompDynApps_template
$(1)App = $$(SHAVE_DEFAULT_COMPONENT_DYN_SEARCH_PATH)/$(1)
MV_DEFAULT_SHAVE_GROUP_APPS += $$(DirAppOutput)/$$($(1)App)
endef

$(foreach app,$(AUTO_DETECTED_SHAVE_DEFAULT_COMPONENT_DYN_APPS),$(eval $(call cDefaultCompDynApps_template,$(call basenamenotdir,$(app)))))
endif

#Build internal variable list of all SHAVE applications
shaveXApps := $(foreach group,$(MV_SHAVE_GROUPS),$(MV_$(group)_APPS))

# USB device stack consists of a set of prebuilt components:
# - base components: datapump osrtems xdci
# - protocol specific components: protohid protomsc protondfu protovideo protovsc2 apidownload apistorage
# Fill LeonOS MV_USB_COMPONENTS if MV_USB_PROTOS is specified
MV_USB_PROTOS ?=
RTEMS_USB_HOST_BUILD ?= no
RTEMS_USB_HOST_BUILD_LRT ?= no
RTEMS_USB_LIB_BUILD_TYPE ?= release
RTEMS_USB_LIB_BUILD_TYPE_LRT ?= release
CONFIG_USB_FORCE_HS ?= no
RTEMS_USB_LIB_PATH ?= $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/lib/$(RTEMS_USB_LIB_BUILD_TYPE)
RTEMS_USB_LIB_PATH_LRT ?= $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/lib/$(RTEMS_USB_LIB_BUILD_TYPE_LRT)
RTEMS_USB_INCLUDE_PATH ?= $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include
RTEMS_USB_LIB_BASE ?= $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/lib
RTEMS_USB_LIB_BASE_LRT ?= $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/lib

ifeq ($(RTEMS_USB_LIB_BUILD_TYPE), debug)
   # enable some additional debug messages
   CCOPT += -D__TMS_USBPUMP_DEBUG=1
   CCOPT_LRT += -D__TMS_USBPUMP_DEBUG=1
   CCOPT_LNN += -D__TMS_USBPUMP_DEBUG=1
endif

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155 ma2450 ma2455))
   MV_USB_ARCH=ma2x5x
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485))
   MV_USB_ARCH=ma2x8x
endif

ifneq ($(MV_USB_PROTOS),)
   # cannot have host and device active at the same time
   ifeq ($(RTEMS_USB_HOST_BUILD), yes)
      $(error USB host and device can not be active at the same time)
   endif

   # protondfu requires apidownload as well
   ifeq ($(findstring protondfu,$(MV_USB_PROTOS)),protondfu)
      MV_USB_PROTOS += apidownload
   endif
   # protomsc requires apistorage as well
   ifeq ($(findstring protomsc,$(MV_USB_PROTOS)),protomsc)
      MV_USB_PROTOS += apistorage
   endif
   # protowmc may require usbseri
   ifeq ($(findstring protowmc,$(MV_USB_PROTOS)),protowmc)
      MV_USB_PROTOS += usbseri
   endif
   MV_USB_COMPONENTS ?= datapump osrtems xdci $(MV_USB_PROTOS)
else
   ifeq ($(RTEMS_USB_HOST_BUILD), yes)
      CCOPT += -DRTEMS_USB_HOST_BUILD
      MV_USB_HOST_CLASS_DRIVERS ?= class_msd class_generic classcomposite class_uvc class_nhid class_acm class_wmc class_uac1
      MV_USB_COMPONENTS ?= osrtems xhcd datapump usbd hcdkit \
                           transaction_translator superspeed \
                           classkit hcd $(MV_USB_HOST_CLASS_DRIVERS)
   endif
endif
RTEMS_USB_INCLUDE_DIRS ?= datapump osrtems xdci $(MV_USB_PROTOS)

# Check if USB is used with RTEMS
ifneq ($(MV_USB_COMPONENTS),)
   ifneq ($(MV_SOC_OS), rtems)
      $(error USB driver is only supported under RTEMS OS)
   endif
endif

# Fill LeonRT MV_USB_COMPONENTS_LRT if MV_USB_PROTOS_LRT is specified
MV_USB_PROTOS_LRT ?=
ifneq ($(MV_USB_PROTOS_LRT),)
   # protondfu requires apidownload as well
   ifeq ($(findstring protondfu,$(MV_USB_PROTOS_LRT)),protondfu)
      MV_USB_PROTOS_LRT += apidownload
   endif
   # protomsc requires apistorage as well
   ifeq ($(findstring protomsc,$(MV_USB_PROTOS_LRT)),protomsc)
      MV_USB_PROTOS_LRT += apistorage
   endif
   RTEMS_USB_INCLUDE_DIRS_LRT ?= datapump osrtems xdci $(MV_USB_PROTOS)
   MV_USB_COMPONENTS_LRT ?= datapump osrtems xdci $(MV_USB_PROTOS)
else
   ifeq ($(RTEMS_USB_HOST_BUILD_LRT), yes)
      CCOPT_LRT += -DRTEMS_USB_HOST_BUILD
      CCOPT_LNN += -DRTEMS_USB_HOST_BUILD
      MV_USB_HOST_CLASS_DRIVERS_LRT ?= class_msd class_generic classcomposite class_uvc
      MV_USB_COMPONENTS_LRT ?= osrtems xhcd datapump  usbd hcdkit transaction_translator  \
                               superspeed classkit hcd $(MV_USB_HOST_CLASS_DRIVERS_LRT)
   endif
endif

# Check if USB is used with RTEMS
ifneq ($(MV_USB_COMPONENTS_LRT),)
   ifneq ($(MV_SOC_OS_LRT), rtems)
      $(error USB driver is only supported under RTEMS OS)
   endif
endif

#Add object list of raw data object files.
#The main use of this is to include mbin files and testing data using sparc-elf-objcopy,
#but this may also be used if tables need to be loaded or LUTs and such, again, using sparc-elf-objcopy
RAWDATAOBJECTFILES ?=
RAWDATAOBJECTFILES_LRT ?=

#defining global variables needed for clean and debugging of gnerated output
PROJECTINTERM ?=
PROJECTCLEAN ?=
DDRINIT_SWITCH ?= -ddrInit:

#Old thinking was that the group wuld get statically linked, but we move this to get it dynamically linked too
#making it a second shvdlib
ifneq ($(strip $(shaveXApps)),)
SHVDYNSCRIPT := ./$(DirAppOutput)/shvDynInfrastructureBase.ldscript
PROJECTCLEAN += ./$(DirAppOutput)/shvDynInfrastructureBase.ldscript
endif

#Each shave application module requires a number of build time options
#Have some predefined default values if no one sets anything specific
#shaveXApps := $(foreach group,$(MV_SHAVE_GROUPS),$(MV_$(group)_APPS))

define cDynContextDefs_template
$(1)_HEAPSIZE       ?= 1024*6
$(1)_STACKSIZE      ?= 1024*6
$(1)_MainEntry      ?= Entry
$(1)_HEAP_INIT      ?= DYNCONTEXT_HEAP_INIT

$(1)_OtherEntries   ?=
cDynContextDefs_$(1) ?= -D'APPDYNBSSDATASECTION=__$(1)_appbss_sectionStart' -D'APPDYNBSSDATASECTIONEND=__$(1)_appbss_sectionEnd'
cDynContextDefs_$(1) += -D'APPDYNDATASECTION=$(1)appdyndata' -D'SHAVEAPPNAME="$(1)"'
cDynContextDefs_$(1) += -D'APPENTRY=$(1)X_$$($(1)_MainEntry)' -D'APPHEAPSIZE=($$($(1)_HEAPSIZE))'
cDynContextDefs_$(1) += -D'APPSTACKSIZE=($$($(1)_STACKSIZE))' -D'HEAP_INIT_VAL=$$($(1)_HEAP_INIT)'

endef

$(foreach app,$(shaveXApps),$(eval $(call cDynContextDefs_template,$(call basenamenotdir,$(app)))))

ifeq ($(SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION), YES)
CCOPT+=-DSHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
CCOPT_LRT+=-DSHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
CCOPT_LNN+=-DSHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
endif

CCOPT+=-I $(MV_TOOLS_BASE)/common/shared/include
CCOPT_LRT+=-I $(MV_TOOLS_BASE)/common/shared/include

#Instead, let's start building groups. Default group first (most people will probably just use this):
define cGroupVars_template
$(1) ?=  $$(DirAppOutput)/$(1)
#all groups should have these as these are implicit part of all our documentation
$(1)_LIBS ?= --whole-archive $$(CommonMlibFile).mvlib --no-whole-archive $$(CompilerANSILibs)

ifneq "y" "$(CONFIG_BUILD_SYSTEM_NEXTGEN)"
-include appsGrpDynContextRules$(1).mk
endif

PROJECTCLEAN += $$($(1)).mvlibG
endef

$(foreach group,$(MV_SHAVE_GROUPS),$(eval $(call cGroupVars_template,$(group))))

#Keep this for reference because it is easier to debug. This refers to how the rules*.mk fiels get created:
#define cGroupAppVars_template
#
#cDynContextDefs_$(1) += -D'APPGROUPDYNDATASECTION=DEFAULT_SHAVE_GROUPgrpdyndata'
#dummy_$(1)_app:
#	@echo $(1)
#	@echo $$(cDynContextDefs_$(1))
#
#endef
#
#$(foreach app,$(MV_DEFAULT_SHAVE_GROUP_APPS),$(eval $(call cGroupAppVars_template,$(call basenamenotdir,$(app)))))

#Default group is slightly special because we include all default libs
#default rule already brought the CommonMlibFile in so we must take it out when including
#the libraries
DEFAULT_SHAVE_GROUP_LIBS += $(filter-out $(CommonMlibFile).mvlib,$(PROJECT_SHAVE_LIBS))

ifeq ($(SHAVE_COMPONENTS),yes)
ifneq ('$(findstring PipePrint, $(ComponentList_SVE))', '')
# Add component compiled object
 CompilerANSILibs += $(filter %PipePrint_shave.o,$(SHAVE_COMPONENT_OBJS))
 ifneq ($(CUSTOM_SHAVEPIPEPRINT_SECTION),)
   ifneq ($(strip $(shaveXApps)),)
     MVCCOPT += -D'SHAVEPIPEPRINT_SECTION="$(CUSTOM_SHAVEPIPEPRINT_SECTION)"'
     MVCC_NN_OPT +=  -D'SHAVEPIPEPRINT_SECTION="$(CUSTOM_SHAVEPIPEPRINT_SECTION)"'
   endif
 endif
endif
endif

# Force a debug_run script to be created from default template unless one already exists
# By default we default to running an automatically generated debug script
# Applications which need to can override debugSCR in the top level makefile
ifeq ($(wildcard $(DirAppScripts)/debug_run_mdbg2.scr),)
	SourceDebugScript   ?= $(DirDbgScrCommon)/default_mdbg2.scr
else
	SourceDebugScript   ?= $(DirAppScripts)/debug_run_mdbg2.scr
endif
debugSCR             ?= $(DirAppOutput)/generated_debug_script.scr

# Force a debug_run tcl script to be created from default template unless one already exists
# By default we default to running an automatically generated debug script
# Applications which need to can override debugSCR in the top level makefile
ifeq ($(wildcard $(DirAppScripts)/debug_run.tcl),)
   SourceDebugScriptTcl ?= $(DirDbgScrCommon)/default_debug_run.tcl
else
   SourceDebugScriptTcl ?= $(DirAppScripts)/debug_run.tcl
endif
debugSCRTcl             ?= $(DirAppOutput)/generated_debug_script.tcl

#Variable used to control checking of MV_TOOLS_DIR being set for external users
#This is so that internal Movidius users don't need to go through checking if
#MV_TOOLS_DIR is defined on their systems. This is related to feature request in
#Bug 18741 on the internal Movidius bugzilla
#!!!! Please DON'T MAKE ANY CHANGE to the following line, NOT even adding small spaces or anything
#unless you also change the companion line in mdk/regression/mdk_release/Makefile -> 'pack' target
CHECK_TOOLS_VAR=no

##########################################################################
#                       END OF GENERAL SETTINGS                          #
##########################################################################
