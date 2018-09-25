#Activate the flag that disables the default debug mode build because of larger code sections. Code section is larger on ma2450
BUILD_WITH_CODE_SIZE_OPTIMIZATION=yes
MV_SOC_PLATFORM  = myriad2
MV_SOC_REV ?= ma2450
SHAVE_COMPONENTS = no
MV_COMMON_BASE  ?= ../../../../../../common
APPNAME = ISPPipes
LinkerScript = $(MV_COMMON_BASE)/../examples/Sipp/SippFw/common/config/custom.ldscript

DirAppRoot = ../..
DirAppOutput = ./output

LEON_STARTUP_MASK  ?=LRT
MV_DEFAULT_START_PROC_ID=LRT

ComponentList_LOS += PipePrint
ComponentList_LRT += PipePrint

#########################################################################################
#Use asm code
#MVCCOPT += -D'SIPP_USE_MVCV'

#########################################################################################
# Include SIPP specifics
include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk
#########################################################################################

#Sipp options
CCOPT   += -U SIPP_CMX_POOL_SZ
CCOPT   += -DSIPP_CMX_POOL_SZ=80000
CCOPT   += -DSIPP_NO_PRINTF
CCOPT   += -DRT_DBG
CCOPT   += -DSIPP_ADD_OPIPE_RT_CHECKS

#The binary elf for Shave:
svuSippImg = $(DirAppOutput)/svuSippImg
RAWDATAOBJECTFILES += $(svuSippImg)_sym.o
RAWDATAOBJECTFILES += $(svuSippImg)Map.o

#Append SippSvuObj with project specific Sipp-Filters

#########################################################################################
# Include the top makefile
PROJECTCLEAN += $(SippSvuObj)
include $(MV_COMMON_BASE)/generic.mk
#########################################################################################

$(svuSippImg).mvlib : $(SippSvuObj) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(SippSvuObj) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

$(svuSippImg)_sym.o : $(svuSippImg).shvdcomplete
	$(ECHO) $(OBJCOPY) --prefix-symbols=SS_ --extract-symbol $< $@

$(svuSippImg)Map.o : $(svuSippImg).shvdlib
			@mkdir -p $(dir $@)
						$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
												--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=mbinImgSipp \
																		-O elf32-littlesparc -B sparc $< $@

TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450, MOVISIM_MA2450"
TEST_TYPE := "AUTO"
#Mark test as running long
TEST_TAGS := "MA2150, SIPP, MA2450, LONG_RUN_TEST, MOVISIM_LONG_RUN_TEST_MA2450, MOVISIM_LONGER_RUN_TEST_MA2150"
