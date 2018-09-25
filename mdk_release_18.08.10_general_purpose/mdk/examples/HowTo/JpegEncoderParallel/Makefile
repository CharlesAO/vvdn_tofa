# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Components used ]--------------------------------#
ComponentList_LOS := UnitTest VcsHooks JpegM2EncoderParallel PipePrint
ComponentList_SVE := JpegM2EncoderParallel
#Choosing if this project has shave components or not
SHAVE_COMPONENTS=yes
#...From enviornoments

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE   ?= ../../../common


MV_SOC_PLATFORM = myriad2
MV_SOC_REV      ?= ma2450

# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to La800_480


#-------------------------------[ Local shave applications sources ]--------------------#
#Choosing C sources the JpegM2EncoderParallel application on shave
JpegM2EncoderParallelApp = output/JpegM2EncoderParallel
SHAVE_C_SOURCES_JpegM2EncoderParallel = $(wildcard $(MV_COMMON_BASE)/components/JpegM2EncoderParallel/*shave*/*.c)

#Choosing ASM sources for the shave JpegM2EncoderParallel app on shave
SHAVE_ASM_SOURCES_JpegM2EncoderParallel  = $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/jpegGetBlockY420/arch/ma2x5x/shave/src/jpegGetBlockY420.asm
SHAVE_ASM_SOURCES_JpegM2EncoderParallel += $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/dct16bit/arch/ma2x5x/shave/src/dct16bit.asm
SHAVE_ASM_SOURCES_JpegM2EncoderParallel += $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/huffman_encode/arch/ma2x5x/shave/src/huffman_encode.asm

#Generating list of required generated assembly files for the JpegM2EncoderParallel app on shave
SHAVE_GENASMS_JpegM2EncoderParallel = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_JpegM2EncoderParallel))

#Generating required objects list from sources
SHAVE_JpegM2EncoderParallel_OBJS = $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_JpegM2EncoderParallel)) \
                   $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_JpegM2EncoderParallel))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_JpegM2EncoderParallel) $(SHAVE_JpegM2EncoderParallel_OBJS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_JpegM2EncoderParallel)

#--------------------------[ Shave applications section ]--------------------------#

SHAVE_APP_LIBS=$(JpegM2EncoderParallelApp).mvlib
SHAVE0_APPS=$(JpegM2EncoderParallelApp).shv0lib
SHAVE1_APPS=$(JpegM2EncoderParallelApp).shv1lib
SHAVE2_APPS=$(JpegM2EncoderParallelApp).shv2lib
SHAVE3_APPS=$(JpegM2EncoderParallelApp).shv3lib
SHAVE4_APPS=$(JpegM2EncoderParallelApp).shv4lib
SHAVE5_APPS=$(JpegM2EncoderParallelApp).shv5lib
SHAVE6_APPS=$(JpegM2EncoderParallelApp).shv6lib
SHAVE7_APPS=$(JpegM2EncoderParallelApp).shv7lib
SHAVE8_APPS=$(JpegM2EncoderParallelApp).shv8lib
SHAVE9_APPS=$(JpegM2EncoderParallelApp).shv9lib
SHAVE10_APPS=$(JpegM2EncoderParallelApp).shv10lib
SHAVE11_APPS=$(JpegM2EncoderParallelApp).shv11lib

# Chose one of the predefined quantization tables.
# Available values 90 and 50 default is 50.
MVCCOPT += -D'_USE_QUANTIZATION_TABLE_Q_=50'

CCOPT += -DMYRIAD2
JpegM2EncoderParallel_platform = myriad2
DDR_DATA = .ddr.data
CCOPT			+= -D'JPEG_DATA_SECTION(x)=__attribute__((section(".ddr.data"))) x'
CCOPT			+= -D'JPEG_JPEGGUARD_DATA_SECTION(x)=__attribute__((section(".ddr.data"))) x'
CCOPT			+= -D'JPEG_BITSREAM_DATA_SECTION(x)=__attribute__((section(".cmx.jpegdata"))) x'
CCOPT			+= -D'JPEG_CONST_DATA_SECTION(x)=__attribute__((section(".cmx.const.data"))) x'
CCOPT			+= -D'JPEG_CODE_SECTION(x)=__attribute__((section(".cmx.jpegtext"))) x'

SHAVE_HEADERS += $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/jpegGetBlockY420/shave/include/jpegGetBlockY420.h
SHAVE_HEADERS += $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/dct16bit/shave/include/dct16bit.h
SHAVE_HEADERS += $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/huffman_encode/shave/include/huffman_encode.h
SHAVE_HEADERS += $(MV_COMMON_BASE)/components/kernelLib/MvCV/include/*

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the JpegM2EncoderParallelApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e start -u huffman_start -u startLastShave --gc-sections
$(JpegM2EncoderParallelApp).mvlib : $(SHAVE_JpegM2EncoderParallel_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(SHAVE_JpegM2EncoderParallel_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@



# -------------------------------- [ Build Options ] ------------------------------ #
# App related driver build options
TEST_INPUT_FOLDER = $(MV_COMMON_BASE)/../resources
# -------------------------------- [ Extra dependancies] ---------------------------#
# App related build options

# Extra app related options
#CCOPT			+= -DDEBUG

#Define Choose shave for the JPEG encoder component (if different than 0 which is default)
#CCOPT           += -D'JPEGe_SHAVE_NO=0'

# The sample profiler will use DDR. The needed DDR buffer size can be determined using the following two configuration parameters.
# Please check the readme how to calculate the free DDR space needed.
# The sample duration should be configured. For most of the applications, this value should be ok. If the application runs shaves for a long time, bigger values can be used for decreasing the used DDR size
SAMPLE_PERIOD_MICRO=10
# This value should be configured to cover the total runtime of shaves. In the current example we are using 12 shaves, ~128 ms each. This means 1536 ms. We are setting a bigger value for this, to be on the safe side
SAMPLE_DURATION_MS=5000
# Enable Graph creation. Value 0 means text output only. 2 benefits: easier setup on Windows, faster execution
SAMPLE_PROFILER_GRAPHS_ENABLED=0

TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE := "AUTO"
TEST_TAGS := "MA2150, MDK_COMPONENTS, MA2450"
