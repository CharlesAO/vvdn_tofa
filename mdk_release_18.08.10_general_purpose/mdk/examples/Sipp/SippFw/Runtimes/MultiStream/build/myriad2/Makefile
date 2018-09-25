#Activate the flag that disables the default debug mode build because of larger code sections. Code section is larger on ma2450
BUILD_WITH_CODE_SIZE_OPTIMIZATION=yes
MV_SOC_PLATFORM  = myriad2
MV_SOC_REV ?= ma2450
SHAVE_COMPONENTS = no
MV_COMMON_BASE  ?= ../../../../../../../common
LinkerScript = $(MV_COMMON_BASE)/../examples/Sipp/SippFw/common/config/custom.ldscript

LEON_STARTUP_MASK ?= LRT

ComponentList_LOS += PipePrint
ComponentList_LRT += PipePrint

DirAppRoot = ../..
DirAppOutput = ./output

#########################################################################################
# Include SIPP specifics
include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk
#########################################################################################

# Override default Sipp options
CCOPT   += -U SIPP_CMX_POOL_SZ -D SIPP_NO_PRINTF
CCOPT   += -DSIPP_CMX_POOL_SZ=80000
CCOPT   += -DSIPP_SCRT_ENABLE_OPIPE

#The binary elf for Shave:
svuSippImg = $(DirAppOutput)/svuSippImg
RAWDATAOBJECTFILES += $(svuSippImg)_sym.o

#########################################################################################
# Include the top makefile
PROJECTCLEAN += $(SippSvuObj)
include $(MV_COMMON_BASE)/generic.mk
#########################################################################################

$(svuSippImg).mvlib : $(SippSvuObj)
	@mkdir -p $(dir $@)
	$(ECHO) $(LD) $(MVLIBOPT) $(SippSvuObj) $(CompilerANSILibs) -o $@

$(svuSippImg)_sym.o : $(svuSippImg).shvdcomplete
	$(ECHO) $(OBJCOPY) --prefix-symbols=SS_ --extract-symbol $< $@

$(DirAppObjBase)$(DirAppRoot)/leon/appMemMapM2.o : $(svuSippImg).shvdlib

#########################################################################################
# Test specifics
#########################################################################################

TEST_HW_PLATFORM := "MV0212_MA2450, MOVISIM_MA2450"
TEST_TYPE := "AUTO"
TEST_TAGS := "SIPP, MA2450, LONG_RUN_TEST, MOVISIM_LONG_RUN_TEST_MA2450"
