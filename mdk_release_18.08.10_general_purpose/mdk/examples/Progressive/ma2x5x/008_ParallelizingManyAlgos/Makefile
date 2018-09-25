# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_REV      ?= ma2450
# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

# ------------------------------[ Components used ]--------------------------------#
ComponentList_LOS += PipePrint FatalExtension
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no

#-------------------------------[ Settings ]---------------------------------------#
MV_SOC_PLATFORM ?= myriad2
#-------------------------------[ Local shave applications sources ]--------------------#
MYRIAD = myriad2Asm

#Choosing C sources the brthYASM application on shave
BrthYASMApp = shave/brthYASM
SHAVE_C_SOURCES_brthYASM =  $(DirAppRoot)/shave/shaveStartBrightness.c
#Choosing ASM sources for the shave brthYASM app on shave
SHAVE_ASM_SOURCES_brthYASM = $(DirAppRoot)/shave/$(MYRIAD)/effectASM.asm
#SHAVE_ASM_SOURCES_brthYASM =

#Generating list of required generated assembly files for the brthYASM app on shave
SHAVE_GENASMS_brthYASM = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_brthYASM))
#Generating required objects list from sources
SHAVE_brthYASM_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_brthYASM)) \
					$(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_brthYASM))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_brthYASM) $(SHAVE_brthYASM_OBJS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_brthYASM)

#-------------------------------[ Local shave applications sources ]--------------------#
#Choosing name for the aacEncode - not optimized in assembly (aacEncode_noasm) application on shave
SobelApp = shave/sobel
SHAVE_C_SOURCES_Sobel = $(DirAppRoot)/shave/shaveStartSobel.c
#Choosing ASM sources for the shave aacEncode_asmopt app on shave
SHAVE_ASM_SOURCES_Sobel =  $(DirAppRoot)/shave/$(MYRIAD)/Sobel.asm
#Generating list of required generated assembly files for the brthYASM app on shave
SHAVE_GENASMS_Sobel = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_Sobel))
#Generating required objects list from sources
SHAVE_Sobel_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_Sobel)) \
					$(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_Sobel))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_Sobel_OBJS) $(SHAVE0_APPS) $(SHAVE_APP_LIBS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_Sobel)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(BrthYASMApp).mvlib $(SobelApp).mvlib
SHAVE0_APPS = $(BrthYASMApp).shv0lib
SHAVE1_APPS = $(BrthYASMApp).shv1lib
SHAVE2_APPS = $(SobelApp).shv2lib


# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION ?=

# Verbose or not, uncomment for more verbosity
#ECHO ?=

RAWDATAOBJECTFILES += $(DirAppObjDir)/testframe.o

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

LEON_HEADERS += $(MV_COMMON_BASE)/components/kernelLib/MvCV/include/imgproc.h

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the brthYASMApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
#Describe the rule for building the Sobel application.
ENTRY1 = -e startSobel --gc-sections
$(SobelApp).mvlib : $(SHAVE_Sobel_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRY1) $(SHAVE_Sobel_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@
ENTRY2 = -e startBrightness --gc-sections
$(BrthYASMApp).mvlib : $(SHAVE_brthYASM_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRY2) $(SHAVE_brthYASM_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT			+= -DDEBUG

MY_RESOURCE = $(MV_EXTRA_DATA)/DunLoghaire_80x60.yuv

# ------------------------------[ Extra Rules ]-------------------------------------------#

DDR_DATA = .ddr.data
REVERSE_BYTES=

# Add to MVASM include path
$(DirAppObjDir)/testframe.o: $(MY_RESOURCE) Makefile
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=inputFrame \
	-O elf32-sparc -B sparc $< $@

TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150,TCL_MA2150, MA2450"
