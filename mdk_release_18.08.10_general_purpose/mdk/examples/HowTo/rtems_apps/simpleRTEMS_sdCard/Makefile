# ------------------------------[ General Settings ]--------------------------------------#
MV_SOC_REV ?= ma2450

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

# Select LOS component list
ComponentList_LOS += PipePrint FatalExtension

# Specific RTEMS Build
#MV_RTEMS_LOS_LIB = ./lib/

# Selects the default startup processor (options LOS or LRT) (When not specified Leon OS is the default)
#LEON_STARTUP_MASK   ?=LOS

# Optionally disable LEON Caches (Not advised!!)
#DISABLE_LEON_ICACHE =yes
#DISABLE_LEON_DCACHE =yes

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE   ?= ../../../../common

# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to revision defined in mdk/common/tools_settings.mk
# Warning: This is really not recommended
#MV_TOOLS_VERSION =

# ------------------------------[ Build overrides ]--------------------------------#
# Note: If your project needs a custom linker script create a file called custom.ldscript
# in the project config folder. This will be automatically picked up.

# Example of changing the leon Defines
#CCOPT += -DDISABLE_LEON_DCACHE
#CCOPT += -DISABLE_LEON_ICACHE


# ------------------------------[ Components used ]--------------------------------#

#-------------------[ Define the sources and objects for each Shave APP ]---------#

#-----------------------[ Shave Application Assignment ]------------------#

#--------------------------[ Include common makefile rules ]--------------------------

include $(MV_COMMON_BASE)/generic.mk

#--------------------------[ Rules to build Shave Libraries ]--------------------------
TEST_TAGS        := "MA2150, MA2450"
