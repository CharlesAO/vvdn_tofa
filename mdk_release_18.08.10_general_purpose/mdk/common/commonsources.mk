################################################################################
#       Building up list of components paths                                   #
################################################################################

################################################################################
# 07.04.2016 - new components structure                                        #
# 02.02.2017 - Updated to include asm folder and bm folder                     #
# This is the desired structure for components - demonstrative purposes only:  #
#components                                                                    #
#├── ComponentA                                                                #
#│   ├── leon                                                                  #
#│   │   ├── bm                                                                #
#│   │   │   ├── asm                                                           #
#│   │   │   ├── include                                                       #
#│   │   │   └── src                                                           #
#│   │   ├── common                                                            #
#│   │   │   ├── include                                                       #
#│   │   │   └── src                                                           #
#│   │   └── rtems                                                             #
#│   │       ├── include                                                       #
#│   │       └── src                                                           #
#│   └── shave                                                                 #
#├── ComponentB                                                                #
#│   ├── arch                                                                  #
#│   │   └── ma2150                                                            #
#│   │       └── leon                                                          #
#│   │           ├── bm                                                        #
#│   │           │   ├── asm                                                   #
#│   │           │   ├── include                                               #
#│   │           │   └── src                                                   #
#│   │           └── rtems                                                     #
#│   │               ├── include                                               #
#│   │               └── src                                                   #
#│   └── leon                                                                  #
#│       └── rtems                                                             #
#│           ├── include                                                       #
#│           └── src                                                           #
#└── ComponentC                                                                #
#    ├── leon                                                                  #
#    │   └── bm                                                                #
#    │       ├── asm                                                           #
#    │       ├── include                                                       #
#    │       └── src                                                           #
#    ├── Readme.txt                                                            #
#    └── shave                                                                 #
################################################################################

ifeq ($(shell test -d $(MV_COMPONENTS_DIR)/MVBoardsCommon && echo 1), 1)
include $(MV_COMPONENTS_DIR)/MVBoardsCommon/mvboardscommon.mk
else
# nothing should be done if MVBoardsCommon component doesn't exist
mvboardscheck:
	@echo "" > /dev/null
endif

LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE)/src/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE)/include/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE)/common/src/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE)/common/include/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_LOS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/$(BOARD_OS_TYPE)/src/*,$(ComponentList_LOS)))
LEON_COMPONENT_PATHS_LOS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/$(BOARD_OS_TYPE)/include/*,$(ComponentList_LOS)))
LEON_COMPONENT_PATHS_LOS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/src/*,$(ComponentList_LOS)))
LEON_COMPONENT_PATHS_LOS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/include/*,$(ComponentList_LOS)))
ifdef ($(MV_SOC_REV),ma2490)
LEON_COMPONENT_PATHS_LOS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/common/src/*,$(ComponentList_LOS)))
LEON_COMPONENT_PATHS_LOS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/common/include/*,$(ComponentList_LOS)))
endif
LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE_LRT)/src/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE_LRT)/include/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE_LRT)/common/src/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE_LRT)/common/include/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_LRT_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/$(BOARD_OS_TYPE_LRT)/src/*,$(ComponentList_LRT)))
LEON_COMPONENT_PATHS_LRT_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/$(BOARD_OS_TYPE_LRT)/include/*,$(ComponentList_LRT)))
LEON_COMPONENT_PATHS_LRT_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/src/*,$(ComponentList_LRT)))
LEON_COMPONENT_PATHS_LRT_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/include/*,$(ComponentList_LRT)))

LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE_LNN)/src/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE_LNN)/include/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE_LNN)/common/src/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/$(BOARD_OS_TYPE_LNN)/common/include/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_LNN_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/$(BOARD_OS_TYPE_LNN)/src/*,$(ComponentList_LNN)))
LEON_COMPONENT_PATHS_LNN_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/$(BOARD_OS_TYPE_LNN)/include/*,$(ComponentList_LNN)))
LEON_COMPONENT_PATHS_LNN_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/src/*,$(ComponentList_LNN)))
LEON_COMPONENT_PATHS_LNN_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/include/*,$(ComponentList_LNN)))

#END############################################################################

#END############################################################################

################################################################################
# The following path setting is for backwards compatibility with older         #
# components structure                                                         #
################################################################################

#Build the list of leon source required from the components variable
LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*/leon,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/*/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/*/*/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/shared/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_LOS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/shared,$(ComponentList_LOS))

LEON_COMPONENT_PATHS_LOS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/*,$(ComponentList_LOS)))
LEON_COMPONENT_PATHS_LOS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/*/*,$(ComponentList_LOS)))

LEON_COMPONENT_PATHS_LOS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/src,$(ComponentList_LOS)))
LEON_COMPONENT_PATHS_LOS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/src/*,$(ComponentList_LOS)))

LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*/leon,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/*/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/*/*/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/shared/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_LRT_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/shared,$(ComponentList_LRT))

LEON_COMPONENT_PATHS_LRT_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/*,$(ComponentList_LRT)))
LEON_COMPONENT_PATHS_LRT_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/*/*,$(ComponentList_LRT)))

LEON_COMPONENT_PATHS_LRT_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/src,$(ComponentList_LRT)))
LEON_COMPONENT_PATHS_LRT_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/src/*,$(ComponentList_LRT)))

LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*/leon,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/*/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/*/*/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/shared/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_LNN_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/shared,$(ComponentList_LNN))

LEON_COMPONENT_PATHS_LNN_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/*,$(ComponentList_LNN)))
LEON_COMPONENT_PATHS_LNN_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/*/*,$(ComponentList_LNN)))

LEON_COMPONENT_PATHS_LNN_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/src,$(ComponentList_LNN)))
LEON_COMPONENT_PATHS_LNN_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/src/*,$(ComponentList_LNN)))

# ARMv8
ARMV8_COMPONENT_PATHS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/ARMv8-A,$(ComponentList_ARM))
ARMV8_COMPONENT_PATHS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*/ARMv8-A,$(ComponentList_ARM))
ARMV8_COMPONENT_PATHS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/ARMv8-A/*,$(ComponentList_ARM))
ARMV8_COMPONENT_PATHS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/ARMv8-A/*/*,$(ComponentList_ARM))
ARMV8_COMPONENT_PATHS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/ARMv8-A/*/*/*,$(ComponentList_ARM))
ARMV8_COMPONENT_PATHS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/shared/*,$(ComponentList_ARM))
ARMV8_COMPONENT_PATHS_INIT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/shared,$(ComponentList_ARM))
ARMV8_COMPONENT_PATHS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/ARMv8-A/common/*,$(ComponentList_ARM)))
ARMV8_COMPONENT_PATHS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/ARMv8-A/common/*/*,$(ComponentList_ARM)))
ARMV8_COMPONENT_PATHS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/common/src/*,$(ComponentList_ARM)))
ARMV8_COMPONENT_PATHS_INIT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/common/include/*,$(ComponentList_ARM)))

#sort all LEON_COMPONENT_PATHS because duplicate elimination is needed
#LEON_COMPONENT_PATHS_LOS += $(sort $(wildcard $(LEON_COMPONENT_PATHS_LOS_INIT)))
#LEON_COMPONENT_PATHS_LRT += $(sort $(wildcard $(LEON_COMPONENT_PATHS_LRT_INIT)))

#END############################################################################

################################################################################
# Older flow settings - unrelating with currently working flow
################################################################################

LEON_COMPONENT_PATHS_BM_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_BM_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*/leon/bm,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_BM_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_BM_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm/*/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_BM_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm/*/*/*,$(ComponentList_LOS))

LEON_COMPONENT_PATHS_BM_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_BM_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*/leon/bm,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_BM_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_BM_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm/*/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_BM_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm/*/*/*,$(ComponentList_LRT))

LEON_COMPONENT_PATHS_BM_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_BM_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*/leon/bm,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_BM_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_BM_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm/*/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_BM_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/bm/*/*/*,$(ComponentList_LNN))

LEON_COMPONENT_PATHS_RTEMS_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_RTEMS_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*/leon/rtems,$(ComponentList_LOS))
# Gather Components based on MV_SOC_REV arch variants bm/rtems
LEON_COMPONENT_PATHS_RTEMS_LOS += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/bm/*,$(ComponentList_LOS)))
LEON_COMPONENT_PATHS_RTEMS_LOS += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/rtems/*,$(ComponentList_LOS)))
LEON_COMPONENT_PATHS_RTEMS_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_RTEMS_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems/*/*,$(ComponentList_LOS))
LEON_COMPONENT_PATHS_RTEMS_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems/*/*/*,$(ComponentList_LOS))

LEON_COMPONENT_PATHS_RTEMS_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_RTEMS_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*/leon/rtems,$(ComponentList_LRT))
# Gather Components based on MV_SOC_REV arch variants bm/rtems
LEON_COMPONENT_PATHS_RTEMS_LRT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/bm/*,$(ComponentList_LRT)))
LEON_COMPONENT_PATHS_RTEMS_LRT += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/rtems/*,$(ComponentList_LRT)))
LEON_COMPONENT_PATHS_RTEMS_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_RTEMS_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems/*/*,$(ComponentList_LRT))
LEON_COMPONENT_PATHS_RTEMS_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems/*/*/*,$(ComponentList_LRT))

LEON_COMPONENT_PATHS_RTEMS_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_RTEMS_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*/leon/rtems,$(ComponentList_LNN))
# Gather Components based on MV_SOC_REV arch variants bm/rtems
LEON_COMPONENT_PATHS_RTEMS_LNN += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/bm/*,$(ComponentList_LNN)))
LEON_COMPONENT_PATHS_RTEMS_LNN += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/rtems/*,$(ComponentList_LNN)))
LEON_COMPONENT_PATHS_RTEMS_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_RTEMS_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems/*/*,$(ComponentList_LNN))
LEON_COMPONENT_PATHS_RTEMS_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/leon/rtems/*/*/*,$(ComponentList_LNN))

LEON_COMPONENT_PATHS_LOS += $(sort $(wildcard $(LEON_COMPONENT_PATHS_LOS_INIT) \
                                              $(LEON_COMPONENT_PATHS_BM_LOS) \
                                              $(LEON_COMPONENT_PATHS_RTEMS_LOS)))
LEON_COMPONENT_PATHS_LRT += $(sort $(wildcard $(LEON_COMPONENT_PATHS_LRT_INIT) \
                                              $(LEON_COMPONENT_PATHS_BM_LRT) \
                                              $(LEON_COMPONENT_PATHS_RTEMS_LRT)))
LEON_COMPONENT_PATHS_LNN += $(sort $(wildcard $(LEON_COMPONENT_PATHS_LNN_INIT) \
                                              $(LEON_COMPONENT_PATHS_BM_LNN) \
                                              $(LEON_COMPONENT_PATHS_RTEMS_LNN)))
ARM_COMPONENT_PATHS += $(sort $(wildcard $(ARMV8_COMPONENT_PATHS_INIT)))
#END############################################################################


################################################################################
# Including header files; Used both in backwards compatibility and in current
# structure
################################################################################

LEON_COMPONENT_HEADERS_PATHS_LOS += $(LEON_COMPONENT_PATHS_LOS)
LEON_COMPONENT_HEADERS_PATHS_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*,$(ComponentList_LOS))
LEON_COMPONENT_HEADERS_PATHS_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*include*,$(ComponentList_LOS))
LEON_COMPONENT_HEADERS_PATHS_LOS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/kernels/*/shave/include,$(ComponentList_LOS))

LEON_COMPONENT_HEADERS_PATHS_LRT += $(LEON_COMPONENT_PATHS_LRT)
LEON_COMPONENT_HEADERS_PATHS_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*,$(ComponentList_LRT))
LEON_COMPONENT_HEADERS_PATHS_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*include*,$(ComponentList_LRT))
LEON_COMPONENT_HEADERS_PATHS_LRT += $(patsubst %,$(MV_COMPONENTS_DIR)/%/kernels/*/shave/include,$(ComponentList_LRT))

LEON_COMPONENT_HEADERS_PATHS_LNN += $(LEON_COMPONENT_PATHS_LNN)
LEON_COMPONENT_HEADERS_PATHS_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*,$(ComponentList_LNN))
LEON_COMPONENT_HEADERS_PATHS_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*include*,$(ComponentList_LNN))
LEON_COMPONENT_HEADERS_PATHS_LNN += $(patsubst %,$(MV_COMPONENTS_DIR)/%/kernels/*/shave/include,$(ComponentList_LNN))

ARM_COMPONENT_HEADERS_PATHS += $(ARM_COMPONENT_PATHS)
ARM_COMPONENT_HEADERS_PATHS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*,$(ComponentList_ARM))
ARM_COMPONENT_HEADERS_PATHS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*include*,$(ComponentList_ARM))
#END############################################################################

################################################################################
# Used for backwards compatibility. Are not used in current flow
################################################################################

LEON_COMPONENT_HEADERS_PATHS_BM += $(LEON_COMPONENT_PATHS_BM_LOS)
LEON_COMPONENT_HEADERS_PATHS_RTEMS += $(LEON_COMPONENT_PATHS_RTEMS_LOS)

#END############################################################################


SH_COMPONENTS_PATHS_1 += $(patsubst %, $(MV_COMPONENTS_DIR)/%/*shave*, $(ComponentList_SVE))
SH_COMPONENTS_PATHS_1 += $(patsubst %, $(MV_COMPONENTS_DIR)/%/*/*shave*, $(ComponentList_SVE))
SH_COMPONENTS_PATHS_1 += $(patsubst %, $(MV_COMPONENTS_DIR)/%/*shave*/*, $(ComponentList_SVE))
SH_COMPONENTS_PATHS_1 += $(patsubst %, $(MV_COMPONENTS_DIR)/%/*shave*/*/*, $(ComponentList_SVE))
SH_COMPONENTS_PATHS_1 += $(patsubst %, $(MV_COMPONENTS_DIR)/%/kernels/*/shave/src, $(ComponentList_SVE))
SH_COMPONENTS_PATHS_1 += $(patsubst %, $(MV_COMPONENTS_DIR)/%/kernels/*/shave/src/cpp, $(ComponentList_SVE))
SH_COMPONENTS_PATHS_1 += $(foreach var, $(SOC_REV_DIR),$(patsubst %, $(MV_COMPONENTS_DIR)/%/kernels/*/arch/$(var)/shave/src, $(ComponentList_SVE)))
SH_COMPONENTS_PATHS_2 += $(wildcard $(addsuffix /*.*,$(SH_COMPONENTS_PATHS_1)))
SH_COMPONENTS_PATHS_3 += $(patsubst %/,%,$(sort $(dir $(SH_COMPONENTS_PATHS_2))))

SH_COMPONENTS_PATHS = $(foreach v,$(SH_COMPONENTS_PATHS_3),$(if $(findstring myriad,$(v)),,$(v)))

SH_COMPONENTS_PATHS += $(patsubst %, $(MV_COMPONENTS_DIR)/%/*shave*/*/$(MV_SOC_PLATFORM), $(ComponentList_SVE))
SH_COMPONENTS_PATHS += $(patsubst %, $(MV_COMPONENTS_DIR)/%/*shave*/$(MV_SOC_PLATFORM), $(ComponentList_SVE))
SH_COMPONENTS_PATHS += $(patsubst %, $(MV_COMPONENTS_DIR)/%/*shared*/*, $(ComponentList_SVE))
SH_COMPONENTS_PATHS += $(patsubst %, $(MV_COMPONENTS_DIR)/%/*shared*, $(ComponentList_SVE))

SH_COMPONENTS_PATHS += $(patsubst %, $(MV_COMPONENTS_DIR)/%/shave/src,$(ComponentList_SVE))
SH_COMPONENTS_PATHS += $(foreach var, $(SOC_REV_DIR),$(patsubst %, $(MV_COMPONENTS_DIR)/%/arch/$(var)/shave/src,$(ComponentList_SVE)))
SH_COMPONENTS_PATHS += $(foreach var, $(SOC_REV_DIR),$(patsubst %, $(MV_COMPONENTS_DIR)/%/arch/$(var)/shave/src/*,$(ComponentList_SVE)))
SH_COMPONENTS_PATHS += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/src,$(ComponentList_SVE)))
SH_COMPONENTS_PATHS += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/src/*,$(ComponentList_SVE)))

SH_COMPONENTS_HEADERS_PATHS += $(SH_COMPONENTS_PATHS)

SH_COMPONENTS_HEADERS_PATHS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*,$(ComponentList_SVE))
SH_COMPONENTS_HEADERS_PATHS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/*include*,$(ComponentList_SVE))
SH_COMPONENTS_HEADERS_PATHS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/kernels/*/shave/include,$(ComponentList_SVE))

SH_COMPONENTS_HEADERS_PATHS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/shave/include/,$(ComponentList_SVE))
SH_COMPONENTS_HEADERS_PATHS += $(patsubst %,$(MV_COMPONENTS_DIR)/%/shared/include/,$(ComponentList_SVE))
SH_COMPONENTS_HEADERS_PATHS += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/include,$(ComponentList_SVE)))
SH_COMPONENTS_HEADERS_PATHS += $(foreach var, $(SOC_REV_DIR),$(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shave/include,$(ComponentList_SVE)))
###################################################################
#  Configuration of Leon(OS) Source Files
###################################################################

# Application Specific Stuff
LEON_APP_URC_SOURCES += $(wildcard $(DirAppRoot)/*leon/*.urc) \
                        $(wildcard $(DirAppRoot)/*leon/*/*.urc)
LEON_APP_URC_C_REQUIRED   += $(patsubst %.urc,$(DirAppObjBase)%_urc.c,$(LEON_APP_URC_SOURCES))

LEON_APP_C_SOURCES +=$(wildcard $(DirAppRoot)/*leon/*.c)
LEON_APP_C_SOURCES +=$(wildcard $(DirAppRoot)/*leon/*/*.c)
LEON_APP_C_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon/*.c))
LEON_APP_C_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon/*/*.c))
LEON_APP_C_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon/bm/src/*.c))
LEON_APP_C_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon/rtems/src/*.c))


LEON_APP_CPP_SOURCES +=$(wildcard $(DirAppRoot)/*leon/*.cpp)
LEON_APP_CPP_SOURCES +=$(wildcard $(DirAppRoot)/*leon/*/*.cpp)
LEON_APP_CPP_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon/*.cpp))
LEON_APP_CPP_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon/*/*.cpp))
LEON_APP_CPP_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon/bm/src/*.cpp))
LEON_APP_CPP_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon/rtems/src/*.cpp))

LEON_APP_ASM_SOURCES += $(wildcard $(DirAppRoot)/leon/*.S)
LEON_APP_ASM_SOURCES += $(wildcard $(DirAppRoot)/leon/*/*.S)
LEON_APP_ASM_SOURCES += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon/*.S))
LEON_APP_ASM_SOURCES += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon/*/*.S))
LEON_APP_ASM_SOURCES += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon/bm/asm/*.S))


ARMV8_APP_C_SOURCES +=$(wildcard $(DirAppRoot)/*arm/*.c)
ARMV8_APP_C_SOURCES +=$(wildcard $(DirAppRoot)/*arm/*/*.c)
ARMV8_APP_C_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*ARMV8/*.c))
ARMV8_APP_C_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*ARMV8/*/*.c))
ARMV8_APP_C_SOURCES +=$(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*ARMV8/bm/src/*.c))

ARMV8_APP_ASM_SOURCES += $(wildcard $(DirAppRoot)/arm/*.S)
ARMV8_APP_ASM_SOURCES += $(wildcard $(DirAppRoot)/arm/*/*.S)
ARMV8_APP_ASM_SOURCES += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*ARMV8/*.S))
ARMV8_APP_ASM_SOURCES += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*ARMV8/*/*.S))

# Shared Sources

# We should not pull in MDK libc source code while using RTEMS LOS
ifneq ($(MV_SOC_OS), rtems)
  LEON_LIB_C_SOURCES ?=$(wildcard $(MV_LEON_LIBC_BASE)/src/*.c)
  ARMV8_LIB_C_SOURCES ?=$(wildcard $(MV_ARM_LIBC_BASE)/src/*.c)
endif

# We should not pull in MDK libc source code while using RTEMS LRT
ifneq ($(MV_SOC_OS_LRT), rtems)
  LEON_LIB_C_SOURCES_LRT ?=$(wildcard $(MV_LEON_LIBC_BASE)/src/*.c)
endif

# We should not pull in MDK libc source code while using RTEMS LNN
ifneq ($(MV_SOC_OS_LNN), rtems)
  LEON_LIB_C_SOURCES_LNN ?=$(wildcard $(MV_LEON_LIBC_BASE)/src/*.c)
endif

##### remove all rtems components paths from apps running on baremetal and
##### remove all baremetal components paths from apps running on rtems
ifeq ($(BOARD_OS_TYPE), rtems)
    LEON_COMPONENT_C_SOURCES_LOS += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_LOS)))))))
    LEON_COMPONENT_CPP_SOURCES_LOS += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_LOS)))))))
    LEON_COMPONENT_ASM_SOURCES_LOS += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_LOS)))))))
    LEON_COMPONENT_C_SOURCES_RTEMS_LOS += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_RTEMS_LOS)))))))
    LEON_COMPONENT_CPP_SOURCES_RTEMS_LOS += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_RTEMS_LOS)))))))
    LEON_COMPONENT_ASM_SOURCES_RTEMS_LOS += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_RTEMS_LOS)))))))
else ifeq ($(BOARD_OS_TYPE), bm)
    LEON_COMPONENT_C_SOURCES_LOS += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_LOS)))))))
    LEON_COMPONENT_CPP_SOURCES_LOS += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_LOS)))))))
    LEON_COMPONENT_ASM_SOURCES_LOS += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_LOS)))))))
    LEON_COMPONENT_C_SOURCES_BM_LOS += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_BM_LOS)))))))
    LEON_COMPONENT_CPP_SOURCES_BM_LOS += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_BM_LOS)))))))
    LEON_COMPONENT_ASM_SOURCES_BM_LOS += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_BM_LOS)))))))
endif

ifeq ($(BOARD_OS_TYPE_LRT), rtems)
    LEON_COMPONENT_C_SOURCES_LRT += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_LRT)))))))
    LEON_COMPONENT_CPP_SOURCES_LRT += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_LRT)))))))
    LEON_COMPONENT_ASM_SOURCES_LRT += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_LRT)))))))
    LEON_COMPONENT_C_SOURCES_RTEMS_LRT += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_RTEMS_LRT)))))))
    LEON_COMPONENT_CPP_SOURCES_RTEMS_LRT += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_RTEMS_LRT)))))))
    LEON_COMPONENT_ASM_SOURCES_RTEMS_LRT += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_RTEMS_LRT)))))))
else ifeq ($(BOARD_OS_TYPE_LRT), bm)
    LEON_COMPONENT_C_SOURCES_LRT += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_LRT)))))))
    LEON_COMPONENT_CPP_SOURCES_LRT += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_LRT)))))))
    LEON_COMPONENT_ASM_SOURCES_LRT += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_LRT)))))))
    LEON_COMPONENT_C_SOURCES_BM_LRT += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_BM_LRT)))))))
    LEON_COMPONENT_CPP_SOURCES_BM_LRT += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_BM_LRT)))))))
    LEON_COMPONENT_ASM_SOURCES_BM_LRT += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_BM_LRT)))))))
endif

#Same for LeonNN
ifeq ($(BOARD_OS_TYPE_LNN), rtems)
    LEON_COMPONENT_C_SOURCES_LNN += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_LNN)))))))
    LEON_COMPONENT_CPP_SOURCES_LNN += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_LNN)))))))
    LEON_COMPONENT_ASM_SOURCES_LNN += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_LNN)))))))
    LEON_COMPONENT_C_SOURCES_RTEMS_LNN += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_RTEMS_LNN)))))))
    LEON_COMPONENT_CPP_SOURCES_RTEMS_LNN += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_RTEMS_LNN)))))))
    LEON_COMPONENT_ASM_SOURCES_RTEMS_LNN += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_RTEMS_LNN)))))))
else ifeq ($(BOARD_OS_TYPE_LNN), bm)
    LEON_COMPONENT_C_SOURCES_LNN += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_LNN)))))))
    LEON_COMPONENT_CPP_SOURCES_LNN += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_LNN)))))))
    LEON_COMPONENT_ASM_SOURCES_LNN += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_LNN)))))))
    LEON_COMPONENT_C_SOURCES_BM_LNN += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(LEON_COMPONENT_PATHS_BM_LNN)))))))
    LEON_COMPONENT_CPP_SOURCES_BM_LNN += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(LEON_COMPONENT_PATHS_BM_LNN)))))))
    LEON_COMPONENT_ASM_SOURCES_BM_LNN += $(sort $(wildcard $(subst /rtems/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(LEON_COMPONENT_PATHS_BM_LNN)))))))
endif

ifeq ($(MV_SOC_REV),ma2490)
LEON_COMPONENT_C_SOURCES_LOS += $(foreach var, $(ComponentList_LOS), $(wildcard $(MV_COMPONENTS_DIR)/$(var)/arch/ma2x9x/shared/common/src/*.c))
endif

ARMV8_COMPONENT_C_SOURCES += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.c,$(ARM_COMPONENT_PATHS)))))))
ARMV8_COMPONENT_C_SOURCES += $(foreach var, $(ComponentList_ARM), $(wildcard $(MV_COMPONENTS_DIR)/$(var)/arch/ma2x9x/shared/common/src/*.c))
ARMV8_COMPONENT_CPP_SOURCES += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.cpp,$(ARM_COMPONENT_PATHS)))))))
ARMV8_COMPONENT_ASM_SOURCES += $(sort $(wildcard $(subst /bm/,XXXXX,$(sort $(wildcard $(addsuffix /*.S,$(ARM_COMPONENT_PATHS)))))))

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
# For now, MA2x8x support doesn't use the common drivers as most of them are not
# applicable to the new architecture
LEON_DRIVER_C_SOURCES  += $(wildcard $(MV_SWCOMMON_IC)/src/*.c \
                                     $(MV_SWCOMMON_BASE)/src/*.c)
ARMV8_DRIVER_C_SOURCES  += $(wildcard $(MV_SWCOMMON_IC)/src/*.c \
                                     $(MV_SWCOMMON_BASE)/src/*.c)
else
LEON_DRIVER_C_SOURCES  += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/src/*/*.c \
                                     $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/src/*.c \
                                     $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/src/*.c \
                                     $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/src/*/*.c \
                                     $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/src/*.c \
                                     $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/src/*.c \
                                     $(MV_SWCOMMON_IC)/src/*.c \
                                     $(MV_SWCOMMON_BASE)/src/*.c \
                                     $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/cross/*.c)
endif
ifeq ($(MV_CPU_ARCH), sparc)
	LEON_DRIVER_C_SOURCES  += $(foreach var, $(SOC_REV_DIR), $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/arch/$(var)/src/*/*.c \
							                   $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/bm/arch/$(var)/src/*.c \
											   $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/src/*.c \
											   $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/src/*.c \
											   $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/hgl/src/*.c \
											   $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/sgl/src/*.c) )
else ifeq ($(MV_CPU_ARCH), armv8)
	ARMV8_DRIVER_C_SOURCES  += $(foreach var, $(SOC_REV_DIR), $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/arch/$(var)/src/*/*.c \
                                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/ARMv8-A/bm/arch/$(var)/src/*.c \
		                                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/bm/arch/$(var)/src/*.c \
		                                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/hgl/arch/$(var)/src/*.c \
		                                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/hgl/src/*.c \
		                                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/ARMv8-A/sgl/src/*.c) )
	else
	   $(error Unsupported MV_CPU_ARCH for OS core: $(MV_CPU_ARCH))
   endif

# Sources for RTEMS drivers
ifeq ($(MV_SOC_OS), rtems)

$(foreach var, $(SOC_REV_DIR), $(eval -include $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/arch/$(var)/src/*/*.mk)))

ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
  RTEMS_DRIVER_C_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/osDrivers/*/*.c)
  RTEMS_DRIVER_C_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/*.c)
  RTEMS_DRIVER_C_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/*/*.c)
  RTEMS_DRIVER_C_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/src/*.c)
  RTEMS_DRIVER_C_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/src/*/*.c)
else
# This is required because the I2C driver implementation for MyriadX differs
# from the one for Myriad2
  RTEMS_DRIVER_C_SOURCES += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/arch/$(var)/src/*.c))
  # Include icDrivers content, if existing
  RTEMS_DRIVER_C_SOURCES += $(RTEMS_IC_DRIVER_C_SOURCES)
  CCOPT_EXTRA += $(RTEMS_IC_DRIVER_CCOPT)
endif

  RTEMS_DRIVER_C_SOURCES += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/$(var)/src/*.c) )
  RTEMS_DRIVER_C_SOURCES += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/$(var)/src/*/*.c))
  ifeq ($(MV_SOC_OS_LIBBSD), 1)
    RTEMS_DRIVER_C_SOURCES += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems-libbsd/arch/$(var)/src/*.c))
    RTEMS_DRIVER_C_SOURCES += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems-libbsd/arch/$(var)/src/*/*.c))
  endif
endif

# Sources for RTEMS drivers
ifeq ($(MV_SOC_OS_LRT), rtems)

$(foreach var, $(SOC_REV_DIR), $(eval -include $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/arch/$(var)/src/*/*.mk)))

ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485))
  RTEMS_DRIVER_C_SOURCES_LRT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/osDrivers/*/*.c)
  RTEMS_DRIVER_C_SOURCES_LRT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/*.c)
  RTEMS_DRIVER_C_SOURCES_LRT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/*/*.c)
  RTEMS_DRIVER_C_SOURCES_LRT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/src/*.c)
  RTEMS_DRIVER_C_SOURCES_LRT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/src/*/*.c)
else
# This is required because the I2C driver implementation for MyriadX differs
# from the one for Myriad2
  RTEMS_DRIVER_C_SOURCES_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/arch/$(var)/src/*.c))
  # Include icDrivers content, if existing
  RTEMS_DRIVER_C_SOURCES_LRT += $(RTEMS_IC_DRIVER_C_SOURCES)
  CCOPT_EXTRA_LRT += $(RTEMS_IC_DRIVER_CCOPT)
endif

  RTEMS_DRIVER_C_SOURCES_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/$(var)/src/*.c) )
  RTEMS_DRIVER_C_SOURCES_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/$(var)/src/*/*.c))
  ifeq ($(MV_SOC_OS_LRT_LIBBSD), 1)
    RTEMS_DRIVER_C_SOURCES_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems-libbsd/arch/$(var)/src/*.c))
    RTEMS_DRIVER_C_SOURCES_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems-libbsd/arch/$(var)/src/*/*.c))
  endif
endif

# Driver Assembly Files LOS bare-metal
ifeq ($(MV_SOC_OS), none)
    ifeq ($(MV_CPU_ARCH), sparc)
        LEON_DRIVER_ASM_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/bm/asm/*.S)
		ifeq ($(MV_SOC_REV),ma2490)
		  LEON_DRIVER_ASM_SOURCES += $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/bm/arch/ma2x9x/asm/traps/traps_$(MV_LEON_TRAP_TYPE).S
		else
        LEON_DRIVER_ASM_SOURCES += $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/bm/asm/traps/traps_$(MV_LEON_TRAP_TYPE).S
	    endif
        LEON_DRIVER_ASM_SOURCES += $(wildcard $(MV_LEON_LIBC_BASE)/src/asm/*.S)
    else ifeq ($(MV_CPU_ARCH), armv8)
	    ifeq ($(MV_CPU_ARCH_64BIT),yes)
        ARMV8_DRIVER_ASM_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/ARMv8-A/bm/bootcode64/*.S)
        ARMV8_DRIVER_ASM_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/ARMv8-A/bm/bootcode64/*.S)
        ARMV8_DRIVER_ASM_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/bm/arch/ma2x9x/src/*.S)
        ARMV8_DRIVER_ASM_SOURCES += $(wildcard $(MV_SWCOMMON_BASE)/asm/*.S)
        ARMV8_DRIVER_C_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/ARMv8-A/bm/bootcode64/*.c)
	    else
		ARMV8_DRIVER_ASM_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/ARMv8-A/bm/bootcode/*.S)
        ARMV8_DRIVER_C_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/ARMv8-A/bm/bootcode/*.c)
	    endif
    else
       $(error Unsupported MV_CPU_ARCH for OS core: $(MV_CPU_ARCH))
    endif
endif

# Driver Assembly Files LRT bare-metal
ifeq ($(MV_SOC_OS_LRT), none)
  LEON_DRIVER_ASM_SOURCES_LRT  += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/bm/asm/*.S)
		ifeq ($(MV_SOC_REV),ma2490)
		  LEON_DRIVER_ASM_SOURCES_LRT += $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/bm/arch/ma2x9x/asm/traps/traps_$(MV_LEON_TRAP_TYPE_LRT).S
		else
  LEON_DRIVER_ASM_SOURCES_LRT += $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/bm/asm/traps/traps_$(MV_LEON_TRAP_TYPE_LRT).S
	    endif
  LEON_DRIVER_ASM_SOURCES_LRT += $(wildcard $(MV_LEON_LIBC_BASE)/src/asm/*.S)
endif

# Driver Assembly Files LNN bare-metal
ifeq ($(MV_SOC_OS_LNN), none)
  LEON_DRIVER_ASM_SOURCES_LNN  += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/bm/asm/*.S)
		ifeq ($(MV_SOC_REV),ma2490)
		  LEON_DRIVER_ASM_SOURCES_LNN += $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/bm/arch/ma2x9x/asm/traps/traps_$(MV_LEON_TRAP_TYPE_LNN).S
		else
  LEON_DRIVER_ASM_SOURCES_LNN += $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/leon/bm/asm/traps/traps_$(MV_LEON_TRAP_TYPE_LNN).S
	    endif
  LEON_DRIVER_ASM_SOURCES_LNN += $(wildcard $(MV_LEON_LIBC_BASE)/src/asm/*.S)
endif

# Add the startup code for RTEMS LOS
ifeq ($(MV_SOC_OS), rtems)
  LEON_DRIVER_ASM_SOURCES += $(MV_RTEMS_ASM_STARTUP_CODE)
endif

# Add the startup code for RTEMS LRT
ifeq ($(MV_SOC_OS_LRT), rtems)
  LEON_DRIVER_ASM_SOURCES_LRT += $(MV_RTEMS_ASM_STARTUP_CODE)
endif

# Add the startup code for RTEMS LNN
ifeq ($(MV_SOC_OS_LNN), rtems)
  LEON_DRIVER_ASM_SOURCES_LNN += $(MV_RTEMS_ASM_STARTUP_CODE)
endif

###################################################################
#  Declaring the Application sources for Leon RT (Myriad2 Only)
###################################################################
LEON_RT_LIB_NAME       	?= leon_rt/leonRTApp

ifeq ($(LEON_RT_BUILD), yes)
	LEON_RT_APP_LIBS        = $(LEON_RT_LIB_NAME).mvlib
endif

#Add LeonNN in the mix
LEON_NN_LIB_NAME       	?= leon_nn/leonNNApp
ifeq ($(LEON_NN_BUILD), yes)
	LEON_NN_APP_LIBS        = $(LEON_NN_LIB_NAME).mvlib
endif

LEON_RT_C_SOURCES      += $(wildcard $(DirAppRoot)/leon_rt/*.c)
LEON_RT_C_SOURCES      += $(wildcard $(DirAppRoot)/leon_rt/*/*.c)
LEON_RT_C_SOURCES      += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_rt/*.c))
LEON_RT_C_SOURCES      += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_rt/*/*.c))
LEON_RT_CPP_SOURCES    += $(wildcard $(DirAppRoot)/leon_rt/*.cpp)
LEON_RT_CPP_SOURCES    += $(wildcard $(DirAppRoot)/leon_rt/*/*.cpp)
LEON_RT_CPP_SOURCES    += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_rt/*.cpp))
LEON_RT_CPP_SOURCES    += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_rt/*/*.cpp))
LEON_RT_ASM_SOURCES    += $(wildcard $(DirAppRoot)/leon_rt/*.S)
LEON_RT_ASM_SOURCES    += $(wildcard $(DirAppRoot)/leon_rt/*/*.S)
LEON_RT_ASM_SOURCES    += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_rt/*.S))
LEON_RT_ASM_SOURCES    += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_rt/*/*.S))
LEON_RT_APP_OBJS       += $(patsubst %.c,$(DirAppObjBase)%_lrt.o,$(LEON_RT_C_SOURCES))
LEON_RT_APP_OBJS       += $(patsubst %.cpp,$(DirAppObjBase)%_lrt.o,$(LEON_RT_CPP_SOURCES))
LEON_RT_APP_OBJS       += $(patsubst %.S,$(DirAppObjBase)%_lrt.o,$(LEON_RT_ASM_SOURCES))

#Adding LeonNN sources
LEON_NN_C_SOURCES      += $(wildcard $(DirAppRoot)/leon_nn/*.c)
LEON_NN_C_SOURCES      += $(wildcard $(DirAppRoot)/leon_nn/*/*.c)
LEON_NN_C_SOURCES      += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_nn/*.c))
LEON_NN_C_SOURCES      += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_nn/*/*.c))
LEON_NN_CPP_SOURCES    += $(wildcard $(DirAppRoot)/leon_nn/*.cpp)
LEON_NN_CPP_SOURCES    += $(wildcard $(DirAppRoot)/leon_nn/*/*.cpp)
LEON_NN_CPP_SOURCES    += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_nn/*.cpp))
LEON_NN_CPP_SOURCES    += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_nn/*/*.cpp))
LEON_NN_ASM_SOURCES    += $(wildcard $(DirAppRoot)/leon_nn/*.S)
LEON_NN_ASM_SOURCES    += $(wildcard $(DirAppRoot)/leon_nn/*/*.S)
LEON_NN_ASM_SOURCES    += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_nn/*.S))
LEON_NN_ASM_SOURCES    += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/leon_nn/*/*.S))
LEON_NN_APP_OBJS       += $(patsubst %.c,$(DirAppObjBase)%_lrt.o,$(LEON_NN_C_SOURCES))
LEON_NN_APP_OBJS       += $(patsubst %.cpp,$(DirAppObjBase)%_lrt.o,$(LEON_NN_CPP_SOURCES))
LEON_NN_APP_OBJS       += $(patsubst %.S,$(DirAppObjBase)%_lrt.o,$(LEON_NN_ASM_SOURCES))

##########################################################################
# Optional Build of LeonRT application for Myriad2 Projects
##########################################################################
ifeq ($(LEON_RT_BUILD), yes)
	LEON_RT_APPS           = $(LEON_RT_LIB_NAME).rtlib
endif

#Add Leon NN in the mix
ifeq ($(LEON_NN_BUILD), yes)
	LEON_NN_APPS           = $(LEON_NN_LIB_NAME).nnlib
endif

###################################################################
#  Declaring the SHAVE applications used by the app.              #
###################################################################
#First autodetect the applications
SHAVE_SEARCH_PATH ?= shaveApps
AUTO_DETECTED_SHAVE_APPS = $(wildcard $(SHAVE_SEARCH_PATH)/*)

define shaveSourcesRulesTemplate
#Choosing C sources the brthYASM application on shave
SHAVE_C_AUTOSTAT_SOURCES_$(1) = $$(wildcard $$(DirAppRoot)/$$(SHAVE_SEARCH_PATH)/$(1)/*.c)
SHAVE_CPP_AUTOSTAT_SOURCES_$(1) = $$(wildcard $$(DirAppRoot)/$$(SHAVE_SEARCH_PATH)/$(1)/*.cpp)
SHAVE_ASM_AUTOSTAT_SOURCES_$(1) = $$(wildcard $$(DirAppRoot)/$$(SHAVE_SEARCH_PATH)/$(1)/*.asm)

#Generating list of required generated assembly files for the brthYASM app on shave
SHAVE_GENASMS_AUTOSTAT_$(1)  = $$(patsubst %.c,$$(DirAppObjBase)%.asmgen,$$(SHAVE_C_AUTOSTAT_SOURCES_$(1)))
SHAVE_GENASMS_AUTOSTAT_$(1) += $$(patsubst %.cpp,$$(DirAppObjBase)%.asmgen,$$(SHAVE_CPP_AUTOSTAT_SOURCES_$(1)))
#Generating required objects list from sources
SHAVE_$(1)_AUTOSTAT_OBJS = $$(patsubst $$(DirAppObjBase)%.asmgen,$$(DirAppObjBase)%_shave.o,$$(SHAVE_GENASMS_AUTOSTAT_$(1))) \
					$$(patsubst %.asm,$$(DirAppObjBase)%_shave.o,$$(SHAVE_ASM_AUTOSTAT_SOURCES_$(1)))

#update clean rules with our generated files
PROJECTCLEAN += $$(SHAVE_GENASMS_AUTOSTAT_$(1)) $$(SHAVE_$(1)_AUTOSTAT_OBJS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $$(SHAVE_GENASMS_AUTOSTAT_$(1))

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS += $$(DirAppOutput)/$(1).mvlib
SHAVE0_APPS += $$(DirAppOutput)/$(1).shv0lib
SHAVE1_APPS += $$(DirAppOutput)/$(1).shv1lib
SHAVE2_APPS += $$(DirAppOutput)/$(1).shv2lib
SHAVE3_APPS += $$(DirAppOutput)/$(1).shv3lib
SHAVE4_APPS += $$(DirAppOutput)/$(1).shv4lib
SHAVE5_APPS += $$(DirAppOutput)/$(1).shv5lib
SHAVE6_APPS += $$(DirAppOutput)/$(1).shv6lib
SHAVE7_APPS += $$(DirAppOutput)/$(1).shv7lib
SHAVE8_APPS += $$(DirAppOutput)/$(1).shv8lib
SHAVE9_APPS += $$(DirAppOutput)/$(1).shv9lib
SHAVE10_APPS += $$(DirAppOutput)/$(1).shv10lib
SHAVE11_APPS += $$(DirAppOutput)/$(1).shv11lib
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
SHAVE12_APPS += $$(DirAppOutput)/$(1).shv12lib
SHAVE13_APPS += $$(DirAppOutput)/$(1).shv13lib
SHAVE14_APPS += $$(DirAppOutput)/$(1).shv14lib
SHAVE15_APPS += $$(DirAppOutput)/$(1).shv15lib
endif
endef

$(foreach app,$(AUTO_DETECTED_SHAVE_APPS),$(eval $(call shaveSourcesRulesTemplate,$(lastword $(subst /, ,$(app))))))

#--------------same as above for SHAVENN now-----------------
SHAVE_NN_SEARCH_PATH ?= shaveNNApps
AUTO_DETECTED_SHAVE_NN_APPS = $(wildcard $(SHAVE_NN_SEARCH_PATH)/*)

define shaveSourcesRulesTemplate
#Choosing C sources the brthYASM application on shave
SHAVE_NN_C_AUTOSTAT_SOURCES_$(1) = $$(wildcard $$(DirAppRoot)/$$(SHAVE_NN_SEARCH_PATH)/$(1)/*.c)
SHAVE_NN_CPP_AUTOSTAT_SOURCES_$(1) = $$(wildcard $$(DirAppRoot)/$$(SHAVE_NN_SEARCH_PATH)/$(1)/*.cpp)
SHAVE_NN_ASM_AUTOSTAT_SOURCES_$(1) = $$(wildcard $$(DirAppRoot)/$$(SHAVE_NN_SEARCH_PATH)/$(1)/*.asm)

#Generating list of required generated assembly files for the brthYASM app on shave
SHAVE_NN_GENASMS_AUTOSTAT_$(1)  = $$(patsubst %.c,$$(DirAppObjBase)%.asmgenNN,$$(SHAVE_NN_C_AUTOSTAT_SOURCES_$(1)))
SHAVE_NN_GENASMS_AUTOSTAT_$(1) += $$(patsubst %.cpp,$$(DirAppObjBase)%.asmgenNN,$$(SHAVE_NN_CPP_AUTOSTAT_SOURCES_$(1)))
#Generating required objects list from sources
SHAVE_NN_$(1)_AUTOSTAT_OBJS = $$(patsubst $$(DirAppObjBase)%.asmgenNN,$$(DirAppObjBase)%_shaveNN.o,$$(SHAVE_NN_GENASMS_AUTOSTAT_$(1))) \
					$$(patsubst %.asm,$$(DirAppObjBase)%_shaveNN.o,$$(SHAVE_NN_ASM_AUTOSTAT_SOURCES_$(1)))

#update clean rules with our generated files
PROJECTCLEAN += $$(SHAVE_NN_GENASMS_AUTOSTAT_$(1)) $$(SHAVE_NN_$(1)_AUTOSTAT_OBJS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $$(SHAVE_NN_GENASMS_AUTOSTAT_$(1))

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_NN_APP_LIBS += $$(DirAppOutput)/$(1).mvlib
SHAVE0_NN_APPS += $$(DirAppOutput)/$(1).shvNN0lib
SHAVE1_NN_APPS += $$(DirAppOutput)/$(1).shvNN1lib
SHAVE2_NN_APPS += $$(DirAppOutput)/$(1).shvNN2lib
SHAVE3_NN_APPS += $$(DirAppOutput)/$(1).shvNN3lib
SHAVE4_NN_APPS += $$(DirAppOutput)/$(1).shvNN4lib
SHAVE5_NN_APPS += $$(DirAppOutput)/$(1).shvNN5lib
SHAVE6_NN_APPS += $$(DirAppOutput)/$(1).shvNN6lib
SHAVE7_NN_APPS += $$(DirAppOutput)/$(1).shvNN7lib
SHAVE8_NN_APPS += $$(DirAppOutput)/$(1).shvNN8lib
SHAVE9_NN_APPS += $$(DirAppOutput)/$(1).shvNN9lib
SHAVE10_NN_APPS += $$(DirAppOutput)/$(1).shvNN10lib
SHAVE11_NN_APPS += $$(DirAppOutput)/$(1).shvNN11lib
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
SHAVE12_NN_APPS += $$(DirAppOutput)/$(1).shvNN12lib
SHAVE13_NN_APPS += $$(DirAppOutput)/$(1).shvNN13lib
SHAVE14_NN_APPS += $$(DirAppOutput)/$(1).shvNN14lib
SHAVE15_NN_APPS += $$(DirAppOutput)/$(1).shvNN15lib
SHAVE16_NN_APPS += $$(DirAppOutput)/$(1).shvNN16lib
SHAVE17_NN_APPS += $$(DirAppOutput)/$(1).shvNN17lib
SHAVE18_NN_APPS += $$(DirAppOutput)/$(1).shvNN18lib
SHAVE19_NN_APPS += $$(DirAppOutput)/$(1).shvNN19lib
endif
endef

$(foreach app,$(AUTO_DETECTED_SHAVE_NN_APPS),$(eval $(call shaveSourcesRulesTemplate,$(lastword $(subst /, ,$(app))))))
#--------------end of SHAVE_NN-------------------------------


#Add rules for dynamic loading sources
define cAppDynamicSourcesRule_template
#Choosing C sources the application on shave
SHAVE_C_AUTODYN_SOURCES_$(1) =  $$(wildcard $$(DirAppRoot)/$$(SHAVE_DEFAULT_DYN_SEARCH_PATH)/$(1)/*.c)
SHAVE_C_AUTODYN_SOURCES_$(1) += $(MV_COMMON_BASE)/drivers/myriad2/socDrivers/shared/src/ShaveData.c
ifeq ($(SHAVE_COMPONENTS),yes)
SHAVE_C_AUTODYN_SOURCES_$(1) += $(foreach component,$(ComponentList_SVE), $(wildcard $(MV_COMPONENTS_DIR)/$(component)/$(SHAVE_DEFAULT_COMPONENT_DYN_SEARCH_PATH)/$(1)/*.c))
SHAVE_C_AUTODYN_SOURCES_$(1) += $(foreach component,$(ComponentList_SVE), $(wildcard $(MV_COMPONENTS_DIR)/$(component)/$(SHAVE_DEFAULT_COMPONENT_DYN_SEARCH_PATH)/*.c))
endif
#Choosing C++ sources the application on shave
SHAVE_CPP_AUTODYN_SOURCES_$(1) = $$(wildcard $$(DirAppRoot)/$$(SHAVE_DEFAULT_DYN_SEARCH_PATH)/$(1)/*.cpp)
ifeq ($(SHAVE_COMPONENTS),yes)
SHAVE_CPP_AUTODYN_SOURCES_$(1) += $(foreach component,$(ComponentList_SVE), $(wildcard $(MV_COMPONENTS_DIR)/$(component)/$(SHAVE_DEFAULT_COMPONENT_DYN_SEARCH_PATH)/$(1)/*.cpp))
SHAVE_CPP_AUTODYN_SOURCES_$(1) += $(foreach component,$(ComponentList_SVE), $(wildcard $(MV_COMPONENTS_DIR)/$(component)/$(SHAVE_DEFAULT_COMPONENT_DYN_SEARCH_PATH)/*.cpp))
endif

#Choosing ASM sources for the shave app on shave
SHAVE_ASM_AUTODYN_SOURCES_$(1) = $$(wildcard $$(DirAppRoot)/$$(SHAVE_DEFAULT_DYN_SEARCH_PATH)/$(1)/*.asm)
ifeq ($(SHAVE_COMPONENTS),yes)
SHAVE_ASM_AUTODYN_SOURCES_$(1) += $(foreach component,$(ComponentList_SVE), $(wildcard $(MV_COMPONENTS_DIR)/$(component)/$(SHAVE_DEFAULT_COMPONENT_DYN_SEARCH_PATH)/$(1)/*.asm))
SHAVE_ASM_AUTODYN_SOURCES_$(1) += $(foreach component,$(ComponentList_SVE), $(wildcard $(MV_COMPONENTS_DIR)/$(component)/$(SHAVE_DEFAULT_COMPONENT_DYN_SEARCH_PATH)/*.asm))
endif

#Generating list of required generated assembly files for the app on shave
SHAVE_AUTODYN_GENASMS_$(1)  = $$(patsubst %.c,$$(DirAppObjBase)%.asmgen,$$(SHAVE_C_AUTODYN_SOURCES_$(1)))
SHAVE_AUTODYN_GENASMS_$(1) += $$(patsubst %.cpp,$$(DirAppObjBase)%.asmgen,$$(SHAVE_CPP_AUTODYN_SOURCES_$(1)))

#Generating required objects list from sources
SHAVE_$(1)_AUTODYN_OBJS  = $$(patsubst $$(DirAppObjBase)%.asmgen,$$(DirAppObjBase)%_shave.o,$$(SHAVE_AUTODYN_GENASMS_$(1)))
SHAVE_$(1)_AUTODYN_OBJS += $$(patsubst %.asm,$$(DirAppObjBase)%_shave.o,$$(SHAVE_ASM_AUTODYN_SOURCES_$(1)))
PROJECTCLEAN += $(SHAVE_AUTODYN_GENASMS_$(1)) $(SHAVE_$(1)_AUTODYN_OBJS)

endef

$(foreach app,$(shaveXApps),$(eval $(call cAppDynamicSourcesRule_template,$(lastword $(subst /, ,$(app))),$(app))))

SHAVE_APP_LIBS ?=
SHAVE_NN_APP_LIBS ?=

define SHAVE_ADD_TO_TEMP_APPS
  SHAVE$(1)_APPS ?=

  # Temporary libs used for global symbol generation
  SHAVE$(1)_TEMP_APPS = $(patsubst %$(1)lib, %$(1)templib, $$(SHAVE$(1)_APPS))
  ALL_SHAVE_TEMP_APPS += $$(SHAVE$(1)_TEMP_APPS)
endef

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
SHAVES_IDX = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
else
SHAVES_IDX = 0 1 2 3 4 5 6 7 8 9 10 11
endif
$(foreach idx,$(SHAVES_IDX),$(eval $(call SHAVE_ADD_TO_TEMP_APPS,$(idx))))

#And SHAVE NN apps
define SHAVE_NN_ADD_TO_TEMP_APPS
  SHAVE$(1)_NN_APPS ?=

  # Temporary libs used for global symbol generation
  SHAVE$(1)_NN_TEMP_APPS = $(patsubst %$(1)lib, %$(1)templib, $$(SHAVE$(1)_NN_APPS))
  ALL_SHAVE_NN_TEMP_APPS += $$(SHAVE$(1)_NN_TEMP_APPS)
endef

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
SHAVES_NN_IDX = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
else
#Other architectures don't have these
SHAVE_NN_IDX =
endif
$(foreach idx_nn,$(SHAVES_IDX),$(eval $(call SHAVE_NN_ADD_TO_TEMP_APPS,$(idx_nn))))


###################################################################
#       Building up list of Leon low level driver h files         #
###################################################################

LEON_COMPONENT_HEADERS_LOS += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_PATHS_LOS)))
LEON_COMPONENT_HEADERS_LOS += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_HEADERS_PATHS_LOS)))
LEON_COMPONENT_HEADERS_LOS += $(wildcard $(addsuffix /*.hpp,$(LEON_COMPONENT_HEADERS_PATHS_LOS)))

LEON_COMPONENT_HEADERS_LOS += $(foreach var, $(SOC_REV_DIR),$(wildcard $(addsuffix /*.h, $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/include,$(ComponentList_LOS)))))
LEON_COMPONENT_HEADERS_LOS += $(foreach var, $(SOC_REV_DIR),$(wildcard $(addsuffix /*.h, $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/bm/include,$(ComponentList_LOS)))))
LEON_COMPONENT_HEADERS_LOS += $(foreach var, $(SOC_REV_DIR),$(wildcard $(addsuffix /*.h, $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/include,$(ComponentList_LOS)))))

LEON_COMPONENT_HEADERS_BM_LOS += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_PATHS_BM_LOS)))
LEON_COMPONENT_HEADERS_RTEMS_LOS += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_PATHS_RTEMS_LOS)))

LEON_COMPONENT_HEADERS_LRT += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_PATHS_LRT)))
LEON_COMPONENT_HEADERS_LRT += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_HEADERS_PATHS_LRT)))

LEON_COMPONENT_HEADERS_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(addsuffix /*.h, $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/include,$(ComponentList_LRT)))))
LEON_COMPONENT_HEADERS_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(addsuffix /*.h, $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/bm/include,$(ComponentList_LRT)))))
LEON_COMPONENT_HEADERS_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(addsuffix /*.h, $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/include,$(ComponentList_LRT)))))

LEON_COMPONENT_HEADERS_BM_LRT += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_PATHS_BM_LRT)))
LEON_COMPONENT_HEADERS_RTEMS_LRT += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_PATHS_RTEMS_LRT)))

#and same for LeonNN
LEON_COMPONENT_HEADERS_LNN += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_PATHS_LNN)))
LEON_COMPONENT_HEADERS_LNN += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_HEADERS_PATHS_LNN)))

LEON_COMPONENT_HEADERS_LNN += $(foreach var, $(SOC_REV_DIR),$(wildcard $(addsuffix /*.h, $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/common/include,$(ComponentList_LNN)))))
LEON_COMPONENT_HEADERS_LNN += $(foreach var, $(SOC_REV_DIR),$(wildcard $(addsuffix /*.h, $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/leon/bm/include,$(ComponentList_LNN)))))
LEON_COMPONENT_HEADERS_LNN += $(foreach var, $(SOC_REV_DIR),$(wildcard $(addsuffix /*.h, $(patsubst %,$(MV_COMPONENTS_DIR)/%/arch/$(var)/shared/include,$(ComponentList_LNN)))))

LEON_COMPONENT_HEADERS_BM_LNN += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_PATHS_BM_LNN)))
LEON_COMPONENT_HEADERS_RTEMS_LNN += $(wildcard $(addsuffix /*.h,$(LEON_COMPONENT_PATHS_RTEMS_LNN)))

ARM_COMPONENT_HEADERS += $(wildcard $(addsuffix /*.h,$(ARM_COMPONENT_PATHS)))
ARM_COMPONENT_HEADERS += $(wildcard $(addsuffix /*.h,$(ARM_COMPONENT_HEADERS_PATHS)))
ARM_COMPONENT_HEADERS += $(wildcard $(addsuffix /*.hpp,$(ARM_COMPONENT_HEADERS_PATHS)))
############################################
# LEON HEADERS for LEON OS
############################################
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
# For now, MA2x8x support doesn't use the common drivers as most of them are not
# applicable to the new architecture
LEON_HEADERS_INIT += $(wildcard $(MV_SWCOMMON_IC)/src/*.h \
                           $(MV_SWCOMMON_BASE)/src/*.h \
                           $(MV_SWCOMMON_IC)/include/*.h \
                           $(MV_SWCOMMON_BASE)/include/*.h)
ARMV8_HEADERS_INIT += $(wildcard $(MV_SWCOMMON_IC)/src/*.h \
                           $(MV_SWCOMMON_BASE)/src/*.h \
                           $(MV_SWCOMMON_IC)/include/*.h \
                           $(MV_SWCOMMON_BASE)/include/*.h)
else
LEON_HEADERS_INIT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include/*/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/src/*/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/include/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include/*/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/src/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/src/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/include/*.h \
                           $(MV_SWCOMMON_IC)/src/*.h \
                           $(MV_SWCOMMON_BASE)/src/*.h \
                           $(MV_SWCOMMON_IC)/include/*.h \
                           $(MV_SWCOMMON_BASE)/include/*.h)
endif


ifeq ($(MV_CPU_ARCH), sparc)
LEON_HEADERS_INIT  += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/arch/$(var)/include/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/include/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/src/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/include/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/src/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/include/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/hgl/include/*.h \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/sgl/include/*.h) )
endif
ifeq ($(MV_CPU_ARCH), armv8)
	ARMV8_HEADERS_INIT  += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/arch/$(var)/include/*.h \
	                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/bm/arch/$(var)/include/*.h \
	                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/bm/arch/$(var)/src/*.h \
	                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/hgl/arch/$(var)/include/*.h \
	                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/ARMv8-A/hgl/arch/$(var)/src/*.h \
	                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/include/*.h \
	                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/hgl/include/*.h \
	                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/system/ARMv8-A/bm/bootcode/*.hs \
	                       $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/ARMv8-A/sgl/include/*.h) )
endif

# Only pull in MDK libc when we are not using RTEMS LOS
ifneq ($(MV_SOC_OS), rtems)
ifeq ($(MV_CPU_ARCH),armv8)
ARMV8_HEADERS_INIT += $(wildcard $(MV_ARM_LIBC_BASE)/include)
ARMV8_HEADERS_INIT += $(ARM_COMPONENT_HEADERS)
else
LEON_HEADERS_INIT += $(wildcard $(MV_LEON_LIBC_BASE)/include)
endif
endif
#and add the ones from our project by default
ifneq ($(MV_CPU_ARCH),armv8)
LEON_HEADERS_INIT += $(wildcard $(DirAppRoot)/*leon*/*.h)
LEON_HEADERS_INIT += $(wildcard $(DirAppRoot)/*leon*/*/*.h)
LEON_HEADERS_INIT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon*/*.h))
LEON_HEADERS_INIT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon*/*/*.h))
LEON_HEADERS_INIT += $(LEON_COMPONENT_HEADERS_LOS)
endif

ifeq ($(MV_SOC_OS), rtems)
  #Add the ones from our project by default
  LEON_HEADERS_INIT += $(wildcard $(DirAppRoot)/*rtems*/*.h)
  LEON_HEADERS_INIT += $(wildcard $(DirAppRoot)/*rtems*/*/*.h)
  # RTEMS drivers header files
ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
  LEON_HEADERS_INIT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/include/*.h)
  LEON_HEADERS_INIT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/src/*.h)
else
# This is required because the I2C driver implementation for MyriadX differs
# from the one for Myriad2
  LEON_HEADERS_INIT += $(foreach var,$(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/arch/$(var)/include/*.h))
endif
  LEON_HEADERS_INIT += $(foreach var,$(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/$(var)/include/*.h))
  LEON_HEADERS_INIT += $(foreach var,$(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/$(var)/src/*.h))
  ifeq ($(MV_SOC_OS_LIBBSD), 1)
    LEON_HEADERS_INIT += $(foreach var,$(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems-libbsd/arch/$(var)/include/*.h))
    LEON_HEADERS_INIT += $(foreach var,$(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems-libbsd/arch/$(var)/src/*.h))
  endif
  LEON_HEADERS_INIT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/osDrivers/include/*.h)
  LEON_HEADERS_INIT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/include/*.h)

  #Add RTEMS headers
  LEON_HEADERS_INIT += $(LEON_COMPONENT_HEADERS_RTEMS_LOS)
else
  LEON_HEADERS_INIT += $(LEON_COMPONENT_HEADERS_BM_LOS)
endif

#sort all LEON_HEADERS_INIT because duplicate elimination is needed
LEON_HEADERS += $(sort $(wildcard $(LEON_HEADERS_INIT)))

#sort all LEON_HEADERS_INIT because duplicate elimination is needed
ARMV8_HEADERS += $(sort $(wildcard $(ARMV8_HEADERS_INIT)))
############################################
# LEON HEADERS for LEON LRT
############################################

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
# For now, MA2x8x support doesn't use the common drivers as most of them are not
# applicable to the new architecture
LEON_HEADERS_INIT_LRT += $(wildcard $(MV_SWCOMMON_IC)/src/*.h \
                               $(MV_SWCOMMON_BASE)/src/*.h \
                               $(MV_SWCOMMON_IC)/include/*.h \
                               $(MV_SWCOMMON_BASE)/include/*.h)
else
LEON_HEADERS_INIT_LRT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include/*/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/src/*/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include/*/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/src/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/src/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/include/*.h \
                               $(MV_SWCOMMON_IC)/src/*.h \
                               $(MV_SWCOMMON_BASE)/src/*.h \
                               $(MV_SWCOMMON_IC)/include/*.h \
                               $(MV_SWCOMMON_BASE)/include/*.h)
endif

LEON_HEADERS_INIT_LRT  += $(foreach var, $(SOC_REV_DIR), $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/arch/$(var)/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/src/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/src/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/hgl/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/sgl/include/*.h) )


# Only pull in MDK libc when we are not using RTEMS LOS
ifneq ($(MV_SOC_OS_LRT), rtems)
LEON_HEADERS_INIT_LRT += $(wildcard $(MV_LEON_LIBC_BASE)/include)
endif

#and add the ones from our project by default
LEON_HEADERS_INIT_LRT += $(wildcard $(DirAppRoot)/*leon*/*.h)
LEON_HEADERS_INIT_LRT += $(wildcard $(DirAppRoot)/*leon*/*/*.h)
LEON_HEADERS_INIT_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon*/*.h))
LEON_HEADERS_INIT_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon*/*/*.h))
LEON_HEADERS_INIT_LRT += $(LEON_COMPONENT_HEADERS_LRT)


ifeq ($(MV_SOC_OS_LRT), rtems)
  #Add the ones from our project by default
  LEON_HEADERS_INIT_LRT += $(wildcard $(DirAppRoot)/*rtems*/*.h)
  LEON_HEADERS_INIT_LRT += $(wildcard $(DirAppRoot)/*rtems*/*/*.h)
  # RTEMS drivers header files
ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
  LEON_HEADERS_INIT_LRT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/include/*.h)
  LEON_HEADERS_INIT_LRT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/src/*.h)
else
# This is required because the I2C driver implementation for MyriadX differs
# from the one for Myriad2
  LEON_HEADERS_INIT_LRT += $(foreach var,$(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/arch/$(var)/include/*.h))
endif
  LEON_HEADERS_INIT_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/$(var)/include/*.h))
  LEON_HEADERS_INIT_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/$(var)/src/*.h))
  ifeq ($(MV_SOC_OS_LRT_LIBBSD), 1)
    LEON_HEADERS_INIT_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems-libbsd/arch/$(var)/include/*.h))
    LEON_HEADERS_INIT_LRT += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems-libbsd/arch/$(var)/src/*.h))
  endif
  LEON_HEADERS_INIT_LRT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/osDrivers/include/*.h)
  LEON_HEADERS_INIT_LRT += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/include/*.h)
   #Add RTEMS headers
  LEON_HEADERS_INIT_LRT += $(LEON_COMPONENT_HEADERS_RTEMS_LRT)
else
  LEON_HEADERS_INIT_LRT += $(LEON_COMPONENT_HEADERS_BM_LRT)
endif

LEON_HEADERS_LRT += $(sort $(wildcard $(LEON_HEADERS_INIT_LRT)))

############################################
# LEON HEADERS for LEON NN
############################################

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
# For now, MA2x8x support doesn't use the common drivers as most of them are not
# applicable to the new architecture
LEON_HEADERS_INIT_LNN += $(wildcard $(MV_SWCOMMON_IC)/src/*.h \
                               $(MV_SWCOMMON_BASE)/src/*.h \
                               $(MV_SWCOMMON_IC)/include/*.h \
                               $(MV_SWCOMMON_BASE)/include/*.h)
else
LEON_HEADERS_INIT_LNN += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/include/*/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/src/*/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/include/*/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/src/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/src/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/include/*.h \
                               $(MV_SWCOMMON_IC)/src/*.h \
                               $(MV_SWCOMMON_BASE)/src/*.h \
                               $(MV_SWCOMMON_IC)/include/*.h \
                               $(MV_SWCOMMON_BASE)/include/*.h)
endif

LEON_HEADERS_INIT_LNN  += $(foreach var, $(SOC_REV_DIR), $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/arch/$(var)/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/bm/arch/$(var)/src/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/hgl/arch/$(var)/src/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/hgl/include/*.h \
                               $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/sgl/include/*.h) )


# Only pull in MDK libc when we are not using RTEMS LOS
ifneq ($(MV_SOC_OS_LNN), rtems)
LEON_HEADERS_INIT_LNN += $(wildcard $(MV_LEON_LIBC_BASE)/include/*.h)
endif

#and add the ones from our project by default
LEON_HEADERS_INIT_LNN += $(wildcard $(DirAppRoot)/*leon*/*.h)
LEON_HEADERS_INIT_LNN += $(wildcard $(DirAppRoot)/*leon*/*/*.h)
LEON_HEADERS_INIT_LNN += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon*/*.h))
LEON_HEADERS_INIT_LNN += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*leon*/*/*.h))
LEON_HEADERS_INIT_LNN += $(LEON_COMPONENT_HEADERS_LNN)


ifeq ($(MV_SOC_OS_LNN), rtems)
  #Add the ones from our project by default
  LEON_HEADERS_INIT_LNN += $(wildcard $(DirAppRoot)/*rtems*/*.h)
  LEON_HEADERS_INIT_LNN += $(wildcard $(DirAppRoot)/*rtems*/*/*.h)
  # RTEMS drivers header files
ifneq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
  LEON_HEADERS_INIT_LNN += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/include/*.h)
  LEON_HEADERS_INIT_LNN += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/src/*.h)
else
# This is required because the I2C driver implementation for MyriadX differs
# from the one for Myriad2
  LEON_HEADERS_INIT_LNN += $(foreach var,$(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/arch/$(var)/include/*.h))
endif
  LEON_HEADERS_INIT_LNN += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/$(var)/include/*.h))
  LEON_HEADERS_INIT_LNN += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems/arch/$(var)/src/*.h))
  ifeq ($(MV_SOC_OS_LNN_LIBBSD), 1)
    LEON_HEADERS_INIT_LNN += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems-libbsd/arch/$(var)/include/*.h))
    LEON_HEADERS_INIT_LNN += $(foreach var, $(SOC_REV_DIR),$(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/leon/rtems-libbsd/arch/$(var)/src/*.h))
  endif
  LEON_HEADERS_INIT_LNN += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/brdDrivers/osDrivers/include/*.h)
  LEON_HEADERS_INIT_LNN += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/icDrivers/osDrivers/include/*.h)
   #Add RTEMS headers
  LEON_HEADERS_INIT_LNN += $(LEON_COMPONENT_HEADERS_RTEMS_LNN)
else
  LEON_HEADERS_INIT_LNN += $(LEON_COMPONENT_HEADERS_BM_LNN)
endif

LEON_HEADERS_LNN += $(sort $(wildcard $(LEON_HEADERS_INIT_LNN)))
###################################################################
# LEON HEADERS FOR LEON NN END                                    #
###################################################################

###################################################################
#       Building up list of Shave low level driver incl files     #
###################################################################

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2185 ma2180 ma2150 ma2155))
SHAVE_ASM_HEADERS = $(wildcard $(MV_SWCOMMON_BASE)/shave_code/$(MV_SOC_PLATFORM)/include/*.incl)
else
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
SHAVE_ASM_HEADERS = $(wildcard $(MV_SWCOMMON_SHAVE_BASE)/shave_code/$(MV_SOC_PLATFORM)/include/*.incl)
else
SHAVE_ASM_HEADERS = $(wildcard $(MV_SWCOMMON_BASE)/shave_code/$(MV_SOC_PLATFORM)/include/*.incl)
endif
endif
#and add the ones from our project by default
SHAVE_ASM_HEADERS += $(wildcard $(DirAppRoot)/shave/*.incl)

###################################################################
#       Building up list of Shave low level driver header files   #
###################################################################
SHAVE_HEADERS += $(wildcard $(DirAppRoot)/*shave*/*.h)
SHAVE_HEADERS += $(wildcard $(DirAppRoot)/*shave*/*/*.h)
SHAVE_HEADERS += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*shave*/*.h))
SHAVE_HEADERS += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*shave*/*/*.h))
SHAVE_HEADERS += $(wildcard $(DirAppRoot)/*shave*/*.hpp)
SHAVE_HEADERS += $(wildcard $(DirAppRoot)/*shave*/*/*.hpp)
SHAVE_HEADERS += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*shave*/*.hpp))
SHAVE_HEADERS += $(foreach var, $(SOC_REV_DIR),$(wildcard $(DirAppRoot)/arch/$(var)/*shave*/*/*.hpp))

#and the headers from components
SHAVE_COMPONENT_HEADERS  = $(wildcard $(addsuffix /*.h,$(SH_COMPONENTS_PATHS)))
SHAVE_COMPONENT_HEADERS += $(wildcard $(addsuffix /*.h,$(SH_COMPONENTS_HEADERS_PATHS)))
SHAVE_COMPONENT_HEADERS += $(wildcard $(addsuffix /*.hpp,$(SH_COMPONENTS_PATHS)))
SHAVE_COMPONENT_HEADERS += $(wildcard $(addsuffix /*.hpp,$(SH_COMPONENTS_HEADERS_PATHS)))


###################################################################
# Leon Object File declarations
###################################################################

# LEON OS
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.c,$(DirAppObjBase)%.o,$(LEON_LIB_C_SOURCES))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.c,$(DirAppObjBase)%.o,$(LEON_COMPONENT_C_SOURCES_LOS))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.cpp,$(DirAppObjBase)%.o,$(LEON_COMPONENT_CPP_SOURCES_LOS))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.S,$(DirAppObjBase)%.o,$(LEON_COMPONENT_ASM_SOURCES_LOS))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.c,$(DirAppObjBase)%.o,$(LEON_DRIVER_C_SOURCES))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.S,$(DirAppObjBase)%.o,$(LEON_DRIVER_ASM_SOURCES))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.c,%.o,$(LEON_APP_URC_C_REQUIRED))
ifeq ($(MV_SOC_OS), rtems)
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.c,$(DirAppObjBase)%.o,$(RTEMS_DRIVER_C_SOURCES))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.c,$(DirAppObjBase)%.o,$(LEON_COMPONENT_C_SOURCES_RTEMS_LOS))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.cpp,$(DirAppObjBase)%.o,$(LEON_COMPONENT_CPP_SOURCES_RTEMS_LOS))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.S,$(DirAppObjBase)%.o,$(LEON_COMPONENT_ASM_SOURCES_RTEMS_LOS))

else
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.c,$(DirAppObjBase)%.o,$(LEON_COMPONENT_C_SOURCES_BM_LOS))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.cpp,$(DirAppObjBase)%.o,$(LEON_COMPONENT_CPP_SOURCES_BM_LOS))
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.S,$(DirAppObjBase)%.o,$(LEON_COMPONENT_ASM_SOURCES_BM_LOS))

endif
#and also add the projects raw data files
LEON_SHARED_OBJECTS_REQUIRED_INIT += $(RAWDATAOBJECTFILES)

#and sort all LEON_SHARED_OBJECTS_REQUIRED_INIT because duplicate elimination is needed
LEON_SHARED_OBJECTS_REQUIRED += $(sort $(LEON_SHARED_OBJECTS_REQUIRED_INIT))

# ARMV8
ARMV8_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.c,$(DirAppObjBase)%.o,$(ARMV8_LIB_C_SOURCES))
ARMV8_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.c,$(DirAppObjBase)%.o,$(ARMV8_COMPONENT_C_SOURCES))
ARMV8_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.cpp,$(DirAppObjBase)%.o,$(ARMV8_COMPONENT_CPP_SOURCES))
ARMV8_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.S,$(DirAppObjBase)%.o,$(ARMV8_COMPONENT_ASM_SOURCES))
ARMV8_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.c,$(DirAppObjBase)%.o,$(ARMV8_DRIVER_C_SOURCES))
ARMV8_SHARED_OBJECTS_REQUIRED_INIT += $(patsubst %.S,$(DirAppObjBase)%.o,$(ARMV8_DRIVER_ASM_SOURCES))

#and also add the projects raw data files
ARMV8_SHARED_OBJECTS_REQUIRED_INIT += $(RAWDATAOBJECTFILES)

#and sort all ARMV8_SHARED_OBJECTS_REQUIRED_INIT because duplicate elimination is needed
ARMV8_SHARED_OBJECTS_REQUIRED += $(sort $(ARMV8_SHARED_OBJECTS_REQUIRED_INIT))

# LEON RT
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.c,$(DirAppObjBase)%_lrt.o,$(LEON_LIB_C_SOURCES_LRT))

LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.c,$(DirAppObjBase)%_lrt.o,$(LEON_COMPONENT_C_SOURCES_LRT))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.cpp,$(DirAppObjBase)%_lrt.o,$(LEON_COMPONENT_CPP_SOURCES_LRT))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.S,$(DirAppObjBase)%_lrt.o,$(LEON_COMPONENT_ASM_SOURCES_LRT))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.c,$(DirAppObjBase)%_lrt.o,$(LEON_DRIVER_C_SOURCES))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.S,$(DirAppObjBase)%_lrt.o,$(LEON_DRIVER_ASM_SOURCES_LRT))

ifeq ($(MV_SOC_OS_LRT), rtems)
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.c,$(DirAppObjBase)%_lrt.o,$(RTEMS_DRIVER_C_SOURCES_LRT))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.c,$(DirAppObjBase)%_lrt.o,$(LEON_COMPONENT_C_SOURCES_RTEMS_LRT))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.cpp,$(DirAppObjBase)%_lrt.o,$(LEON_COMPONENT_CPP_SOURCES_RTEMS_LRT))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.S,$(DirAppObjBase)%_lrt.o,$(LEON_COMPONENT_ASM_SOURCES_RTEMS_LRT))

else
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.c,$(DirAppObjBase)%_lrt.o,$(LEON_COMPONENT_C_SOURCES_BM_LRT))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.cpp,$(DirAppObjBase)%_lrt.o,$(LEON_COMPONENT_CPP_SOURCES_BM_LRT))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(patsubst %.S,$(DirAppObjBase)%_lrt.o,$(LEON_COMPONENT_ASM_SOURCES_BM_LRT))

endif

#and also add the projects raw data files
LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT += $(RAWDATAOBJECTFILES_LRT)

#and sort all LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT because duplicate elimination is needed
LEON_SHARED_OBJECTS_REQUIRED_LRT += $(sort $(LEON_SHARED_OBJECTS_REQUIRED_INIT_LRT))

#-----------Same for LeonNN--------------
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.c,$(DirAppObjBase)%_lnn.o,$(LEON_LIB_C_SOURCES_LNN))

LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.c,$(DirAppObjBase)%_lnn.o,$(LEON_COMPONENT_C_SOURCES_LNN))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.cpp,$(DirAppObjBase)%_lnn.o,$(LEON_COMPONENT_CPP_SOURCES_LNN))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.S,$(DirAppObjBase)%_lnn.o,$(LEON_COMPONENT_ASM_SOURCES_LNN))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.c,$(DirAppObjBase)%_lnn.o,$(LEON_DRIVER_C_SOURCES))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.S,$(DirAppObjBase)%_lnn.o,$(LEON_DRIVER_ASM_SOURCES_LNN))

ifeq ($(MV_SOC_OS_LNN), rtems)
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.c,$(DirAppObjBase)%_lnn.o,$(RTEMS_DRIVER_C_SOURCES_LNN))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.c,$(DirAppObjBase)%_lnn.o,$(LEON_COMPONENT_C_SOURCES_RTEMS_LNN))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.cpp,$(DirAppObjBase)%_lnn.o,$(LEON_COMPONENT_CPP_SOURCES_RTEMS_LNN))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.S,$(DirAppObjBase)%_lnn.o,$(LEON_COMPONENT_ASM_SOURCES_RTEMS_LNN))

else
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.c,$(DirAppObjBase)%_lnn.o,$(LEON_COMPONENT_C_SOURCES_BM_LNN))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.cpp,$(DirAppObjBase)%_lnn.o,$(LEON_COMPONENT_CPP_SOURCES_BM_LNN))
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(patsubst %.S,$(DirAppObjBase)%_lnn.o,$(LEON_COMPONENT_ASM_SOURCES_BM_LNN))

endif

#and also add the projects raw data files
LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN += $(RAWDATAOBJECTFILES_LNN)

#and sort all LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN because duplicate elimination is needed
LEON_SHARED_OBJECTS_REQUIRED_LNN += $(sort $(LEON_SHARED_OBJECTS_REQUIRED_INIT_LNN))
#-----------LeonNN end--------------

LEON_APP_OBJECTS_REQUIRED   += $(patsubst %.c,$(DirAppObjBase)%.o,$(LEON_APP_C_SOURCES))
LEON_APP_OBJECTS_REQUIRED   += $(patsubst %.cpp,$(DirAppObjBase)%.o,$(LEON_APP_CPP_SOURCES))
LEON_APP_OBJECTS_REQUIRED   += $(patsubst %.S,$(DirAppObjBase)%.o,$(LEON_APP_ASM_SOURCES))

ARMV8_APP_OBJECTS_REQUIRED   += $(patsubst %.c,$(DirAppObjBase)%.o,$(ARMV8_APP_C_SOURCES))
ARMV8_APP_OBJECTS_REQUIRED   += $(patsubst %.c,$(DirAppObjBase)%.o,$(ARMV8_APP_ASM_SOURCES))

ALL_SHAVE_APPS += $(foreach idx,$(SHAVES_IDX),$(SHAVE$(idx)_APPS))
ALL_SHAVE_NN_APPS += $(foreach idx_nn,$(SHAVES_NN_IDX),$(SHAVE$(idx_nn)_NN_APPS))
ifneq ($(strip $(shaveXApps)),)
#Add all shave apps coming from the apps loading their data sections dynamically
ALL_SHAVE_DYN_APPS := $(foreach group,$(MV_SHAVE_GROUPS),$(addsuffix _shvXdata_sym.o,$(MV_$(group)_APPS)))
ALL_SHAVE_DYN_APPS += $(foreach group,$(MV_SHAVE_GROUPS),$(addsuffix _shvXdata.o,$(MV_$(group)_APPS)))
ALL_SHAVE_DYN_APPS += $(foreach group,$(MV_SHAVE_GROUPS),$(addsuffix .shvXlib$(group),$(MV_$(group)_APPS)))
ALL_SHAVE_DYN_APPS += $(foreach group,$(MV_SHAVE_GROUPS),$($(group)).libgroup)
ALL_SHAVE_DYN_APPS += $(foreach group,$(MV_SHAVE_GROUPS),$($(group))_shvZdata.o)
#ALL_SHAVE_DYN_APPS += $(foreach group,$(MV_SHAVE_GROUPS),$($(group))_shvZdata_sym.o)
ALL_SHAVE_DYN_APPS += $(foreach app,$(shaveXApps),$(DirAppObjBase)$(lastword $(subst /, ,$(app)))_dyncontext.o)
#And include the Global dyncontext array object
ALL_SHAVE_DYN_APPS += $(DirAppObjBase)dynContextMaster.o
endif

###################################################################
# Leon Libraries declarations
###################################################################

# LEON OS
# RTEMS USB driver
ifeq ($(MV_SOC_OS), rtems)
  RTEMS_USB_LIBS  += $(patsubst %,$(RTEMS_USB_LIB_PATH)/%.a,$(MV_USB_COMPONENTS))
  DefaultSparcRTEMSLibs += $(wildcard $(RTEMS_USB_LIBS))
endif

# LEON RT
# RTEMS USB driver
ifeq ($(MV_SOC_OS_LRT), rtems)
  RTEMS_USB_LIBS_LRT  += $(patsubst %,$(RTEMS_USB_LIB_PATH_LRT)/%.a,$(MV_USB_COMPONENTS_LRT))
  DefaultSparcRTEMSLibsRT += $(wildcard $(RTEMS_USB_LIBS_LRT))
endif


###################################################################
# Dependencies
###################################################################

ifeq ($(MAKEDEPEND),yes)
  LEON_APP_DEPS  += $(patsubst %.o,$(DirAppObjBase)%.d,$(filter %.o,$(LEON_SHARED_OBJECTS_REQUIRED) $(LEON_APP_OBJECTS_REQUIRED)))
  ARMV8_APP_DEPS += $(patsubst %.o,$(DirAppObjBase)%.d,$(filter %.o,$(ARMV8_SHARED_OBJECTS_REQUIRED) $(ARMV8_APP_OBJECTS_REQUIRED)))
  LEON_RT_DEPS   += $(patsubst %.o,$(DirAppObjBase)%.d,$(filter %.o,$(LEON_SHARED_OBJECTS_REQUIRED_LRT) $(LEON_RT_APP_OBJS)))
  LEON_NN_DEPS   += $(patsubst %.o,$(DirAppObjBase)%.d,$(filter %.o,$(LEON_SHARED_OBJECTS_REQUIRED_LNN)))
  SHAVE_APP_DEPS += $(patsubst %_shave.o,$(DirAppObjBase)%.d,$(filter %_shave.o,$(SHAVE_COMPONENT_OBJS)))
  SHAVE_APP_DEPS += $(patsubst %_shave.o,$(DirAppObjBase)%.d,$(filter %_shave.o,$(SH_SWCOMMON_OBJS)))
  SHAVE_APP_DEPS += $(patsubst %.c,$(DirAppObjBase)%.d,$(filter %.c,$(wildcard $(DirAppRoot)/shave/*.c)))
  SHAVE_APP_DEPS += $(patsubst %.c,$(DirAppObjBase)%.d,$(filter %.c,$(wildcard $(DirAppRoot)/shave/*/*.c)))
  SHAVE_APP_DEPS += $(patsubst %.cpp,$(DirAppObjBase)%.d,$(filter %.cpp,$(wildcard $(DirAppRoot)/shave/*.cpp)))
  SHAVE_APP_DEPS += $(patsubst %.cpp,$(DirAppObjBase)%.d,$(filter %.cpp,$(wildcard $(DirAppRoot)/shave/*/*.cpp)))
endif

ListOfLinkerScriptSrcFiles = $(notdir $(wildcard $(DirLDScrCommon)/*.src.ld))
LinkSrcLdFilesList =  $(patsubst %.src.ld, %, $(ListOfLinkerScriptSrcFiles))

