# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_REV ?= ma2450

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

DirLDScrCommon       ?= $(MV_COMMON_BASE)/scripts/ld/$(MV_SOC_PLATFORM)collection

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

#Component lists:
ComponentList_LOS += PipePrint FatalExtension

CCOPT_EXTRA +=

#-------------------------------[ Settings ]---------------------------------------#

MV_LOCAL_OBJ = no
#-------------------------------[ Local shave applications sources ]--------------------#
MYRIAD = myriad2Asm

#Choosing C sources the brthYASM application on shave
BrthYASMApp = shave/$(MYRIAD)/brthYASM
SHAVE_C_SOURCES_brthYASM = $(wildcard $(DirAppRoot)/shave/*.c)
#Choosing ASM sources for the shave brthYASM app on shave
SHAVE_ASM_SOURCES_brthYASM = $(wildcard $(DirAppRoot)/shave/$(MYRIAD)/*.asm)
#SHAVE_ASM_SOURCES_brthYASM =

#Generating list of required generated assembly files for the brthYASM app on shave
SHAVE_GENASMS_brthYASM = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_brthYASM))
#Generating required objects list from sources
SHAVE_brthYASM_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_brthYASM)) \
					$(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_brthYASM))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_brthYASM) $(SHAVE_brthYASM_OBJS)
PROJECTCLEAN += $(SHAVE0_APPS) $(SHAVE_APP_LIBS) $(BrthYASMApp).mvlib $(BrthYASMApp).shv6lib $(BrthYASMApp).shv7lib
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
#PROJECTINTERM += $(SHAVE_GENASMS_brthYASM)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(BrthYASMApp).mvlib
SHAVE6_APPS = $(BrthYASMApp).shv6lib
SHAVE7_APPS = $(BrthYASMApp).shv7lib

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION = Latest

# Verbose or not, uncomment for more verbosity
#ECHO=

RAWDATAOBJECTFILES += $(DirAppObjDir)/testframe.o

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the brthYASMApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e start  --gc-sections
$(BrthYASMApp).mvlib : $(SHAVE_brthYASM_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_brthYASM_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@


# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT			+= -DDEBUG

MY_RESOURCE = $(MV_EXTRA_DATA)/DunLoghaire_80x60.yuv

# ------------------------------[ Extra Rules ]-------------------------------------------#

# Add to MVASM include path
$(DirAppObjDir)/testframe.o: $(MY_RESOURCE) Makefile
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=inputFrame \
	-O elf32-sparc -B sparc $< $@

TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150,TCL_MA2150, MA2450"
