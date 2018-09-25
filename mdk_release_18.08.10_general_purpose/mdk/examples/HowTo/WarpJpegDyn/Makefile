# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
APPNAME = warpEff
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

#The Movidius dev kits are 2: MV0212 equipped with MA2450 and MV0182 equipped with
#other revisions(MA2150)
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif
# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
LEON_RT_BUILD = yes

# ------------------------------[ Components used ]--------------------------------#
ifeq ($(EVM_BOARD_USED),MV0182)
 ComponentList  = Board182
else
 ComponentList  = MV0212
endif

ComponentList_LOS += PipePrint UnitTest VcsHooks FatalExtension
ComponentList_LRT := PipePrint CameraModules CameraModules/imx208 LcdGeneric CamGeneric PipePrint imageWarpDyn JpegM2EncoderDyn
ComponentList_SVE := JpegM2EncoderDyn imageWarpDyn kernelLib/MvCV/kernels/warpMeshExpand kernelLib/MvCV/include kernelLib/MvCV/kernels/warpMeshSample8bit
ComponentList_SVE += kernelLib/MvCV/kernels/dct16bit kernelLib/MvCV/kernels/jpegGetBlockY420 kernelLib/MvCV/kernels/huffman_encode

SHAVE_COMPONENTS = yes
# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

imageWarpDynlib_HEAPSIZE  = 0
imageWarpDynlib_HEAP_INIT = DYNCONTEXT_HEAP_NOINIT

JpegEncoderShaveDynlib_HEAPSIZE  = 0
JpegEncoderShaveDynlib_HEAP_INIT = DYNCONTEXT_HEAP_NOINIT
#-------------------------------[ Local shave applications sources ]--------------------#
#No local shave applications for this example

#--------------------------[ Shave applications section ]--------------------------#
#No local shave applications for this example

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =

# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#No local applications build rules for this example

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options
CCOPT_LRT += -D$(EVM_BOARD_USED)
CCOPT_LRT += -DUse_LcdGeneric_on_LRT
CCOPT     += -D$(EVM_BOARD_USED)


CCOPT_LRT   += -U'DEFAULT_HEAP_SIZE'
CCOPT_LRT   += -D'DEFAULT_HEAP_SIZE=40960'


# Extra app related options
CCOPT += -DNO_PRINT

#Mark test as running long
TEST_TAGS:= "MA2150, MA2450, LONG_RUN_TEST"
