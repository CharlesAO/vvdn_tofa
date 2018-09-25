

# Verbose or not, common out for more verbosity
ECHO ?= @

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_REV ?= ma2450
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif

MV_SOC_PLATFORM ?= myriad2

RAWDATAOBJECTFILES += $(DirAppObjDir)/testframe1.o
RAWDATAOBJECTFILES += $(DirAppObjDir)/testframe2.o
RAWDATAOBJECTFILES += $(DirAppObjDir)/testframe3.o
RAWDATAOBJECTFILES += $(DirAppObjDir)/testframe4.o

MV_COMMON_BASE  ?= ../../../../common
DirAppRoot = ./
DirAppOutput = ./output

MV_SOC_OS = rtems

# ------------------------------[ Components used ]--------------------------------#
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no
ComponentList_LOS := PipePrint FatalExtension

# Use this makefile variable to specify the USB device class protos to be included into the build
# Available USB device protos: protohid protomsc protondfu protovideo protovsc2
MV_USB_PROTOS = protovideo
# Uncomment the following line to use the debug build of USB libraries
#RTEMS_USB_LIB_BUILD_TYPE = debug

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
# MV_TOOLS_VERSION ?=

# Creating define for the pre-compiler to know what dev board has been selected
CCOPT     += -D$(EVM_BOARD_USED)

# Include the top makefile
include $(MV_COMMON_BASE)/generic.mk

MY_RESOURCE_1 = ../../../../resources/f1_2104x1560_I422.yuv
MY_RESOURCE_2 = ../../../../resources/f2_2104x1560_I422.yuv
MY_RESOURCE_3 = ../../../../resources/f1_640x480_I422.yuv
MY_RESOURCE_4 = ../../../../resources/f2_640x480_I422.yuv

$(DirAppObjDir)/testframe1.o: $(MY_RESOURCE_1) Makefile
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr_direct.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=inputFrame1 \
	-O elf32-sparc -B sparc $< $@

$(DirAppObjDir)/testframe2.o: $(MY_RESOURCE_2) Makefile
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr_direct.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=inputFrame2 \
	-O elf32-sparc -B sparc $< $@

$(DirAppObjDir)/testframe3.o: $(MY_RESOURCE_3) Makefile
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr_direct.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=inputFrame3 \
	-O elf32-sparc -B sparc $< $@

$(DirAppObjDir)/testframe4.o: $(MY_RESOURCE_4) Makefile
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr_direct.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=inputFrame4 \
	-O elf32-sparc -B sparc $< $@

TEST_TAGS:= "MA2150, MA2450"

