MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV ?= ma2450
SHAVE_COMPONENTS = no
MV_COMMON_BASE  ?= ../../../../../../common
ComponentList := UnitTestVcs VcsHooks PipePrint

DirAppRoot = ../..
DirAppOutput = ./output

CCOPT       += -DSIPP_NO_PRINTF
CCOPT_LRT   += -DSIPP_NO_PRINTF
CCOPT       += -D'CMX_DATA=__attribute__((section(".cmx.data")))'

#########################################################################################
# Include SIPP specifics
include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk
#########################################################################################

#The binary elf for Shave:
svuSippImg = $(DirAppOutput)/svuSippImg
RAWDATAOBJECTFILES += $(svuSippImg)_sym.o
RAWDATAOBJECTFILES += $(svuSippImg)Map.o

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

TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE := "AUTO"
TEST_TAGS := "MA2150, SIPP, MA2450, LONG_RUN_TEST"
