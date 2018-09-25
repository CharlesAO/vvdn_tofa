# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_REV ?= ma2450

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems
#Component lists:
ComponentList_LOS = PipePrint FatalExtension UnitTestVcs VcsHooks

CCOPT_EXTRA +=

#MV_TOOLS_VERSION ?=
#-------------------------------[ Settings ]---------------------------------------#
MV_LOCAL_OBJ = no
#-------------------------------[ Local shave applications sources ]--------------------#


#Choosing C sources the AddShave application on shave
AddApp = shave_add/myriad2Asm/addshave
SHAVE_C_SOURCES_AddApp = $(wildcard $(DirAppRoot)/shave_add/*.c)
#Choosing ASM sources for the shave hello app on shave
SHAVE_ASM_SOURCES_AddApp = $(wildcard $(DirAppRoot)/shave_add/myriad2Asm/*.asm)

#Generating list of required generated assembly files for the hello app on shave
SHAVE_GENASMS_AddApp = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_AddApp))
#Generating required objects list from sources
SHAVE_AddApp_OBJS = $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_AddApp)) \
                   $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_AddApp))

#Choosing C sources the SubShave application on shave
SubApp = shave_sub/myriad2Asm/subshave
SHAVE_C_SOURCES_SubApp = $(wildcard $(DirAppRoot)/shave_sub/*.c)
#Choosing ASM sources for the shave hello app on shave
SHAVE_ASM_SOURCES_SubApp = $(wildcard $(DirAppRoot)/shave_sub/myriad2Asm/*.asm)

#Generating list of required generated assembly files for the hello app on shave
SHAVE_GENASMS_SubApp = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_SubApp))
#Generating required objects list from sources
SHAVE_SubApp_OBJS = $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_SubApp)) \
                   $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_SubApp))

RAWDATAOBJECTFILES  = $(AddApp)_sym.o $(AddApp)_bin.o
RAWDATAOBJECTFILES += $(SubApp)_sym.o $(SubApp)_bin.o

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_AddApp) $(SHAVE_AddApp_OBJS) $(AddApp).mvlib
PROJECTCLEAN += $(SHAVE_GENASMS_SubApp) $(SHAVE_SubApp_OBJS) $(SubApp).mvlib
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_AddApp)
PROJECTINTERM += $(SHAVE_GENASMS_SubApp)

PROJECTCLEAN += $(AddApp).shvdlib $(AddApp).map
PROJECTCLEAN += $(AddApp).shvdcomplete $(AddApp)_sym.o
PROJECTCLEAN += $(SubApp).shvdlib $(SubApp).map
PROJECTCLEAN += $(SubApp).shvdcomplete $(SubApp)_sym.o


# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release


# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the AddApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e shaveentry  --gc-sections
$(AddApp).mvlib : $(SHAVE_AddApp_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_AddApp_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

#Describe the rule for building the SubApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS = -e shaveentry  --gc-sections
$(SubApp).mvlib : $(SHAVE_SubApp_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_SubApp_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@


# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT += -DDEBUG

# ------------------------------[ Extra Rules ]-------------------------------------------#


#This creates a binary file packing the shvdlib file
$(AddApp)_bin.o : $(AddApp).shvdlib
	$(ECHO) $(OBJCOPY)  -I binary --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=adddyn \
	-O elf32-sparc -B sparc $< $@


# App related build options
#This creates a binary file packing the shvdlib file
$(SubApp)_bin.o : $(SubApp).shvdlib
	$(ECHO) $(OBJCOPY)  -I binary --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=subdyn \
	-O elf32-sparc -B sparc $< $@

LDDYNOPT  += --gc-sections -entry=shaveentry
LDSYMOPT  += --gc-sections -entry=shaveentry

TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, MA2450"
TEST_HW_PLATFORM := "MOVISIM_MA2150, MOVISIM_MA2450, MV0182_MA2150, MV0212_MA2450 "
