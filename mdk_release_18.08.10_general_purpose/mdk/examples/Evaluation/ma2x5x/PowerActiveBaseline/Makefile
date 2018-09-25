# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

#The Movidius dev kits are 2: MV0212 equipped with MA2450 and MV0182 equipped with
#other revisions( MA2150)
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif
# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk

# ------------------------------[ Components used ]--------------------------------#
ifeq ($(EVM_BOARD_USED),MV0182)
 ComponentList_LOS  = Board182
else
 ComponentList_LOS = MV0212
endif
ComponentList_LOS += UnitTest PipePrint

# ------------------------------[ Tools overrides ]--------------------------------#
# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

include $(MV_COMMON_BASE)/generic.mk

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options
CCOPT_LRT += -D$(EVM_BOARD_USED)
CCOPT     += -D$(EVM_BOARD_USED)

#---------------------------------------------------------------------#
#-------------------------- Jenkins options --------------------------#
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE        := "AUTO"
TEST_TAGS        := "MA2150, MA2450"
