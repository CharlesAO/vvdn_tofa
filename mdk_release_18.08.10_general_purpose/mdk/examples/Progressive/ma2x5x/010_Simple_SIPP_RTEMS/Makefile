#Activate the flag that disables the default debug mode build because of larger code sections. Code section is larger on ma2150 ma2450
BUILD_WITH_CODE_SIZE_OPTIMIZATION=yes
MV_SOC_PLATFORM  ?= myriad2
MV_SOC_REV ?= ma2450

ComponentList_LOS += PipePrint FatalExtension UnitTestVcs VcsHooks
ComponentList_LRT += PipePrint

SHAVE_COMPONENTS = no
MV_COMMON_BASE  ?= ../../../../common

DirAppRoot = .
DirAppOutput = ./output

MV_SOC_OS = rtems

LEON_RT_BUILD = yes

CCOPT += -Wstrict-prototypes
CCOPT_LRT += -D'SIPP_NO_PRINTF'  -Wstrict-prototypes
#########################################################################################
# Include SIPP specifics
include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk
#########################################################################################

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
	$(ECHO) $(OBJCOPY) --prefix-symbols=lrt_SS_ --extract-symbol $< $@

TEST_TAGS        := "MA2150, MA2450"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450, MOVISIM_MA2150, MOVISIM_MA2450"
