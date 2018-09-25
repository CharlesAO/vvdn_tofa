ComponentList_$(VTRACK_LEON_PREFIX) += flic/core flic/types visionBaseFlicPlg flic/plugins/plgRotationMat
ComponentList += PipePrint shaveFifoComm kernelLib/MvCV

ifeq ($(MV_SOC_REV), ma2480)
    MV_SOC_TYPE=ma2x8x
else
    MV_SOC_TYPE=ma2x5x
endif

include $(MV_COMMON_BASE)/components/MEST/mest.mk
ifeq ($(MV_SOC_REV), ma2480)
	ComponentList += $(MEST_COMP_API)
endif

VTRACK_DIR = $(MV_COMMON_BASE)/components/vTrack/source
SH_COMPONENTS_HEADERS_PATHS += $(VTRACK_DIR)/common
SH_COMPONENTS_HEADERS_PATHS += $(VTRACK_DIR)/api
SH_COMPONENTS_HEADERS_PATHS += $(VTRACK_DIR)/apiLite
ifeq ($(MV_SOC_TYPE), ma2x8x)
SH_COMPONENTS_HEADERS_PATHS +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/ppm/arch/$(MV_SOC_TYPE)
endif
ifeq ($(MV_SOC_TYPE), ma2x5x)
SH_COMPONENTS_HEADERS_PATHS +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_corners/arch/$(MV_SOC_TYPE)
endif

LEON_COMPONENT_HEADERS_PATHS += $(VTRACK_DIR)/api
LEON_COMPONENT_HEADERS_PATHS += $(VTRACK_DIR)/apiLite
LEON_COMPONENT_HEADERS_PATHS += $(VTRACK_DIR)/common
LEON_COMPONENT_HEADERS_PATHS += $(VTRACK_DIR)/leon
SH_COMPONENTS_HEADERS_PATHS += $(VTRACK_DIR)/pixelPipe/shvDynApps/ppm
SH_COMPONENTS_HEADERS_PATHS += $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_corners
SH_COMPONENTS_HEADERS_PATHS += $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_gauss
ComponentList += vTrack/source/pixelPipe vTrack/source/opticalFlow vTrack/source/featurePipe vTrack/source/vpipe

#fm sources
fmApp = $(DirAppOutput)/fm

SHAVE_CPP_SOURCES_fm =  $(VTRACK_DIR)/featurePipe/shvDynApps/fm/vTrackHarrisResponse.cpp
SHAVE_CPP_SOURCES_fm +=  $(VTRACK_DIR)/featurePipe/shvDynApps/fm/featureMaintenance.cpp

#Generating list of required generated assembly files for the Disp2Depth app on shave
SHAVE_GENASMS_fm = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_fm))

#Generating required objects list from sources
SHAVE_fm_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_fm)) \
                    $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_fm))
PROJECTCLEAN += $(SHAVE_GENASMS_fm) $(SHAVE_fm_OBJS)

MV_DEFAULT_SHAVE_GROUP_APPS += $(fmApp)

#of sources
ofApp = $(DirAppOutput)/of

SHAVE_CPP_SOURCES_of =  $(VTRACK_DIR)/opticalFlow/shvDynApps/of/opticalFlow.cpp

#Generating list of required generated assembly files for the Disp2Depth app on shave
SHAVE_GENASMS_of = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_of))

#Generating required objects list from sources
SHAVE_of_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_of)) \
                    $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_of))
PROJECTCLEAN += $(SHAVE_GENASMS_of) $(SHAVE_of_OBJS)

MV_DEFAULT_SHAVE_GROUP_APPS += $(ofApp)

#ppm sources
ppmApp = $(DirAppOutput)/ppm

SHAVE_CPP_SOURCES_ppm =  $(VTRACK_DIR)/pixelPipe/shvDynApps/ppm/cornerFilter.cpp
SHAVE_CPP_SOURCES_ppm +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/ppm/gaussFilter.cpp
SHAVE_CPP_SOURCES_ppm +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/ppm/pixelPipeMaster.cpp
ifeq ($(MV_SOC_TYPE), ma2x8x)
SHAVE_CPP_SOURCES_ppm +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/ppm/arch/$(MV_SOC_TYPE)/cornerHW.cpp
endif
SHAVE_CPP_SOURCES_ppm +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_corners/pixelPipeCornerHelper.cpp
ifeq ($(MV_SOC_TYPE), ma2x5x)
SHAVE_CPP_SOURCES_ppm +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_corners/arch/$(MV_SOC_TYPE)/cornerSW.cpp
SHAVE_CPP_SOURCES_ppm +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_corners/arch/$(MV_SOC_TYPE)/cornerSWFuncs.cpp
endif
SHAVE_CPP_SOURCES_ppm +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_gauss/pixelPipeGaussHelper.cpp

#Generating list of required generated assembly files for the Disp2Depth app on shave
SHAVE_GENASMS_ppm = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_ppm))

#Generating required objects list from sources
SHAVE_ppm_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_ppm)) \
                    $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_ppm))
PROJECTCLEAN += $(SHAVE_GENASMS_ppm) $(SHAVE_ppm_OBJS)

MV_DEFAULT_SHAVE_GROUP_APPS += $(ppmApp)

#pp_corners sources
pp_cornersApp = $(DirAppOutput)/pp_corners

SHAVE_CPP_SOURCES_pp_corners =  $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_corners/pixelPipeCornerHelper.cpp
ifeq ($(MV_SOC_TYPE), ma2x5x)
SHAVE_CPP_SOURCES_pp_corners +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_corners/arch/$(MV_SOC_TYPE)/cornerSW.cpp
SHAVE_CPP_SOURCES_pp_corners +=  $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_corners/arch/$(MV_SOC_TYPE)/cornerSWFuncs.cpp
endif

#Generating list of required generated assembly files for the Disp2Depth app on shave
SHAVE_GENASMS_pp_corners = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_pp_corners))

#Generating required objects list from sources
SHAVE_pp_corners_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_pp_corners)) \
                    $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_pp_corners))
PROJECTCLEAN += $(SHAVE_GENASMS_pp_corners) $(SHAVE_pp_corners_OBJS)

MV_DEFAULT_SHAVE_GROUP_APPS += $(pp_cornersApp)


#pp_gauss sources
pp_gaussApp = $(DirAppOutput)/pp_gauss

SHAVE_CPP_SOURCES_pp_gauss =  $(VTRACK_DIR)/pixelPipe/shvDynApps/pp_gauss/pixelPipeGaussHelper.cpp

#Generating list of required generated assembly files for the Disp2Depth app on shave
SHAVE_GENASMS_pp_gauss = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_pp_gauss))

#Generating required objects list from sources
SHAVE_pp_gauss_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_pp_gauss)) \
                    $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_pp_gauss))
PROJECTCLEAN += $(SHAVE_GENASMS_pp_gauss) $(SHAVE_pp_gauss_OBJS)

MV_DEFAULT_SHAVE_GROUP_APPS += $(pp_gaussApp)

ppm_MainEntry = pixelPipeMasterEntry
pp_corners_MainEntry = fifoCommSlaveRun
pp_gauss_MainEntry = fifoCommSlaveRun
of_MainEntry = opticalFlowRun
fm_MainEntry = featureMaintenance


VTRACK_DEFINES  := $(MEST_DEFINES) -D'USE_CMX_DMA_NEW_DRIVER=1'
VTRACK_INCLUDES := $(MEST_INCLUDES)

MVCCOPT   += $(VTRACK_DEFINES) $(VTRACK_INCLUDES)
MVCCPPOPT += $(VTRACK_DEFINES) $(VTRACK_INCLUDES)

ifeq ($(VTRACK_LEON_PREFIX),LOS)
	CCOPT += $(VCCOPT)
else
	CCOPT_LRT += $(VCCOPT)
endif

