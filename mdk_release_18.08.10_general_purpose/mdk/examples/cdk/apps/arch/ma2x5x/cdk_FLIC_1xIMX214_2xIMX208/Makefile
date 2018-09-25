MDK_ROOT =  ../../../../../..
GUZZI_INC_DIR =  $(realpath $(MDK_ROOT)/packages/movidius/Guzzi/arch/$(SOC_REV_DRV)/include)
GUZZI_LIB_DIR =  $(realpath $(MDK_ROOT)/packages/movidius/Guzzi/arch/$(SOC_REV_DRV)/lib)

MV_COMMON_BASE  ?= $(MDK_ROOT)/common

$(info ########################## BUILD settings ##########################)
MV_SOC_PLATFORM  = myriad2
MV_SOC_REV  = ma2450
SOC_REV_DRV = ma2x5x
AppConfig = mv182_A214M_B208M_C208S_ma2150


#LinkerScript = $(MV_COMMON_BASE)/components/flic/common/ldscripts/arch/2x5x/LdBaseDdr.ldscript
DirLDScrCommon = $(MV_COMMON_BASE)/scripts/ld/$(MV_SOC_PLATFORM)memorySections
$(info LD script: $(DirLDScrCommon))
DdrInitHeaderMvcmd   ?= $(MV_COMMON_BASE)/utils/ddrInit/ddrInitHeaderForMvcmd_$(MV_SOC_REV)

#MVCONVOPT ?= -cv:$(MV_SOC_REV)
MvCmdfile = ./output/$(APPNAME)_ddrinit.mvcmd
DefaultFlashScript = ./scripts/flash/default_flash_$(MV_SOC_PLATFORM).scr
$(info Flash script: $(DefaultFlashScript))

# Adding dependency from Guzzi camera library
output/$(APPNAME).elf: $(GUZZI_LIB_DIR)/libguzzi_rtems_$(AppConfig).a

################## Application settings #################

LEON_RT_BUILD   ?= no

#Determine board type from Myriad chip revision - not suitable for client side implementations.
$(warning Warning: Board determined based purely on Myriad revision)
ifeq ($(MV_SOC_REV), ma2150) #MV182 Board presumed
TARGET_BOARD 	?= MV0182
else ifeq ($(MV_SOC_REV), ma2450) #MV212 Board presumed
TARGET_BOARD 	?= MV0212
endif #mv212



OUTPUT_UNIT 	?= USB

BOARD_SUPPORT_MIPI = no
BOARD_SUPPORT_HDMI = no
BOARD_SUPPORT_USB  = no

#Initialize error helpers for send out
ifeq ($(TARGET_BOARD),MV0182)

BOARD_SUPPORT_MIPI = yes
BOARD_SUPPORT_HDMI = yes
BOARD_SUPPORT_USB  = yes

else ifeq ($(TARGET_BOARD),MV0212)

BOARD_SUPPORT_MIPI = yes
BOARD_SUPPORT_HDMI = yes
BOARD_SUPPORT_USB  = yes

endif





# select yes to use JTAG or select no to use UART for debug logs
JTAG_LOG        ?= yes

CCOPT += -D$(TARGET_BOARD)
$(info TARGET_BOARD=$(TARGET_BOARD))
$(info OUTPUT_UNIT=$(OUTPUT_UNIT))

#Enable critical sections for Tx
CCOPT += -D'CRITICAL_SECTION_ENABLE=1'

#######################################################################################################################
# ~~~~~~~~~~ App Specific defines ~~~~~~~~~~~~~~~~~
CCOPT += -DAPP_CONFIGURATION
CCOPT += -D'MAX_NR_OF_CAMS                   = 3'
CCOPT += -D'PLG_SRC_MAX_SOURCES              = 3'
CCOPT += -D'PLG_SRC_MAX_W                    = 4320'
CCOPT += -D'RX_CBUF_MUL_H                    = 5'
CCOPT += -D'FRAME_T_HEADER_BUFFER_HEIGHT     = 2'
CCOPT += -D'PLG_ISP_MAX_W                    = 2104'


CCOPT += -DWORKAROUND_FOR_FLIC_ON_M2_3xIMX208
CCOPT += -D'DEFAULT_CDMA_INT_LEVEL=10'

TEMPLPROC   ?= ../version_info/tmpl_proc.sh
VERVARS     ?= ../version_info/vervars.sh


IPIPE_COMPONENTS = cdk/arch/ma2x5x/components
################## Application settings END #################
# Basic project modules included

#Component lists:
ComponentList_LOS += Opipe VcsHooks UnitTest Fp16Convert PipePrint
ComponentList_LOS += I2CSlave
#ComponentList_LOS += flic/common/rtemsCfg/cfg02
ComponentList_LOS += flic/common/commonTypes
ComponentList_LOS += flic/core
ComponentList_LOS += flic/types
ComponentList_LOS += flic/plugins/arch/ma2x5x/common/commonTypes
ComponentList_LOS += flic/plugins/arch/ma2x5x/common/ispCommonOpipe
ComponentList_LOS += flic/plugins/arch/ma2x5x/common
ComponentList_LOS += flic/plugins/plgPool
ComponentList_LOS += flic/plugins/arch/ma2x5x/plgOutItf
ComponentList_LOS += flic/plugins/arch/ma2x5x/plgSource
ComponentList_LOS += flic/plugins/arch/ma2x5x/plgIspProc
ComponentList_LOS += flic/plugins/arch/ma2x5x/plgIspMono
#ComponentList_LOS += flic/components/drvSensor
#ComponentList_LOS += flic/components/ispCam3ACtrl
ComponentList_LOS += flic/components/ispInterface
ComponentList_LOS += flic/plugins/arch/ma2x5x/plgIspCtrl
#ComponentList_LOS += flic/components/boards/brdMv182

ComponentList_LOS   += $(IPIPE_COMPONENTS)/sendOut/common
#ComponentList_LOS += ../../examples/cdk/arch/ma2x5x/components/sendOut/common
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CCOPT += -D'OPIPE_MEM_SECT=".cmx.opipe"'

#FLIC settings
CCOPT += -D'DEFAULT_APP_CLOCK_KHZ=600000'

CCOPT += -D'DEF_POOL_SZ = 256*1024*1024'

#FPS config (FRAME_TIME = RTEMS TICKS = miliseconds)
CCOPT += -D'FRAME_TIME=33'

CCOPT += -D'APP_SPECIFC_INTERFACE'
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#MIPI Output unit
ifeq ($(OUTPUT_UNIT),MIPI)

ifeq ($(BOARD_SUPPORT_MIPI), no)
$(error ERROR: MIPI is not supported for the $(TARGET_BOARD) board type)
endif #mv182 or mv212

ComponentList_LOS   += $(IPIPE_COMPONENTS)/sendOut/MipiSend
ComponentList_LOS   += LcdGeneric
CCOPT += -DOUTPUT_UNIT_IS_MIPI
CCOPT += -D'FRAME_T_BUFFER_OFFSET = 0'

#HDMI Output unit
else ifeq ($(OUTPUT_UNIT),HDMI)

ifeq ($(BOARD_SUPPORT_HDMI), no)
$(error ERROR: HDMI is not supported for the $(TARGET_BOARD) board type)
endif #mv182 or mv212

ComponentList_LOS   += $(IPIPE_COMPONENTS)/sendOut/LcdHdmiSend
ComponentList_LOS   += LcdGeneric

CCOPT += -DOUTPUT_UNIT_IS_HDMI
CCOPT += -D'FRAME_T_BUFFER_OFFSET = 0'

ifeq ($(TARGET_BOARD),MV0182)
ComponentList_LOS   += Board182
endif

ifeq ($(TARGET_BOARD),MV0212)
ComponentList_LOS   += MV0212
endif

#USB Output unit
else ifeq ($(OUTPUT_UNIT),USB)

ifeq ($(BOARD_SUPPORT_USB), no)
$(error ERROR: USB is not supported for the $(TARGET_BOARD) board type)
endif #mv182 or mv212

ComponentList_LOS   += $(IPIPE_COMPONENTS)/sendOut/UsbSend
CCOPT += -DOUTPUT_UNIT_IS_USB
CCOPT += -DINPUT_UNIT_IS_USB
CCOPT += -D'FRAME_T_BUFFER_OFFSET = 16'
CCOPT += -D'USB_REFCLK_MHZ=USB_REFCLK_20MHZ'
RTEMS_USB_LIB_BUILD_TYPE = release
MV_USB_PROTOS = protovideo
#Error output unit
else
$(error ERROR: Unknown output unit - $(OUTPUT_UNIT))
endif

MV_LOCAL_OBJ=yes

# Ensure that the we are using the correct rtems libs etc RTEMS ADDING
MV_SOC_OS = rtems
CCOPT_EXTRA = -D'USE_SOFTWARE=1'
#CCOPT += -D'RTEMS_LOS'
DISABLE_ERROR_ON_WARNINGS ?=yes

# app can decide, how mutch memory can be available for isp, and section name where this are.
CCOPT_LRT += -D'OPIPE_MEM_SECT=".cmx.data"'
CCOPT_LRT += -D'OPIPE_CBUFF_SZ=720896'

#CCOPT += -D'SIPP_IRQ_LEVEL = 5'
#########################################################################################


#########################################################################################
# OS-Leon (client)
#########################################################################################



#########################################################################################
# OS-Leon (SPI slave api)
#########################################################################################
SPI_SLAVE_API_BASE  = $(MV_COMMON_BASE)/components/MessageProtocol/leon/rtems/
CCOPT              += -I$(SPI_SLAVE_API_BASE)/include
LEON_APP_C_SOURCES += $(SPI_SLAVE_API_BASE)/src/MessRingBuff.c
LEON_APP_C_SOURCES += $(SPI_SLAVE_API_BASE)/src/MessageProtocol.c
LEON_APP_C_SOURCES += $(SPI_SLAVE_API_BASE)/src/OsDrvSpiSlaveCP.c
LEON_APP_C_SOURCES += $(SPI_SLAVE_API_BASE)/src/OsMessageProtocol.c


#########################################################################################
#Critical to allow multiple symbols definitions
LDOPT_LRT += --allow-multiple-definition
LDOPT += --allow-multiple-definition

#########################################################################################
# Include the top makefile
include $(MV_COMMON_BASE)/generic.mk

######### Extra app related options ##########
CCOPT			+= -DDEBUG
#CCOPT += -DNO_PRINT

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

CCOPT += -D'DTP_DB_NAME="database_IMX214_IMX208C_IMX208M.bin"'

DefaultSparcRTEMSLibs += $(GUZZI_LIB_DIR)/libguzzi_rtems_$(AppConfig).a
DefaultSparcRTEMSLibs += $(GUZZI_LIB_DIR)/libversion_info.a
#DefaultSparcRTEMSLibs += $(GUZZI_LIB_DIR)/liblib_ipipe_simulator.a

#########################################################################################
# Boot over spi (with DDR init) rule

output/$(APPNAME)_ddrinit.mvcmd: spi-boot

spi-boot:  output/$(APPNAME).mvcmd
	make output/$(APPNAME).mvcmd -j CCOPT_EXTRA="-DNO_PRINT" CCOPT_EXTRA_LRT="-DNO_PRINT"

	dd if=/dev/zero of=output/nulllcmd.mvcmd bs=1K count=1 # at 10MHz boot we should have a 819us timeout which is almost double thant the required 450us for DDr init reported
	cat $(DdrInitHeaderMvcmd) \
		./output/nulllcmd.mvcmd \
		./output/$(APPNAME).mvcmd > ./output/$(APPNAME)_ddrinit.mvcmd

# adb wait-for-device
# adb remount
# adb push ./output/$(APPNAME)_ddrinit.mvcmd /system/vendor/firmware/movidius/myriad_default.mvcmd
# adb shell sync
# adb shell sync
# adb shell "echo -n movidius/myriad_default.mvcmd > /sys/class/misc/myriad/boot_fw"
# adb shell sync
# adb shell sync
# adb shell sync

TEST_TYPE := "MANUAL"
$(info ####################################################################)

