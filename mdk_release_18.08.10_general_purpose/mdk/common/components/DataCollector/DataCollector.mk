ifeq ($(DATACOLLECT_MK_FIRST_PART),)
DATACOLLECT_MK_FIRST_PART:=1

ifeq (,$(MV_SOC_REV))
$(info ">> INFO << MV_SOC_REV not defined in aplication Makefile, defaulting to ma2480 !!!")
MV_SOC_REV ?= ma2480
endif
#########################################################################################
# Include SIPP specifics
ifeq ($(MV_SOC_REV), ma2480)
MV_ARCH=2x8x
else
MV_SIPP_OS_ORIG = yes
MV_ARCH=2x5x
endif

MV_SOC_OS =     rtems
MV_SOC_OS_LRT = rtems

LEON_RT_BUILD = yes
LEON_RT_LIB_NAME = $(DirAppOutput)/leonRTApp
ComponentList += flic/core flic/types 
ComponentList += visionBaseFlicPlg
ComponentList += flic/common/commonTypes
ComponentList += flic/plugins/arch/ma$(MV_ARCH)/common/commonTypes
#ComponentList += flic/plugins/common/commonTypes
ComponentList += flic/plugins/plgCamSync
ComponentList += flic/plugins/plgFramePumpCam
ComponentList += flic/plugins/plgImu
ComponentList += flic/plugins/plgRotationMat

ComponentList_LOS += SimpleAE 
ComponentList_LRT += SimpleAE
ComponentList_LOS += flic/plugins/plgXlinkConfig




DATA_COLLECTOR_PATH = $(MV_COMMON_BASE)/components/DataCollector

MDK_ROOT =  $(DATA_COLLECTOR_PATH)/../../..





$(info DATA_COLLECTOR_PATH = $(realpath $(DATA_COLLECTOR_PATH) ))
$(info MDK_ROOT = $(realpath $(MDK_ROOT) ))



USE_USB_VSC?=yes
ifeq ($(USE_USB_VSC),yes)
LEON_APP_URC_SOURCES +=  $(wildcard $(MV_COMMON_BASE)/components/USB_VSC/*leon/*.urc)
CCOPT+= -D'USE_USB_VSC'
ComponentList_LOS+=USB_VSC
MV_USB_PROTOS = protovsc2

else
LEON_APP_URC_SOURCES +=  $(wildcard $(MV_COMMON_BASE)/components/USB_CDC/*leon/*.urc)
MV_USB_PROTOS = protowmc

endif
SIPP_AEC_SLICE_NUMBER ?= 4
CUSTOM_AE_TARGET_BRIGHTNESS ?= 75

ComponentList += LeonIPC FramePump PipePrint
ComponentList_LOS +=XLink DataFlowFramework USB_CDC

#Choosing if this project has shave components or not
SHAVE_COMPONENTS = yes


include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk
CCOPT_LRT    += -U'SIPP_CMX_POOL_SZ'
CCOPT_LRT    += -D'SIPP_CMX_POOL_SZ=84000'
CCOPT_LRT    += -D'SIPP_CDMA_AGENT_NO=0'
CCOPT_LRT    += -D'DATA_COLL_NO_USB'
CCOPT_LRT    += -DSIPP_NO_PRINTF
CCOPT_LRT    += -DAE_TARGET_BRIGHTNESS=$(CUSTOM_AE_TARGET_BRIGHTNESS)
CCOPT_LRT    += -DCVRT_SLICE=$(SIPP_AEC_SLICE_NUMBER)
CCOPT_LRT    += -D'CMX_DATA=__attribute__((section(".cmx.data")))'
CCOPT_LRT    += -DSIPP_DISABLE_INTERRUPT_SETUP
CCOPT_LRT    += -D'FLIC_2LEONS'
CCOPT_LRT    += -D'DYN_IRQ_ASSIGNMENT=1'
CCOPT_LRT    += -D'FLIC_RMI_REQ_IRQ=62'
CCOPT_LRT    += -D'FLIC_RMI_ACK_IRQ=63'
CCOPT    += -D'FLIC_RMI_REQ_IRQ=62'
CCOPT    += -D'FLIC_RMI_ACK_IRQ=63'

ifeq ($(MONOISP_ENABLE), yes)
ifneq ($(MV_SOC_REV), ma2480)
$(error MONOISP not supported for non 2x8x)
endif

#GUZZI_REPO =  $(realpath ../../../../../../../../..)
GUZZI_INC_DIR =  $(realpath $(MDK_ROOT)/packages/movidius/Guzzi/arch/ma2x8x/include)
GUZZI_LIB_DIR =  $(realpath $(MDK_ROOT)/packages/movidius/Guzzi/arch/ma2x8x/lib)
$(info GUZZI_INC_DIR = $(GUZZI_INC_DIR) )
$(info GUZZI_LIB_DIR = $(GUZZI_LIB_DIR) )
output/MyriadApp.elf: $(GUZZI_LIB_DIR)/libguzzi_ma2x8x.a

ComponentList_LOS += mvLibCamera
ComponentList_LOS += DataCollector/GuzziCam



ComponentList += flic/plugins/arch/ma2x8x/common/ispCommon
ComponentList += flic/plugins/arch/ma2x8x/plgSrcMipi
ComponentList += flic/plugins/arch/ma2x8x/plgEventsRec
ComponentList += flic/plugins/arch/ma2x8x/plgSrcCtrl
ComponentList += flic/plugins/arch/ma2x8x/plgMonoIsp
ComponentList += flic/plugins/arch/ma2x8x/plgIspCtrl
ComponentList += flic/pipes/arch/ma2x8x/common
ComponentList += sipp/arch/ma2x8x/pipes/common
ComponentList += sipp/arch/ma2x8x/pipes/monoIsp
######### Guzzi
#CCOPT += -DDEBUG
#CCOPT += -DNO_PRINT
GZZ_APP_CONFIG  ?=single_master_slave

CCOPT +=-DDEFAULT_ALLOC_TIMEOUT_MS=4000

CCOPT += -D__MMS_DEBUG__
CCOPT += -D___RTEMS___
CCOPT += -DSPI_SLAVE
CCOPT += -DCAMERA_INTERFACE_ANDROID_CAMERA3
CCOPT += -I$(GUZZI_INC_DIR)/osal/include
CCOPT += -I$(GUZZI_INC_DIR)/hif/rpc/include
CCOPT += -I$(GUZZI_INC_DIR)/hif/camera3/c/include
CCOPT += -I$(GUZZI_INC_DIR)/dtp/server/source/include
CCOPT += -I$(GUZZI_INC_DIR)/guzzi/modules
CCOPT += -I$(GUZZI_INC_DIR)/guzzi/modules/camera
CCOPT += -I$(GUZZI_INC_DIR)/guzzi/modules/libs
CCOPT += -I$(GUZZI_INC_DIR)/guzzi/modules/include
CCOPT += -I$(GUZZI_INC_DIR)/guzzi/modules/hal/include
CCOPT += -I$(GUZZI_INC_DIR)/guzzi/modules/version_info/include
CCOPT += -I$(GUZZI_INC_DIR)/algorithms/source/include/aca
CCOPT += -I$(GUZZI_INC_DIR)/algorithms/source/include/sg/algs
CCOPT += -I$(GUZZI_LIB_DIR)/dtp_db

ifeq ($(GZZ_APP_CONFIG),master_all)
CCOPT += -D'GZZ_MASTER_SLAVE_SELECT=0'
endif
ifeq ($(GZZ_APP_CONFIG),single_master_slave)
CCOPT += -D'GZZ_MASTER_SLAVE_SELECT=2'
endif
ifeq ($(GZZ_APP_CONFIG),master_slave_slave)
CCOPT += -D'GZZ_MASTER_SLAVE_SELECT=1'
endif
ifeq ($(GZZ_APP_CONFIG),master_slave_alt)
CCOPT += -D'GZZ_MASTER_SLAVE_SELECT=3'
endif

CCOPT_LRT += -I$(IPIPE_BASE)/components/mod_i2c_command/leon/
CCOPT_LRT += -I$(MV_COMMON_BASE)/components/I2CSlave/leon

CCOPT += -D'DTP_DB_NAME="database_IMX378_OV9282mono.bin"'
CCOPT += -D'METADATA_DEVNAME="/dev/metadata"'

DefaultSparcRTEMSLibs += $(GUZZI_LIB_DIR)/libguzzi_ma2x8x.a
DefaultSparcRTEMSLibs += $(GUZZI_LIB_DIR)/libversion_info.a


UNIQUE_SYMBOLS += MonoPipe_OS MonoPipe_RT
LEON_COMPONENT_CPP_SOURCES_LOS += $(DATA_COLLECTOR_PATH)/LOS/MonoPipe_OS.cpp
LEON_COMPONENT_CPP_SOURCES_LRT += $(DATA_COLLECTOR_PATH)/LRT/MonoPipe_RT.cpp

endif


#Use asm code
MVCCOPT_LRT  += -D'SIPP_USE_MVCV'

UNIQUE_SYMBOLS += RemConfigOut RemConfigIn RemLeftOut RemRightOut RemOutSyncOut RemRotOut bLOS
UNIQUE_SYMBOLS += RemLeftIn RemRightIn RemOutSyncIn RemRotIn bLRT


# ------------------------------[ Components used ]--------------------------------#

#-----------------------------------[ dogFreak Components ]--------------------------#
LEON_COMPONENT_CPP_SOURCES_LRT += $(DATA_COLLECTOR_PATH)/LRT/DataCollector.cpp
LEON_COMPONENT_CPP_SOURCES_LRT += $(wildcard $(DATA_COLLECTOR_PATH)/shared/*.cpp)

LEON_HEADERS_LRT += $(wildcard $(DATA_COLLECTOR_PATH)/LRT/*.h)
LEON_HEADERS_LRT += $(wildcard $(DATA_COLLECTOR_PATH)/shared/*.h)

LEON_COMPONENT_CPP_SOURCES_LOS += $(DATA_COLLECTOR_PATH)/LOS/AecNode.cpp
LEON_COMPONENT_CPP_SOURCES_LOS += $(DATA_COLLECTOR_PATH)/LOS/DataCollector.cpp
LEON_COMPONENT_CPP_SOURCES_LOS += $(DATA_COLLECTOR_PATH)/LOS/outStreamNode.cpp
LEON_COMPONENT_CPP_SOURCES_LOS += $(wildcard $(DATA_COLLECTOR_PATH)/shared/*.cpp)


LEON_HEADERS += $(wildcard $(DATA_COLLECTOR_PATH)/LOS/*.h)
LEON_HEADERS += $(wildcard $(DATA_COLLECTOR_PATH)/shared/*.h)

CCOPT += -D'DEF_POOL_SZ = 96*1024*1024'
CCOPT_LRT += -D'DEF_POOL_SZ = 64*1024*1024'

CCOPT += -D'MAX_ELEM_PER_BUFF=128'
CCOPT += -D'MAX_FRAMES=128'
CCOPT += -D'FLIC_2LEONS'
CCOPT += -D'DYN_IRQ_ASSIGNMENT=1'

#framePump enable tearing markers
#CCOPT += -D'CONFIG_TEARING_CHECK'
else

endif
