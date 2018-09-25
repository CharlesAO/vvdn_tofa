###################################################################
#       Building up list of Leon low level include folders        #
###################################################################

###################################################################
#                        LEON CSS / LEON OS                       #
###################################################################
ifeq ($(MV_CPU_ARCH), sparc)
  CC_INCLUDE          += $(foreach var, $(SOC_REV_DIR), \
                         $(addprefix -I,            $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/src \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/src \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/arch/$(var)/include ))

  CC_INCLUDE          += $(addprefix -I,$(wildcard  $(DirSparcDefaultLibs)/include \
                                                    $(MV_LEON_LIBC_BASE)/include \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/include \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/include/ssp \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/include \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/include/c++ \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/include/c++/$(GCCVERSION) ))

  ifneq ($(MV_SOC_REV), $(filter $(MV_SOC_REV),ma2480 ma2485))
    # On MA2x8x Architecture we don't currently include the common base
    CC_INCLUDE        += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/src \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/src \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/ma2x5x/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include/brdGpioCfgs ))
  else
    CC_INCLUDE        += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/ma2x8x/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/include ))
  endif

  # If we are using RTEMS we should not use MDK Libc definitions LOS
  ifneq ($(MV_SOC_OS), rtems)
    CC_INCLUDE        += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/cross/include))
  else
    CC_INCLUDE        += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/osDrivers/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/include ))

    ifeq ($(RTEMS_USB_HOST_BUILD), yes)
      USB_INCLUDE     += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host
      USB_INCLUDE     += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host/transaction_translator
      USB_INCLUDE     += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host/superspeed
      USB_INCLUDE     += $(patsubst %,$(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host/%,$(MV_USB_HOST_CLASS_DRIVERS))
    endif

    USB_INCLUDE       += $(patsubst %,$(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/%,$(MV_USB_COMPONENTS))
    USB_INCLUDE       += $(patsubst %,$(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/%,$(RTEMS_USB_INCLUDE_DIRS))
    USB_INCLUDE       += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include
    CC_INCLUDE        += $(addprefix -I,$(wildcard  $(USB_INCLUDE)))
  endif
else ifeq ($(MV_CPU_ARCH), armv8)
  CC_INCLUDE          += $(foreach var, $(SOC_REV_DIR), \
                         $(addprefix -I,            $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/bm/arch/$(var)/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/bm/arch/$(var)/src \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/hgl/arch/$(var)/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/hgl/arch/$(var)/src \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/arch/$(var)/include  \
                                                    $(MV_SWCOMMON_BASE)/asm ))

  CC_INCLUDE          += $(addprefix -I,$(wildcard  $(DirARMv8DefaultLibs)/include \
                                                    $(MV_ARM_LIBC_BASE)/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/rtems/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/rtems/arch/$(MV_SOC_REV)/include \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/lib/gcc/$(MV_ARMV8_GCC_TOOLS)/$(ARMV8_GCCVERSION)/include \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/lib/gcc/$(MV_ARMV8_GCC_TOOLS)/$(ARMV8_GCCVERSION)/include/ssp \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARMV8_GCC_TOOLS)/libc/usr/include \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARMV8_GCC_TOOLS)/include/c++ \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(ARMV8_DIR)/$(MV_ARMV8_GCC_TOOLS)/include/c++/$(ARMV8_GCCVERSION)))

  # If we are using RTEMS we should not use MDK Libc definitions LOS
  ifneq ($(MV_SOC_OS), rtems)
    CC_INCLUDE        += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/cross/include))
  else
    CC_INCLUDE        += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/osDrivers/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/include ))
    ifeq ($(RTEMS_USB_HOST_BUILD), yes)
      USB_INCLUDE     += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host
      USB_INCLUDE     += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host/transaction_translator
      USB_INCLUDE     += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host/superspeed
      USB_INCLUDE     += $(patsubst %,$(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host/%,$(MV_USB_HOST_CLASS_DRIVERS))
    endif
    USB_INCLUDE       += $(patsubst %,$(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/%,$(MV_USB_COMPONENTS))
    USB_INCLUDE       += $(patsubst %,$(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/%,$(RTEMS_USB_INCLUDE_DIRS))
    USB_INCLUDE       += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include
    CC_INCLUDE        += $(addprefix -I,$(wildcard  $(USB_INCLUDE)))
  endif
else
  $(error Unsupported MV_CPU_ARCH for OS core: $(MV_CPU_ARCH))
endif

CC_INCLUDE            += $(addprefix -I,$(wildcard  $(MV_SWCOMMON_BASE)/include \
                                                    $(MV_SWCOMMON_BASE)/shave_code/$(MV_SOC_PLATFORM)/include \
                                                    $(MV_SWCOMMON_IC)/include \
                                                    $(MV_SHARED_BASE)/include \
                                                    $(DirAppRoot)/leon \
                                                    $(DirAppRoot)/shared ))

###################################################################
#                        LEON MSS / LEON RT                       #
###################################################################
ifneq ($(MV_SOC_REV), $(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
  # On MA2x8x Architecture we don't currently include the common base
  CC_INCLUDE_LRT      += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/src \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/src \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/ma2x5x/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include/brdGpioCfgs ))
else
ifeq ($(MV_SOC_REV), $(filter $(MV_SOC_REV),ma2480 ma2485))
  CC_INCLUDE_LRT      += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/ma2x8x/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/include ))
else
ifeq ($(MV_SOC_REV), $(filter $(MV_SOC_REV),ma2490))
  CC_INCLUDE_LRT      += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/ma2x9x/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/include ))
else
  $(info WRONG if we got here)
endif
endif
endif

# If we are using RTEMS we should not use MDK Libc definitions LRT
ifneq ($(MV_SOC_OS_LRT), rtems)
  CC_INCLUDE_LRT      += $(addprefix -I,$(wildcard  $(MV_COMMON_BASE)/libc/$(MV_SOC_PLATFORM)/leon/include))
  CC_INCLUDE_LRT      += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/cross/include))
else
  CC_INCLUDE_LRT      += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/osDrivers/include))
  CC_INCLUDE_LRT      += $(addprefix -I,$(wildcard  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/include))

  ifeq ($(RTEMS_USB_HOST_BUILD_LRT), yes)
    USB_INCLUDE_LRT   += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host
    USB_INCLUDE_LRT   += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host/transaction_translator
    USB_INCLUDE_LRT   += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host/superspeed
    USB_INCLUDE_LRT   += $(patsubst %,$(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/host/%,$(MV_USB_HOST_CLASS_DRIVERS_LRT))
  endif
  USB_INCLUDE_LRT     += $(patsubst %,$(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include/%,$(MV_USB_COMPONENTS_LRT))
  USB_INCLUDE_LRT     += $(MV_PACKAGES_BASE)/usb/$(MV_USB_ARCH)/include
  CC_INCLUDE_LRT      += $(addprefix -I,$(wildcard  $(USB_INCLUDE_LRT)))
endif

CC_INCLUDE_LRT        += $(foreach var, $(SOC_REV_DIR), \
                         $(addprefix -I,            $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/src \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/include \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/src \
                                                    $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/arch/$(var)/include ))

CC_INCLUDE_LRT        += $(addprefix -I,$(wildcard  $(DirSparcDefaultLibs)/include \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/include \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/include/ssp \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/include \
                                                    $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/include/c++/$(GCCVERSION)))

CC_INCLUDE_LRT        += $(addprefix -I,$(wildcard  $(MV_SWCOMMON_BASE)/include \
                                                    $(MV_SWCOMMON_BASE)/shave_code/$(MV_SOC_PLATFORM)/include \
                                                    $(MV_SWCOMMON_IC)/include \
                                                    $(MV_SHARED_BASE)/include \
                                                    $(DirAppRoot)/leon \
                                                    $(DirAppRoot)/shared ))

##########################
# Similarly for Leon NN  #
##########################
ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
# On MA2x8x Architecture we don't currently include the common base
CC_INCLUDE_LNN   += $(addprefix -I,$(wildcard   $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include \
						  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include \
						  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/src \
						  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/src \
						  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/include \
						  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/include \
						  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include/brdGpioCfgs \
						  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include ))
endif

CC_INCLUDE_LNN   += $(addprefix -I,$(wildcard   $(DirSparcDefaultLibs)/include \
                                                  $(MV_SWCOMMON_BASE)/include \
                                                  $(MV_SWCOMMON_IC)/include \
                                                  $(MV_SHARED_BASE)/include \
                                                  $(DirAppRoot)/leon $(DirAppRoot)/shared
#Differentiate between 3-or-more Proc types and one whereby swCommon becomes separate SOC generation altogether
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2185 ma2180 ma2150 ma2155))
CC_INCLUDE_LNN   +=                               $(MV_SWCOMMON_BASE)/shave_code/$(MV_SOC_PLATFORM)/include
else
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
CC_INCLUDE_LNN   +=                               $(MV_SWCOMMON_SHAVE_BASE)/shave_code/$(MV_SOC_PLATFORM)/include
else
CC_INCLUDE_LNN   +=                               $(MV_SWCOMMON_BASE)/shave_code/$(MV_SOC_PLATFORM)/include
endif
endif

CC_INCLUDE_LNN   +=                               $(DirAppRoot)/leon \
                                                  $(DirAppRoot)/shared \
                                                  $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/include \
                                                  $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc/$(MV_GCC_TOOLS)/$(GCCVERSION)/include/ssp \
                                                  $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/include \
                                                  $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/include/c++/$(GCCVERSION)))


CC_INCLUDE_LNN  += $(foreach var, $(SOC_REV_DIR), $(addprefix -I, $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/include \
                                                  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/src \
                                                  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/include \
                                                  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/src \
                                                  $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/arch/$(var)/include ))
######End Leon NN CC_INCLUDE_LNN section #


ifeq ($(MV_CPU_ARCH),sparc)
  #Leon include dirs CSS
  LEON_INCLUDE_PATH   += $(filter-out $(MV_SWCOMMON_IC)/include $(MV_SWCOMMON_BASE)/include, $(patsubst %/,%,$(sort $(dir $(LEON_HEADERS)))))
  #And blindly add all component paths
  LEON_INCLUDE_PATH += $(patsubst %,$(MV_COMPONENTS_DIR)/%,$(ComponentList_LOS))
  LEON_INCLUDE_PATH += $(patsubst %,$(MV_COMPONENTS_DIR)/%/include,$(ComponentList_LOS))
  ifeq ($(MV_SOC_REV),ma2490)
  LEON_INCLUDE_PATH += $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/ma2x9x/shared/common/include,$(ComponentList_LOS))
  endif
  CC_INCLUDE     += $(addprefix -I,$(foreach folder, $(LEON_INCLUDE_PATH), $(if $(findstring ARMv8, $(folder)),,$(folder))))
else ifeq ($(MV_CPU_ARCH),armv8)
  #ARM include dirs CSS
  ARMV8_INCLUDE_PATH   += $(filter-out $(MV_SWCOMMON_IC)/include $(MV_SWCOMMON_BASE)/arch/ma2x9x/ARMv8-A/include $(MV_SWCOMMON_BASE)/arch/ma2x9x/ARMv8-A/asm, $(patsubst %/,%,$(sort $(dir $(ARMV8_HEADERS)))))
  #And blindly add all component paths
  ARMV8_INCLUDE_PATH += $(patsubst %,$(MV_COMPONENTS_DIR)/%,$(ComponentList_ARM))
  ARMV8_INCLUDE_PATH += $(patsubst %,$(MV_COMPONENTS_DIR)/%/include,$(ComponentList_ARM))
  ARMV8_INCLUDE_PATH += $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/ma2x9x/shared/common/include,$(ComponentList_ARM))
  CC_INCLUDE     += $(addprefix -I,$(foreach folder, $(ARMV8_INCLUDE_PATH), $(if $(findstring leon, $(folder)),,$(folder))))
endif
#Leon include dirs MSS
LEON_INCLUDE_PATH_LRT   += $(filter-out $(MV_SWCOMMON_IC)/include $(MV_SWCOMMON_BASE)/include, $(patsubst %/,%,$(sort $(dir $(LEON_HEADERS_LRT)))))
#And blindly add all component paths
LEON_INCLUDE_PATH_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%,$(ComponentList_LRT))
LEON_INCLUDE_PATH_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/include,$(ComponentList_LRT))

#And Leon include dirs LNN
LEON_INCLUDE_PATH_LNN   += $(filter-out $(MV_SWCOMMON_IC)/include $(MV_SWCOMMON_BASE)/include, $(patsubst %/,%,$(sort $(dir $(LEON_HEADERS_LNN)))))
#And blindly add all component paths
LEON_INCLUDE_PATH_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%,$(ComponentList_LNN))
LEON_INCLUDE_PATH_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/include,$(ComponentList_LNN))

CC_INCLUDE_LRT += $(addprefix -I,$(foreach folder, $(LEON_INCLUDE_PATH_LRT), $(if $(findstring ARMv8, $(folder)),,$(folder))))
CC_INCLUDE_LNN += $(addprefix -I,$(foreach folder, $(LEON_INCLUDE_PATH_LNN), $(if $(findstring ARMv8, $(folder)),,$(folder))))

#Add the include folders to the CCOPT variable
CCOPT += $(CC_INCLUDE)
CCOPT_LRT += $(CC_INCLUDE_LRT)
CCOPT_LNN += $(CC_INCLUDE_LNN)

###################################################################
#       Building up list of RTEMS required include folders        #
###################################################################
## LOS
ifeq ($(MV_SOC_OS), rtems)
  #TODO: figure out after resolution of internal Bugzilla 24623 if isystem here can be changed to just -I
  CCOPT += -isystem $(MV_RTEMS_LOS_LIB)/include
  CCOPT += -I $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/include
  CCOPT += -I $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)
  CCOPT += -I $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc
endif
## LRT
ifeq ($(MV_SOC_OS_LRT), rtems)
  #TODO: figure out after resolution of internal Bugzilla 24623 if isystem here can be changed to just -I
  CCOPT_LRT += -isystem $(MV_RTEMS_LRT_LIB)/include
  CCOPT_LRT += -I $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/include
  CCOPT_LRT += -I $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)
  CCOPT_LRT += -I $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc
endif

## LNN
ifeq ($(MV_SOC_OS_LRT), rtems)
  #TODO: figure out after resolution of internal Bugzilla 24623 if isystem here can be changed to just -I
  CCOPT_LNN += -isystem $(MV_RTEMS_LRT_LIB)/include
  CCOPT_LNN += -I $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/$(MV_GCC_TOOLS)/include
  CCOPT_LNN += -I $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)
  CCOPT_LNN += -I $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/$(SPARC_DIR)/lib/gcc
endif


###################################################################
#       Building up list of Shave assembly include folders        #
###################################################################

MVASM_INCLUDE     += $(wildcard $(MV_COMMON_BASE)/swCommon/shave_code/$(MV_SOC_PLATFORM)/include \
																$(MV_COMMON_BASE)/swCommon/shave_code/$(MV_SOC_PLATFORM)/asm)

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485))
MVASM_INCLUDE     += $(wildcard  $(MV_SWCOMMON_BASE)/shave_code/$(MV_SOC_PLATFORM)/include/arch/ma2x8x \
																 $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shave/arch/ma2x8x/include)
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
MVASM_INCLUDE     += \
	$(wildcard  $(MV_SWCOMMON_SHAVE_BASE)/shave_code/$(MV_SOC_PLATFORM)/include/arch/ma2x9x \
	$(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shave/arch/ma2x9x/include)
else
MVASM_INCLUDE     += $(wildcard  $(MV_SWCOMMON_BASE)/shave_code/$(MV_SOC_PLATFORM)/include/arch/ma2x5x \
																 $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shave/arch/ma2x5x/include)
endif

ifeq "$(UNAME)" "Cygwin"
MVASM_INCLUDE_ABS := $(foreach dir,$(MVASM_INCLUDE),$(shell cygpath -aw $(dir)))
else
MVASM_INCLUDE_ABS = $(MVASM_INCLUDE)
endif

#Adding common folder includes
MVASMOPT		 += $(patsubst %,-i:%,$(abspath $(MVASM_INCLUDE_ABS)))

###################################################################
#       Building up list of Shave C/C++ include folders           #
###################################################################

MVCC_INCLUDE      += $(patsubst %,-I %, $(MV_TOOLS_BASE)/common/moviCompile/include \
						   $(MV_PACKAGES_BASE)/pcModel/moviCompile/compilerIntrinsics/include \
						   $(MV_PACKAGES_BASE)/pcModel/moviCompile/compilerVectorFunctions/include \
                           $(MV_COMMON_BASE)/swCommon/include \
                           $(MV_COMMON_BASE)/swCommon/shave_code/$(MV_SOC_PLATFORM)/include \
                           $(MV_SHARED_BASE)/include \
                           $(DirAppRoot)/shave \
                           $(DirAppRoot)/shared)

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485))
MVCC_INCLUDE     += $(patsubst %, -I%,$(wildcard  $(MV_SWCOMMON_BASE)/shave_code/$(MV_SOC_PLATFORM)/include/arch/ma2x8x ))
else
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
MVCC_INCLUDE     += $(patsubst %, -I%,$(wildcard  $(MV_SWCOMMON_SHAVE_BASE)/shave_code/$(MV_SOC_PLATFORM)/include/arch/ma2x9x ))
else
MVCC_INCLUDE     += $(patsubst %, -I%,$(wildcard  $(MV_SWCOMMON_BASE)/shave_code/$(MV_SOC_PLATFORM)/include/arch/ma2x5x ))
endif
endif

ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
MVCC_INCLUDE      += $(patsubst %,-I %, \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/include \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/include \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shave/include)
endif

MVCC_INCLUDE      += $(foreach var, $(SOC_REV_DIR), $(patsubst %,-I %, \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/include \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/include \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shave/arch/$(var)/include \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/include \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/sgl/include \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/hgl/include \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/include))

#And Components include directories
SHAVE_COMPONENT_INCLUDE_PATHS  += $(patsubst %/,%,$(sort $(dir $(SHAVE_COMPONENT_HEADERS))))
#And blindly add all component paths
SHAVE_COMPONENT_INCLUDE_PATHS += $(patsubst %,$(MV_COMPONENTS_DIR)/%,$(ComponentList_SVE))

SHAVE_APP_HEADER_PATHS += $(patsubst %/,%,$(sort $(dir $(SHAVE_HEADERS))))

MVCC_INCLUDE += $(patsubst %,-I %,$(sort $(SHAVE_COMPONENT_INCLUDE_PATHS) $(SHAVE_APP_HEADER_PATHS)))

MVCCOPT += $(MVCC_INCLUDE)
MVCC_NN_OPT += $(MVCC_INCLUDE)

