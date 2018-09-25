#Activate the flag that disables the default debug mode build because of larger code sections. Code section is larger on ma2150 ma2450
BUILD_WITH_CODE_SIZE_OPTIMIZATION=yes
MV_SOC_PLATFORM  ?= myriad2
MV_SOC_REV       ?= ma2450
#The Movidius dev kits are 2: MV0212 equipped with MA2450 and MV0182 equipped with
#other revisions(MA2150)
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif

MV_COMMON_BASE  ?= ../../../common
LEON_RT_BUILD   ?= yes

ifeq ($(EVM_BOARD_USED),MV0182)
 ComponentList_LRT  := Board182
else
 ComponentList_LRT  := MV0212
endif

ComponentList_LRT   +=  CameraModules CameraModules/imx208 LcdGeneric CamGeneric LeonIPC PipePrint JpegM2EncoderDyn
ComponentList_LOS   +=  LeonIPC PipePrint VcsHooks FatalExtension
ComponentList_SVE   := JpegM2EncoderDyn kernelLib/MvCV/kernels/warpMeshExpand kernelLib/MvCV/include kernelLib/MvCV/kernels/warpMeshSample8bit
ComponentList_SVE   += kernelLib/MvCV/kernels/dct16bit kernelLib/MvCV/kernels/jpegGetBlockY420 kernelLib/MvCV/kernels/huffman_encode
SHAVE_COMPONENTS=yes

JpegEncoderShaveDynlib_HEAPSIZE  = 0
JpegEncoderShaveDynlib_HEAP_INIT = DYNCONTEXT_HEAP_NOINIT


MVSIMOPT        += -darw

DirAppRoot = .
DirAppOutput = ./output

MV_SOC_OS = rtems

imageWarpDynlib_HEAPSIZE  = 0
imageWarpDynlib_HEAP_INIT = DYNCONTEXT_HEAP_NOINIT

JpegEncoderShaveDynlib_HEAPSIZE  = 0
JpegEncoderShaveDynlib_HEAP_INIT = DYNCONTEXT_HEAP_NOINIT

# update for LCD ISR routing
CCOPT_LRT    += -DUse_LcdGeneric_on_LRT

CCOPT        += -I$(SIPP_DIR)
CCOPT        += -I$(SIPP_DIR)/include

#########################################################################################
#Sipp options

CCOPT       += -DSIPP_NO_PRINTF
CCOPT_LRT   += -DSIPP_NO_PRINTF

MVASMOPT    += -a -noSlotAllPromo

include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk

CCOPT_LRT    += -D'SIPP_NO_PRINTF'
CCOPT_LRT    += -U'SIPP_CMX_POOL_SZ'
CCOPT_LRT    += -D'SIPP_CMX_POOL_SZ=102400'
#Use asm code
MVCCOPT      += -D'SIPP_USE_MVCV'
CCOPT_LRT  += -D'SIPP_USE_MVCV'

#The binary elf for Shave:
svuSippImg = $(DirAppOutput)/svuSippImg
RAWDATAOBJECTFILES += $(svuSippImg)_sym.o

#Include Sipp Filter Paths
include Cam208CvHdmiModel.mk

# Include the top makefile
PROJECTCLEAN += $(SippSvuObj)
include $(MV_COMMON_BASE)/generic.mk

CCOPT_LRT += -U'DEFAULT_HEAP_SIZE'
CCOPT     += -U'DEFAULT_HEAP_SIZE'

CCOPT_LRT += -D'DEFAULT_HEAP_SIZE=34*1024'
CCOPT     += -D'DEFAULT_HEAP_SIZE=4096'

CCOPT_LRT += -D$(EVM_BOARD_USED)
CCOPT     += -D$(EVM_BOARD_USED)

#########################################################################################

$(svuSippImg).mvlib : $(SippSvuObj)
	@mkdir -p $(DirAppOutput)
	$(ECHO) $(LD)  $(MVLIBOPT) $(SippSvuObj) $(CompilerANSILibs) -o $@

$(svuSippImg)_sym.o : $(svuSippImg).shvdcomplete
	$(ECHO) $(OBJCOPY) --prefix-symbols=lrt_SS_ --extract-symbol $< $@

$(DirAppObjBase)./leon_rt/appMemMapM2_lrt.o : $(svuSippImg).shvdlib

TEST_TAGS:= "MA2150, MA2450"
