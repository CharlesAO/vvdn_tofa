# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# Verbose or not, common out for more verbosity
ECHO=@
#DISABLE_ERROR_ON_WARNINGS=yes

#-------------------------------[ MDK COMMON ]---------------------------------------#
# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE:=../../common
VTRACK_ROOT_DIR =$(MV_COMMON_BASE)/components/vTrack
MVCV_KERNELS_PATH = kernelLib/MvCV/kernels

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
#ShaveSymObject   = $(DirAppObjDir)/$(APPNAME).sym.o

DirLDScrCommon = $(MV_COMMON_BASE)/scripts/ld/$(MV_SOC_PLATFORM)memorySections
# ------------------------------[ Components used ]--------------------------------#

SHAVE_COMPONENTS = yes
ifeq ($(MV_SOC_REV), ma2480)
    LinkerScript = customMX.ldscript
    ComponentList += MV0235
    ComponentList += flic/plugins/arch/ma2x8x/plgStereoProc
    ComponentList += flic/components/Stereo
    ComponentList += HwModInt
    ComponentList += MemoryManager
    ComponentList_LOS += mvLibCamera
else
    ComponentList += MV0212
    ComponentList += imageWarpDyn stereo/sgbm stereo/cost stereo/downsample stereo
    ComponentList_SVE += $(MVCV_KERNELS_PATH)/warpMeshExpand kernelLib/MvCV/include $(MVCV_KERNELS_PATH)/warpMeshSample8bit
    ComponentList_SVE += $(MVCV_KERNELS_PATH)/pyrdown $(MVCV_KERNELS_PATH)/convert_fp16_u8 $(MVCV_KERNELS_PATH)/thresholdDisparityFp16 $(MVCV_KERNELS_PATH)/convertFp16ToQ4 
    ComponentList_SVE += $(MVCV_KERNELS_PATH)/aggregateCostSGBM64_clamp $(MVCV_KERNELS_PATH)/aggregateCostSGBM32_clamp $(MVCV_KERNELS_PATH)/aggregateFivePaths64 $(MVCV_KERNELS_PATH)/aggregateFivePaths32 $(MVCV_KERNELS_PATH)/minKernel64 $(MVCV_KERNELS_PATH)/minKernel32 $(MVCV_KERNELS_PATH)/minAggregate_line
    ComponentList_SVE += $(MVCV_KERNELS_PATH)/censusMatching64 $(MVCV_KERNELS_PATH)/censusMatching32 $(MVCV_KERNELS_PATH)/censusMatchingPyrOnePos 
    ComponentList_SVE += $(MVCV_KERNELS_PATH)/censusMinSubpixel3  $(MVCV_KERNELS_PATH)/censusMinConfidence32 $(MVCV_KERNELS_PATH)/censusMinConfidence64 $(MVCV_KERNELS_PATH)/censusTransformAverageRefMask7x7
    ComponentList_SVE += $(MVCV_KERNELS_PATH)/computeAD64 $(MVCV_KERNELS_PATH)/computeAD32 $(MVCV_KERNELS_PATH)/computeADPyrOnePos
    ComponentList_SVE += $(MVCV_KERNELS_PATH)/computeCombinedCost64 $(MVCV_KERNELS_PATH)/computeCombinedCost32 $(MVCV_KERNELS_PATH)/computeCombinedCost3
    ComponentList_SVE += $(MVCV_KERNELS_PATH)/computeADPyrOnePosWindow $(MVCV_KERNELS_PATH)/computeCombinedCost3Window $(MVCV_KERNELS_PATH)/censusMatchingPyrOnePosWindow
    ComponentList_SVE += $(MVCV_KERNELS_PATH)/censusMinConfidence64Subpixel $(MVCV_KERNELS_PATH)/censusMinSubpixel3Window $(MVCV_KERNELS_PATH)/censusMin64
    ComponentList_SVE += $(MVCV_KERNELS_PATH)/subPixelFilter  kernelLib/MvCV/include
endif

#ComponentList_LOS += flic/plugins/common/arch/ma2x8x
ComponentList += visionStereoPlg


#Don't use it because the application contains warnings
VTRACK_LEON_PREFIX=LRT

DirAppRoot = .
DirAppOutput ?= ./output

include $(MV_COMMON_BASE)/components/DataCollector/DataCollector.mk

include $(VTRACK_ROOT_DIR)/vTrackConfig.mk

#Use 30Hz streaming in case of this application
CCOPT_LRT    += -D'FRAME_RATE_DIVIDER=1'
SIPP_AEC_SLICE_NUMBER=10

#TEST_TYPE     := AUTO
#ComponentList := Board

ifeq ($(MV_SOC_REV), ma2480)
else
#---------- Dynamic data loading applications --------#

#------------------ DOWNSAMPLE APP -------------#
downsampleApp = $(MV_COMMON_BASE)/components/stereo/downsample/shave/downsampleApp

SHAVE_C_SOURCES_downsampleApp = $(wildcard $(MV_COMMON_BASE)/components/stereo/downsample/shave/*.c)
SHAVE_CPP_SOURCES_downsampleApp = $(wildcard $(MV_COMMON_BASE)/components/stereo/downsample/shave/*.cpp)
SHAVE_ASM_SOURCES_downsampleApp = $(SHAVE_ASM_SOURCES_APPNAME1)
# Generating list of required generated assembly files for the SGBMApp app on shave
SHAVE_GENASMS_downsampleApp  = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_downsampleApp))
SHAVE_GENASMS_downsampleApp += $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_downsampleApp))
# Generating required objects list from sources
SHAVE_downsampleApp_OBJS  = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_downsampleApp))
SHAVE_downsampleApp_OBJS += $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_downsampleApp))
include $(MV_COMMON_BASE)/components/stereo/sgbm/sgbm.mk
endif
# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to Latest
#MV_TOOLS_VERSION = Latest



#########################################################################################

#-------------------------------[ MODULES ]---------------------------------------#
ifeq ($(MV_SOC_REV), ma2480)
AppModuleList += ma2x8x
else
AppModuleList += ma2x5x
endif

LEON_COMPONENT_PATHS         += $(patsubst %,appConfig/%/shared,$(AppModuleList))
LEON_COMPONENT_HEADERS_PATHS += $(patsubst %,appConfig/%/shared,$(AppModuleList))
LEON_COMPONENT_PATHS_LOS         += $(patsubst %,appConfig/%/leon,$(AppModuleList))
LEON_COMPONENT_HEADERS_PATHS_LOS += $(patsubst %,appConfig/%/leon,$(AppModuleList))
ifeq ($(AppModuleList), ma2x5x)
LEON_COMPONENT_PATHS_LRT         += $(patsubst %,appConfig/%/leon,$(AppModuleList))
LEON_COMPONENT_HEADERS_PATHS_LRT += $(patsubst %,appConfig/%/leon,$(AppModuleList))
else
LEON_COMPONENT_PATHS_LRT         += $(patsubst %,appConfig/%/leon_rt,$(AppModuleList))
LEON_COMPONENT_HEADERS_PATHS_LRT += $(patsubst %,appConfig/%/leon_rt,$(AppModuleList))
endif
CCOPT_EXTRA           += -D'VCSHOOKS_USE_SOFTWARE=1'

CCOPT_LRT += -U SIPP_CMX_POOL_SZ -DSIPP_CMX_POOL_SZ=256*1024
CCOPT_LRT += -DSIPP_AMC_HIPRIO_AT_CMX
CCOPT_LRT += -DSIPP_SCRT_ENABLE_OPIPE
CCOPT_LRT += -DSIPP_USE_OPIPE_RT
CCOPT_LRT += -D'SIPP_IRQ_LEVEL = 3'
#CCOPT += -D'MVLIBCAMERA_DEBUG'
#CCOPT += -D'OSDRVIMX378_DEBUG'
#CCOPT += -D'OSDRVOV9282_DEBUG'

#The binary elf for Shave:
svuSippSterImg = $(DirAppOutput)/svuSippSterImg
RAWDATAOBJECTFILES += $(svuSippSterImg)_sym.o
RAWDATAOBJECTFILES += $(svuSippSterImg)Map.o

ifeq ($(MV_SOC_REV), ma2480)
SippSvuObj  = $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x8x/src/shave/sippShave_shave.o
SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x8x/src/shave/sippSvuSync_shave.o
SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x8x/src/shave/sippShvDbg_shave.o

# Choose c- based protyping versions
ifneq ($(findstring SIPP_USE_C_PTR_GET_FUNCS,$(MVCCOPT)),)
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x8x/src/shave/sippIoPtrs_shave.o
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x8x/src/shave/sippHpad_shave.o
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x8x/src/shave/sippFilterAccesors_shave.o
else
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x8x/src/shave/sippHpadOpt_shave.o
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x8x/src/shave/sippFilterAccesorsOpt_shave.o
   SippSvuObj += $(DirAppObjBase)$(SIPP_DIR)/arch/ma2x8x/src/shave/sippIoPtrsOpt_shave.o
endif

SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/disp2depth/shave/svudisp2depth.c)
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/disp2depth/shave/disp2depth_exec.c)
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/subpixelFilter/shave/svuSubpixelFilter.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o, $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/subPixelFilter/arch/ma2x8x/shave/src/subPixelFilter.asm)

endif


PROJECTCLEAN += $(SippSvuObj)

MV_DEFAULT_SHAVE_GROUP_APPS += $(SGBMApp) $(downsampleApp) $(costApp)

include $(MV_COMMON_BASE)/generic.mk
include $(VTRACK_ROOT_DIR)/postGenericvTrackConfig.mk

include $(MV_COMMON_BASE)/components/DataCollector/DataCollector.mk

ifeq ($(MV_SOC_REV), ma2480)
else
include $(MV_COMMON_BASE)/components/stereo/sgbm/sgbmPart2.mk
downsampleApp_MainEntry = downsampleImage
ENTRYPOINTS_downsample = -e $(downsampleApp_MainEntry) -u __init_array_start -u __fini_array_start -u __init_array_end -u __fini_array_end --gc-sections
$(downsampleApp).mvlib : $(SHAVE_downsampleApp_OBJS)
	$(ECHO) $(LD) $(ENTRYPOINTS_downsample) $(MVLIBOPT) $(SHAVE_downsampleApp_OBJS) -o $@
endif


$(svuSippSterImg).mvlib : $(SippSvuObj)
	@mkdir -p $(dir $@)
	$(ECHO) $(LD) $(MVLIBOPT) $(SippSvuObj) $(CompilerANSILibs) -o $@

$(svuSippSterImg)_sym.o : $(svuSippSterImg).shvdcomplete
	$(ECHO) $(OBJCOPY) --prefix-symbols=lrt_SS_ --extract-symbol $< $@

$(svuSippSterImg)Map.o : $(svuSippSterImg).shvdlib
			@mkdir -p $(dir $@)
						$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
												--redefine-sym  _binary_$(subst /,_,$(subst .,_,$<))_start=lrt_mbinImgSterSipp \
																		-O elf32-littlesparc -B sparc $< $@
# -------------------------------- [ Build Options ] ------------------------------ #
ifeq ($(MONOISP_ENABLE), yes)
CCOPT     += -DMONOISP
CCOPT_LRT += -DMONOISP
endif
CCOPT_LRT += -DPIPEPRINT_NOWAIT_DEBUG
CCOPT     += -DPIPEPRINT_NOWAIT_DEBUG
CCOPT_LRT += -D'SGBM_SOURCE_CODE_BUILD'
CCOPT     += -D'SGBM_SOURCE_CODE_BUILD'

MVCCOPT   += -DPC_VIEWER_FLAG
CCOPT_LRT     += -DPC_VIEWER_FLAG
CCOPT     += -DPC_VIEWER_FLAG

ifeq ($(DISP), 64)
    MVCCOPT   += -D"DISP=64"
    CCOPT_LRT   += -D"DISP=64"
    CCOPT   += -D"DISP=64"
    
else ifeq ($(DISP), 128)
    MVCCOPT   += -D"DISP=128"
    CCOPT_LRT   += -D"DISP=128"
    CCOPT   += -D"DISP=128"
    
endif

# App related build options

# Extra app related options
CCOPT += -DCAMERA_8BIT_CONFIG

MVASMOPT+=-a
#Application version:
MVCMDOPT = -mvcmdVersion:1.00.0
TEST_TAGS        := "MA2480"
