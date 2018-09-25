#------[ Local shave applications sources ]-----#

#------------------ COST APP -------------#
costApp = $(MV_COMMON_BASE)/components/stereo/sgbm/shave/costApp
SHAVE_CPP_SOURCES_costApp = $(MV_COMMON_BASE)/components/stereo/sgbm/shave/computeCost.cpp
SHAVE_ASM_SOURCES_costApp = $(SHAVE_ASM_SOURCES_APPNAME1)
# Generating list of required generated assembly files for the SGBMApp app on shave
SHAVE_GENASMS_costApp += $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_costApp))
# Generating required objects list from sources
SHAVE_costApp_OBJS  = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_costApp))
SHAVE_costApp_OBJS += $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_costApp))

#------------------ SGBM APP -------------------#
SGBMApp = $(MV_COMMON_BASE)/components/stereo/sgbm/shave/SGBMApp
SHAVE_CPP_SOURCES_SGBMApp += $(MV_COMMON_BASE)/components/stereo/sgbm/shave/disparityMap.cpp
SHAVE_CPP_SOURCES_SGBMApp += $(MV_COMMON_BASE)/components/stereo/sgbm/shave/helpers.cpp
SHAVE_CPP_SOURCES_SGBMApp +=$(MV_COMMON_BASE)/components/stereo/sgbm/shave/sgbm.cpp
SHAVE_ASM_SOURCES_SGBMApp = $(SHAVE_ASM_SOURCES_APPNAME1)
# Generating list of required generated assembly files for the SGBMApp app on shave
SHAVE_GENASMS_SGBMApp  = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_SGBMApp))
SHAVE_GENASMS_SGBMApp += $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_SGBMApp))
# Generating required objects list from sources
SHAVE_SGBMApp_OBJS  = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_SGBMApp))
SHAVE_SGBMApp_OBJS += $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_SGBMApp))

SGBMApp_MainEntry = sgbm
costApp_MainEntry = computeCost