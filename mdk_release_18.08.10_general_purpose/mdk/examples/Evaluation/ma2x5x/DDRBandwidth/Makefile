# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))
# Selects the default startup processor (options LOS or LRT) (When not specified Leon OS is the default)
LEON_STARTUP_MASK   ?=LOS

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_PLATFORM = myriad2
MV_SOC_REV      ?= ma2450
# This provides the unit test framework and hooks into the VCS environment
ComponentList := UnitTestVcs VcsHooks PipePrint

# ------------------------------[ Components used ]--------------------------------#
#ComponentList =
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no

#-------------------------------[ Settings ]---------------------------------------#

#
#-------------------------------[ Local shave applications sources ]--------------------#
LEON_C_STD_CHOICE           ?= -std=gnu11
#Choosing C sources the shave application on shave
ShaveApp = shave/shave
SHAVE_C_SOURCES_shave = $(wildcard $(DirAppRoot)/shave/*.c)
#Choosing ASM sources for the shave shave app on shave
#no asm files
#SHAVE_ASM_SOURCES_shave =

#Generating list of required generated assembly files for the shave app on shave
SHAVE_GENASMS_shave = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_shave))
#List of ASM sources for the shave app
SHAVE_ASM_shave = $(wildcard $(DirAppRoot)/shave/asm/*.asm)
#Generating required objects list from sources
SHAVE_shave_OBJS =  $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_shave)) \
                    $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_shave))


RAWDATAOBJECTFILES  = $(ShaveApp)_sym.o $(ShaveApp)_bin.o

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_shave) $(SHAVE_shave_OBJS) $(SHAVE0_APPS) $(SHAVE_APP_LIBS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_shave)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(ShaveApp).mvlib

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =
# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

#Force to not use ddrInit.elf
DDRINIT_SWITCH=
DdrInitElf=

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the ShaveApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library

ENTRYPOINTS = -e svuMemcpyAsm -u svuMemcpyDMA -u svuSystemTestDMA  --gc-sections
$(ShaveApp).mvlib : $(SHAVE_shave_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_shave_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT			+= -DDEBUG

PIPEPRINT_SECTION ?= ".cmx_direct.data"
CCOPT     += -D'PIPEPRINT_SECTION=$(PIPEPRINT_SECTION)'
CCOPT_LRT += -D'PIPEPRINT_SECTION=$(PIPEPRINT_SECTION)'

PIPEPRINT_SIZE ?= 25600
CCOPT     += -D'PIPEPRINT_SIZE=$(PIPEPRINT_SIZE)'
CCOPT_LRT += -D'PIPEPRINT_SIZE=$(PIPEPRINT_SIZE)'
# ---------------------------------[ Extra Rules ]-------------------------------------------#

#This creates a binary file packing the shvdlib file
$(ShaveApp)_bin.o : $(ShaveApp).shvdlib
	$(ECHO) $(OBJCOPY)  -I binary --rename-section .data=.cmx.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=shavedynlib \
	-O elf32-sparc -B sparc $< $@

LDDYNOPT  += --gc-sections  -entry=svuMemcpyAsm -entry=svuMemcpyDMA -entry=svuSystemTestDMA
LDSYMOPT  += --gc-sections  -entry=svuMemcpyAsm -entry=svuMemcpyDMA -entry=svuSystemTestDMA


TEST_TAGS := "MA2150, MA2450"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
