

# Verbose or not, common out for more verbosity
ECHO ?= @

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_REV ?= ma2450
ifeq ($(MV_SOC_REV),ma2450)
 EVM_BOARD_USED = MV0212
else
 EVM_BOARD_USED = MV0182
endif

MV_SOC_PLATFORM ?= myriad2

MV_COMMON_BASE  ?= ../../../common
DirAppRoot = ./
DirAppOutput = ./output

LEON_RT_BUILD    ?= yes

MV_SOC_OS = rtems

# ------------------------------[ Components used ]--------------------------------#
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = no
ComponentList_LOS := PipePrint FatalExtension
ComponentList_LRT := PipePrint

# Adding the Camera component:
ComponentList_LRT += CamGeneric CameraModules CameraModules/imx214 CameraModules/imx378

# Adding the Lcd component:
ComponentList_LRT += LcdGeneric

# Adding the dev board component:
ifeq ($(EVM_BOARD_USED),MV0212)
 ComponentList_LRT += MV0212
else
 ComponentList_LRT  += Board182
endif

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
# MV_TOOLS_VERSION ?=

#-------------------------------[ Settings ]---------------------------------------#

# configure lcd component options:
CCOPT_LRT   += -DUse_LcdGeneric_on_LRT

# Include SIPP specifics
include $(MV_COMMON_BASE)/components/sipp/arch/$(MV_SOC_REV)/build/myriad2/sippMyriad2Elf.mk

# Sipp options
CCOPT       += -DSIPP_NO_PRINTF
CCOPT_LRT   += -DSIPP_NO_PRINTF
MVCCOPT_LRT += -D'SIPP_USE_MVCV'
CCOPT_LRT   += -U'SIPP_CMX_POOL_SZ'
# Pool size to allocate inside one shave slice
CCOPT_LRT   += -DSIPP_CMX_POOL_SZ=196608

#The binary elf for Shave:
svuSippImg = $(DirAppOutput)/svuSippImg
RAWDATAOBJECTFILES += $(svuSippImg)_sym.o

#Include Sipp Filter Paths
include ./build/sipp/*.mk


#-------------------------------[ MDK Build options ]---------------------------------------#
# Creating define for the pre-compiler to know what dev board has been selected
CCOPT_LRT += -D$(EVM_BOARD_USED)
CCOPT     += -D$(EVM_BOARD_USED)

# Add SIPP as a target for make clean
PROJECTCLEAN += $(SippSvuObj)

# Include the top makefile
include $(MV_COMMON_BASE)/generic.mk

# Add rules for SIPP binaries:
$(svuSippImg).mvlib : $(SippSvuObj)
	@mkdir -p $(dir $@)
	$(ECHO) $(LD) $(MVLIBOPT) $(SippSvuObj) -o $@

$(svuSippImg)_sym.o : $(svuSippImg).shvdcomplete
	$(ECHO) $(OBJCOPY) --prefix-symbols=lrt_SS_ --extract-symbol $< $@

$(DirAppObjBase)$(DirAppRoot)/leon_rt/appMemMap_lrt.o : $(svuSippImg).shvdlib

