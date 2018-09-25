# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk

MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

# ------------------------------[ Components used ]--------------------------------#
#Component lists:
ComponentList_LOS += PipePrint FatalExtension
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no

#-------------------------------[ Settings ]---------------------------------------#

#-------------------------------[ Local shave applications sources ]--------------------#

#Choosing C sources the brthY application on shave
BrthYApp = shave/brthY
SHAVE_C_SOURCES_brthY = $(wildcard $(DirAppRoot)/shave/*.c)
#Choosing ASM sources for the shave brthY app on shave
#no asm files
#SHAVE_ASM_SOURCES_brthY =

#Generating list of required generated assembly files for the brthY app on shave
SHAVE_GENASMS_brthY = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_brthY))
#Generating required objects list from sources
SHAVE_brthY_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_brthY))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_brthY) $(SHAVE_brthY_OBJS) $(SHAVE0_APPS) $(SHAVE_APP_LIBS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_brthY)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(BrthYApp).mvlib
SHAVE0_APPS = $(BrthYApp).shv0lib

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

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the brthYApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e start  --gc-sections
$(BrthYApp).mvlib : $(SHAVE_brthY_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD)  $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_brthY_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@


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
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=inputFrame \
	-O elf32-sparc -B sparc $< $@

TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150,TCL_MA2150, MA2450"
