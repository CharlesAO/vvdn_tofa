#Activate the flag that disables debug mode build and keep release mode as default
BUILD_WITH_CODE_SIZE_OPTIMIZATION=yes
# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk

# Ensure that the we are using the correct rtems libs etc
# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../common
MV_SOC_OS = rtems
LEON_RT_BUILD = yes
SHAVE_COMPONENTS = yes
ComponentList_SVE := PipePrint
ComponentList_LOS := UnitTestVcs VcsHooks PipePrint FatalExtension
ComponentList_LRT := UnitTestVcs VcsHooks PipePrint
CCOPT_EXTRA +=
MV_SOC_REV ?=ma2450
#MV_TOOLS_VERSION ?= Latest
#-------------------------------[ Settings ]---------------------------------------#
MV_SOC_PLATFORM = myriad2

#-------------------------------[ Local shave applications sources ]--------------------#

PROJECTCLEAN += test_neg_522x128.raw

#--------------------------[ Shave applications section ]--------------------------#
#Some overrides for MesgOne and MesgTwo
MesgOne_HEAPSIZE  = 3*1024
MesgOne_STACKSIZE = 3*1024
MesgTwo_HEAPSIZE  = 3*1024
MesgTwo_STACKSIZE = 3*1024
MesgThree_HEAPSIZE  = 3*1024

#--------------------------[ Linker script requirement ]---------------------------#


#--------------------------[ Leon RT special requirements because of common shave libs ]-------------#


# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release

#Add more libs here if you wish to link against mutliple other libraries as part of the default
#SHAVE library group
PROJECT_SHAVE_LIBS +=

#---------------------------[ Include SIPP specifics ]----------------------------------#
include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk

#The binary elf for Shave:
svuSippImg = $(DirAppOutput)/svuSippImg
RAWDATAOBJECTFILES += $(svuSippImg)_sym.o $(svuSippImg)Map.o
PROJECTCLEAN += $(svuSippImg)Map.o

#Include Sipp Filter Paths
include ./BayerISPModel.mk

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#---------------------------[ SIPP dynamic library ]---------------------------------#
$(svuSippImg).mvlib : $(SippSvuObj)
	@mkdir -p $(dir $@)
	$(ECHO) $(LD) $(MVLIBOPT) $(SippSvuObj) -o $@

$(svuSippImg)_sym.o : $(svuSippImg).shvdcomplete
	$(ECHO) $(OBJCOPY) --prefix-symbols=lrt_SS_ --extract-symbol $< $@

$(svuSippImg)Map.o : $(svuSippImg).shvdlib
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=lrt_mbinImgSipp \
	-O elf32-littlesparc -B sparc $< $@

# Add to MVASM include path
$(DirAppObjDir)/testframe.o: $(MY_RESOURCE) Makefile
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=inputFrame \
	-O elf32-littlesparc -B sparc $< $@

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the MesgOneApp application. Simple rule specifying
#which objects build up the said application. The application will be built into a library

#Rules for building the group

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT			+= -DDEBUG

MVCCOPT     += -D'SIPP_USE_MVCV'

CCOPT       += -DSIPP_NO_PRINTF
CCOPT_LRT   += -DSIPP_NO_PRINTF

CCOPT_LRT   += -U'DEFAULT_HEAP_SIZE'
CCOPT_LRT   += -D'DEFAULT_HEAP_SIZE=25600'

TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450"
TEST_TYPE   := "AUTO"
TEST_TAGS   := "MA2150,MA2450"
