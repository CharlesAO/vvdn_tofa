MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

#The Movidius dev kits are 2: MV0212 equipped with MA2450 and MV0182 equipped with
#other revisions(MA2150)
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif
# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
APPNAME = DisparityMap

SHAVE_COMPONENTS = yes
MV_COMMON_BASE	 ?= ../../../../common
LEON_RT_BUILD   ?= yes
DirAppOutput = ./output
BICUBIC_BASE = $(MV_COMMON_BASE)/components/bicubicWarp
ComponentList_LOS  = LeonIPC PipePrint FatalExtension

ifeq ($(EVM_BOARD_USED),MV0182)
 ComponentList_LRT  = Board182
else
 ComponentList_LRT = MV0212
endif

ComponentList_LRT += CameraModules CameraModules/imx208 LcdGeneric CamGeneric bicubicWarp LeonIPC PipePrint
ComponentList_SVE = kernelLib/MvCV/include kernelLib/MvCV/kernels/censusTransform5x5 kernelLib/MvCV/kernels/censusMatching64 kernelLib/MvCV/kernels/censusMin64
ComponentList_SVE += kernelLib/MvCV/kernels/matrixInv3x3_fp32 kernelLib/MvCV/kernels/convertYUV400ToYUV422
#-------------------------------[ Settings ]---------------------------------------#

MV_SOC_OS = rtems


MV_USB_PROTOS = protovideo

# Uncomment the following line to use the debug build of USB libraries
#RTEMS_USB_LIB_BUILD_TYPE = debug

# update for LCD ISR routing
CCOPT        += -I$(SIPP_DIR)
CCOPT        += -I$(SIPP_DIR)/include

# ---------------------------- [ DisparityMap ] ---------------------------------- #

SHAVE_CPP_SOURCES_DisparityMap += $(wildcard $(DirAppRoot)/shave/*.cpp)
SHAVE_GENASMS_DisparityMap = $(patsubst %.cpp, $(DirAppObjBase)%.asmgen, $(SHAVE_CPP_SOURCES_DisparityMap))
SHAVE_DisparityMap_OBJS = $(patsubst $(DirAppObjBase)%.asmgen, $(DirAppObjBase)%_shave.o, $(SHAVE_GENASMS_DisparityMap))

#------------------------------[ Test data ]---------------------------------------#
#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(APPNAME).mvlib
SHAVE0_APPS = $(APPNAME).shv0lib
SHAVE1_APPS = $(APPNAME).shv1lib
SHAVE2_APPS = $(APPNAME).shv2lib
SHAVE3_APPS = $(APPNAME).shv3lib

# Verbose or not, uncomment for more verbosity
#ECHO=

PROJECTCLEAN += $(SHAVE_DisparityMap_OBJS) $(SHAVE_GENASMS_DisparityMap)
PROJECTINTERM += $(SHAVE_GENASMS_DisparityMap)

LEON_COMPONENT_HEADERS_PATHS += $(wildcard $(DirAppRoot)/shave/)
SH_COMPONENTS_HEADERS_PATHS += $(wildcard $(DirAppRoot)/shave/)

CCOPT       += -DSIPP_NO_PRINTF
CCOPT_LRT   += -DSIPP_NO_PRINTF
MVASMOPT    += -a -noSlotAllPromo

# Include SIPP specifics
include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk

CCOPT_LRT	+= -U'SIPP_CMX_POOL_SZ'
CCOPT_LRT   += -D'SIPP_CMX_POOL_SZ=84000'
MVCCOPT     += -D'SIPP_USE_MVCV'
MVCCOPT_LRT += -D'SIPP_USE_MVCV'

#The binary elf for Shave:
svuSippImg = $(DirAppOutput)/svuSippImg
RAWDATAOBJECTFILES += $(svuSippImg)_sym.o
RAWDATAOBJECTFILES += $(svuSippImg)Map.o

# Include the generic Makefile
LinkerScript = $(DirAppRoot)/config/custom.ldscript
include $(MV_COMMON_BASE)/generic.mk

CCOPT_LRT += -U'DEFAULT_HEAP_SIZE'
CCOPT     += -U'DEFAULT_HEAP_SIZE'

CCOPT_LRT += -D'DEFAULT_HEAP_SIZE=4096'
CCOPT     += -D'DEFAULT_HEAP_SIZE=4096'

CCOPT_LRT += -D$(EVM_BOARD_USED)
CCOPT     += -D$(EVM_BOARD_USED)

MVASMOPT := $(filter-out -noSPrefixing,$(MVASMOPT))

# SIPP mvlib
$(svuSippImg).mvlib : $(SippSvuObj)
	@mkdir -p $(dir $@)
	$(ECHO) $(LD) $(MVLIBOPT) $(SippSvuObj) $(CompilerANSILibs) -o $@

$(svuSippImg)_sym.o : $(svuSippImg).shvdcomplete
	$(ECHO) $(OBJCOPY) --prefix-symbols=lrt_SS_ --extract-symbol $< $@

$(svuSippImg)Map.o : $(svuSippImg).shvdlib
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
		--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=lrt_mbinImgSipp \
		-O elf32-littlesparc -B sparc $< $@

# APP mvlib
ENTRYPOINTS += -e disparityMapInit -u disparityMapPyrInit -u disparityMapRun -u Preprocessing -u Postprocessing -u disparityMapRunPatches --gc-sections

$(APPNAME).mvlib : $(SHAVE_DisparityMap_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(SHAVE_DisparityMap_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

dbg:
	@echo $(SHAVE_DisparityMap_OBJS)

test:
	@echo $(SIPP_DIR)

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options
# PROFILING 	- Unit test on static images and profiling on Leon and Shaves
# CAMERA        - Full application on camera frames

CCOPT       += -D__LEON__

ifeq ($(RUN_MODE), PROFILING)
    CCOPT_LRT   += -DPROFILING
else ifeq ($(RUN_MODE), CAMERA)
    CCOPT_LRT   += -DCAMERA
endif

# Don't stop function inlining
MVCOMPILEINLINESTATUS =

# -------------------------------- [ Extra dependancies] ---------------------------#

# ------------------------------[ Extra Rules ]-------------------------------------------#
TEST_TAGS:= "MA2150, MA2450"
