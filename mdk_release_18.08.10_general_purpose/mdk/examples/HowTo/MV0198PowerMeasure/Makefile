# ------------------------------[ General Settings ]--------------------------------------#
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

#The Movidius dev kits are 2: MV0212 equipped with MA2450 and MV0182 equipped with
#other revisions(MA2150)
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif

LEON_RT_BUILD = yes

MV_SOC_OS = rtems


# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE   ?= ../../../common

# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to revision defined in mdk/common/tools_settings.mk
# Warning: This is really not recommended

#------------------------------[ Build overrides ]--------------------------------#
# Note: If your project needs a custom linker script create a file called custom.ldscript
# in the project config folder. This will be automatically picked up.

# ------------------------------[ Components used ]--------------------------------#
ifeq ($(EVM_BOARD_USED),MV0182)
 ComponentList_LRT  = Board182
 ComponentList_LOS  = Board182
else
 ComponentList_LRT = MV0212
 ComponentList_LOS = MV0212
endif

ComponentList_LOS   += PipePrint FatalExtension
ComponentList_LRT   += VcsHooks PipePrint
ComponentList_SVE  := kernelLib/MvCV

SHAVE_COMPONENTS=no

#----------------------[ Local shave applications sources ]------------------------#
#Application
mv0198App = shave/mv0198App

#Choosing C shave local sources
SHAVE_C_SOURCES_mv0198App = $(wildcard $(DirAppRoot)/shave/*.c)
#Choosing shave local sources
SHAVE_CPP_SOURCES_mv0198App  += $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/dilate3x3/shave/src/cpp/dilate3x3.cpp


# C compilation is a two stage process, this rule declares the intermediate asmgen file list
SHAVE_GENASMS_mv0198App = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_mv0198App))
SHAVE_GENASMS_mv0198App += $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_mv0198App))

#Generating the required objects list from  all local shave sources
SHAVE_OBJ_mv0198App = $(patsubst %.c,$(DirAppObjBase)%_shave.o,$(SHAVE_C_SOURCES_mv0198App) )
SHAVE_OBJ_mv0198App += $(patsubst %.cpp,$(DirAppObjBase)%_shave.o,$(SHAVE_CPP_SOURCES_mv0198App) )

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_mv0198App) $(SHAVE_OBJ_mv0198App)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_mv0198App)

#--------------------------[ SHAVE SYSTEM BUILD ]--------------------------#

#Creating the mvlib file containing the application
SHAVE_APP_LIBS = $(mv0198App).mvlib

#Placing application libs on the 6 cores
SHAVE0_APPS = $(mv0198App).shv0lib
SHAVE1_APPS = $(mv0198App).shv1lib
SHAVE2_APPS = $(mv0198App).shv2lib
SHAVE3_APPS = $(mv0198App).shv3lib
SHAVE4_APPS = $(mv0198App).shv4lib
SHAVE5_APPS = $(mv0198App).shv5lib
SHAVE6_APPS = $(mv0198App).shv6lib
SHAVE7_APPS = $(mv0198App).shv7lib
SHAVE8_APPS = $(mv0198App).shv8lib
SHAVE9_APPS = $(mv0198App).shv9lib


#add the input image object to RAWDATAOBJECTFILES
RAWDATAOBJECTFILES += $(DirAppObjDir)/testframe.o

#--------------------------[ Include common makefile rules ]--------------------------

include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the mv0198App application.
#Simple rule specifying which objects build up the said application.
#The application will be built into a library

ENTRYPOINTS = -e shaveRun --gc-sections

$(mv0198App).mvlib : $(SHAVE_OBJ_mv0198App) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_OBJ_mv0198App) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT			+= -DDEBUG
CCOPT_LRT += -D$(EVM_BOARD_USED)
CCOPT     += -D$(EVM_BOARD_USED)

#----------[ Add an input image ]------------------------------------------------#
#The input image location
MY_RESOURCE = $(MV_EXTRA_DATA)/CobKitchen_1280x720.yuv

$(DirAppObjDir)/testframe.o: $(MY_RESOURCE) Makefile
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=lrt_frameLocation \
	-O elf32-sparc -B sparc $< $@

# Automated Test Settings
TEST_TAGS        := "MA2150, MA2450"
