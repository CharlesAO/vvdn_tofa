# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
#ShaveSymObject   = $(DirAppObjDir)/$(APPNAME).sym.o

MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450
# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

#Component lists:
ComponentList_LOS += PipePrint FatalExtension
# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION ?=

# Verbose or not, uncomment for more verbosity
#ECHO ?=

RAWDATAOBJECTFILES += $(DirAppObjDir)/testframe.o

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options
MY_RESOURCE = $(MV_EXTRA_DATA)/DunLoghaire_80x60.yuv

# Add to MVASM include path
$(DirAppObjDir)/testframe.o: $(MY_RESOURCE) Makefile
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=inputFrame \
	-O elf32-sparc -B sparc $< $@

# Extra app related options

# Testing related options
TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150,TCL_MA2150, MA2450"
