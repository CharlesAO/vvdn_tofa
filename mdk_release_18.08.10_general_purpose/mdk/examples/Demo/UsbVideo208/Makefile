MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

#The Movidius dev kits are 2: MV0212 equipped with MA2450 and MV0182 equipped with
#other revisions(MA2150)
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif

SHAVE_COMPONENTS = no
MV_COMMON_BASE  ?= ../../../common
LEON_RT_BUILD   ?= yes
ComponentList_LOS   := LeonIPC PipePrint FatalExtension

ifeq ($(EVM_BOARD_USED),MV0182)
 ComponentList_LRT  = Board182
else
 ComponentList_LRT = MV0212
endif

ComponentList_LRT   += UnitTestVcs VcsHooks CameraModules CameraModules/imx208 LcdGeneric CamGeneric LeonIPC PipePrint
MVSIMOPT        += -darw

DirAppRoot = .
DirAppOutput = ./output

MV_SOC_OS = rtems


# Use this makefile variable to specify the USB device class protos to be included into the build
# Available USB device protos: protohid protomsc protondfu protovideo protovsc2
MV_USB_PROTOS = protovideo
# Uncomment the following line to use the debug build of USB libraries
#RTEMS_USB_LIB_BUILD_TYPE = debug

# update for LCD ISR routing
CCOPT_LRT   += -DUse_LcdGeneric_on_LRT

CCOPT        += -I$(SIPP_DIR)
CCOPT        += -I$(SIPP_DIR)/include

#########################################################################################
#Sipp options
CCOPT       += -DSIPP_NO_PRINTF
CCOPT_LRT   += -DSIPP_NO_PRINTF

MVASMOPT    += -a -noSlotAllPromo

#CCOPT += -DDISABLE_LEON_ICACHE -DDISABLE_LEON_DCACHE
#########################################################################################
# Include SIPP specifics
include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk
#########################################################################################
CCOPT_LRT    += -D'SIPP_NO_PRINTF'
CCOPT_LRT    += -U'SIPP_CMX_POOL_SZ'
CCOPT_LRT    += -D'SIPP_CMX_POOL_SZ=84000'
CCOPT_LRT    += -D'CMX_DATA=__attribute__((section(".cmx.data")))'
CCOPT_LRT    += -DDDR_SIPP_DATA

#Use asm code
MVCCOPT      += -D'SIPP_USE_MVCV'
MVCCOPT_LRT  += -D'SIPP_USE_MVCV'
CCOPT += -DNO_PRINT

#The binary elf for Shave:
svuSippImg = $(DirAppOutput)/svuSippImg
RAWDATAOBJECTFILES += $(svuSippImg)_sym.o
RAWDATAOBJECTFILES += $(svuSippImg)Map.o

#########################################################################################
# Include the top makefile
PROJECTCLEAN += $(SippSvuObj)
include $(MV_COMMON_BASE)/generic.mk

CCOPT_LRT += -U'DEFAULT_HEAP_SIZE'
CCOPT     += -U'DEFAULT_HEAP_SIZE'

CCOPT_LRT += -D'DEFAULT_HEAP_SIZE=4096'
CCOPT     += -D'DEFAULT_HEAP_SIZE=4096'

CCOPT_LRT += -D$(EVM_BOARD_USED)
CCOPT     += -D$(EVM_BOARD_USED)

#########################################################################################

$(svuSippImg).mvlib : $(SippSvuObj)
	@mkdir -p $(DirAppOutput)
	$(ECHO) $(LD)  $(MVLIBOPT) $(SippSvuObj) $(CompilerANSILibs) -o $@

$(svuSippImg)_sym.o : $(svuSippImg).shvdcomplete
	$(ECHO) $(OBJCOPY) --prefix-symbols=lrt_SS_ --extract-symbol $< $@

$(svuSippImg)Map.o : $(svuSippImg).shvdlib
			@mkdir -p $(dir $@)
			$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
				--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=lrt_mbinImgSipp \
				-O elf32-littlesparc -B sparc $< $@

TEST_TAGS:= "MA2150, MA2450"
