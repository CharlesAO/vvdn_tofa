MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

SHAVE_COMPONENTS = no
MV_COMMON_BASE  ?= ../../../../../../../common
ComponentList_LOS := UnitTestVcs VcsHooks PipePrint
DirAppRoot = ../..
DirAppOutput = ./output
LinkerScript = $(MV_COMMON_BASE)/../examples/Sipp/SippFw/common/config/custom.ldscript
#MV_TOOLS_VERSION ?=
#Sipp options
#CCOPT+= -D'SIPP_NO_PRINTF'
CCOPT       += -DSIPP_NO_PRINTF
CCOPT_LRT   += -DSIPP_NO_PRINTF
MVCCOPT += -D'SIPP_USE_MVCV'
#########################################################################################
# Include SIPP specifics
include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk
#########################################################################################


#The binary elf for Shave:
svuSippImg = $(DirAppOutput)/svuSippImg
RAWDATAOBJECTFILES += $(svuSippImg)_sym.o
RAWDATAOBJECTFILES += $(svuSippImg)Map.o

#Include Sipp Filter Paths
include ../sipp/sippTutD0Model.mk

#Reference data

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

$(svuSippImg)Map.o : $(svuSippImg).shvdlib
			@mkdir -p $(dir $@)
						$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
												--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=mbinImgSipp \
																		-O elf32-littlesparc -B sparc $< $@

TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450, MOVISIM_MA2450, MOVISIM_MA2150"
TEST_TYPE := "AUTO"
#Mark test as running long
TEST_TAGS := "MA2150, SIPP, MA2450, LONG_RUN_TEST, MOVISIM_LONG_RUN_TEST_MA2450, MOVISIM_LONG_RUN_TEST_MA2150"
