
include $(MV_COMMON_BASE)/resolvetargetpath.mk

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155))
DdrInitHeaderMvcmd   ?= $(MV_COMMON_BASE)/utils/ddrInit/ddrInitHeaderForMvcmd_ma2150
DdrInitElf           ?= $(MV_COMMON_BASE)/utils/ddrInit/ddrInit_ma2150.elf
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2450 ma2455))
DdrInitHeaderMvcmd   ?= $(MV_COMMON_BASE)/utils/ddrInit/ddrInitHeaderForMvcmd_ma2450$(SOC_REV_DIR_SUFFIX)
DdrInitElf           ?= $(MV_COMMON_BASE)/utils/ddrInit/ddrInit_ma2450$(SOC_REV_DIR_SUFFIX).elf
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485))
DdrInitHeaderMvcmd   ?= $(call RESOLVE_TARGET_PATH,$(MV_COMMON_BASE)/utils/ddrInit/ddrInitHeaderForMvcmd_ma2480)
DdrInitElf           ?= $(call RESOLVE_TARGET_PATH,$(MV_COMMON_BASE)/utils/ddrInit/ddrInit_ma2480.elf)
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490 ma2495))
DdrInitHeaderMvcmd   ?= $(call RESOLVE_TARGET_PATH,$(MV_COMMON_BASE)/utils/ddrInit/ddrInitHeaderForMvcmd_ma2480)
DdrInitElf           ?= $(call RESOLVE_TARGET_PATH,$(MV_COMMON_BASE)/utils/ddrInit/ddrInit_ma2480.elf)
else
    $(error ddrInitElf and Mvcmd Header not defined for this MV_SOC_REV: $(MV_SOC_REV))
endif
