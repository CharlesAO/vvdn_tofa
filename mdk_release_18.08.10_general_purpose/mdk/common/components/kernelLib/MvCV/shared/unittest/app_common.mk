# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

DirAppRoot           = ../../../../shared/unittest
DirAppOutput         = ./output

MV_COMMON_BASE		?= ../../../../../../../../common
MVCV_PATH			 = $(MV_COMMON_BASE)/components/kernelLib/MvCV
MVCV_OUTPUT_PATH	 = shared/unittest/$(DirAppOutput)

MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2150

# ------------------------------[ Build overrides ]--------------------------------#
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155 ma2450 ma2455))
    MV_SOC_REV_DIR:=ma2x5x
else
    ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480))
	    CCOPT += -DCVMA2480 
        MV_SOC_REV_DIR:=ma2x8x
    else
        $(error "$(MV_SOC_REV) variant not supported by unittesting")
    endif
endif


# overrides that need to be set before including generic.mk
ifeq ($(MV_SOC_PLATFORM), myriad2)
ifeq ($(INT_DDR), yes)
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480))
LinkerScript ?= $(DirAppRoot)/config/custom_ma2480_ddr.ldscript
CCOPT += -DINT_DDR
else
LinkerScript ?= $(DirAppRoot)/config/custom_ddr.ldscript
CCOPT += -DINT_DDR
endif
else
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480))
LinkerScript ?= $(DirAppRoot)/config/custom_ma2480_cmx.ldscript
else
LinkerScript ?= $(DirAppRoot)/config/custom_cmx.ldscript
endif
endif
ifeq ($(USE_SHAVE_L2_CACHE), yes)
CCOPT += -DUSE_SHAVE_L2_CACHE
endif
endif

MVASMOPT += -showWarnings:1

# ------------------------------[ Components used ]--------------------------------#
# Extra app related options
ifeq ($(MV_SOC_PLATFORM), myriad2)
ComponentList_LOS  += UnitTestVcs VcsHooks
ComponentList_LRT  += UnitTestVcs VcsHooks
ComponentList_SVE += UnitTestVcs VcsHooks
endif

LEON_COMPONENT_PATHS 			+= ./leon
LEON_COMPONENT_HEADERS_PATHS 	+= ./leon
LEON_COMPONENT_HEADERS_PATHS 	+= ./shared

#-------------------------------[ Local shave applications sources ]--------------------#
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = yes

#-------------------------------[ Local shave applications sources ]--------------------#

#-------------------------------[ MvCV Component ] -------------------------------------#
DirMvCVOutput 		= $(MVCV_PATH)/$(MVCV_OUTPUT_PATH)
DirMvCVObj 			= $(DirMvCVOutput)/objects/
DirMvCVShaveShared 	= $(MVCV_PATH)/shave
DirMvCVInclude  	= $(MVCV_PATH)/include
DirMvCVKernels 		= $(MVCV_PATH)/kernels

#Add MVCV include directories to Components include
SHAVE_COMPONENT_INCLUDE_PATHS += $(DirMvCVInclude)
SHAVE_COMPONENT_INCLUDE_PATHS += ./shared

SH_MVCV_HEADERS_PATH += $(wildcard $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/*/shave/include)

#Add MVCV include directories to Components include
SHAVE_COMPONENT_INCLUDE_PATHS += $(SH_MVCV_HEADERS_PATH)

# Get all kernels
ALL_KERNELS = $(notdir $(wildcard $(DirMvCVKernels)/*))

# Exclude unwanted kernels
EXCLUDED_KERNELS = f32UnitTestSample
KERNELS = $(filter-out $(EXCLUDED_KERNELS), $(ALL_KERNELS))

# Directory path for CPP code
SH_MVCV_PATHS_CPP 		+=  $(patsubst %,%/shave/src/cpp,$(KERNELS))
# Directory path for chip agnostic asm code
SH_MVCV_PATHS_ASM 		+=  $(patsubst %,%/shave/src,$(KERNELS))
# Directory path for chip specific asm code
SH_MVCV_PATHS_ARCH_ASM 	+=  $(patsubst %,%/arch/$(MV_SOC_REV_DIR)/shave/src,$(KERNELS))

SHAVE_MVCV_HEADERS 			= $(sort $(wildcard $(patsubst %,%/*.h,$(SH_MVCV_HEADERS_PATH))))
SHAVE_MVCV_CPP_SOURCES 		= $(sort $(wildcard $(patsubst %,$(DirMvCVKernels)/%/*.cpp,$(SH_MVCV_PATHS_CPP))))
SHAVE_MVCV_ASM_SOURCES 		= $(sort $(wildcard $(patsubst %,$(DirMvCVKernels)/%/*.asm,$(SH_MVCV_PATHS_ASM))))
SHAVE_MVCV_ARCH_ASM_SOURCES = $(sort $(wildcard $(patsubst %,$(DirMvCVKernels)/%/*.asm,$(SH_MVCV_PATHS_ARCH_ASM))))
SHAVE_MVCV_GENASMS 			= $(patsubst $(DirMvCVKernels)/%,$(DirMvCVObj)%, $(patsubst %.cpp,%.asmgen, $(SHAVE_MVCV_CPP_SOURCES)))

SHAVE_MVCV_OBJS  = $(patsubst $(DirMvCVKernels)/%,$(DirMvCVObj)%,$(patsubst %.asm, %_asm.o,$(SHAVE_MVCV_ASM_SOURCES)))
SHAVE_MVCV_OBJS += $(patsubst $(DirMvCVKernels)/%,$(DirMvCVObj)%,$(patsubst %.asm, %_asm_$(MV_SOC_REV).o,$(SHAVE_MVCV_ARCH_ASM_SOURCES)))
SHAVE_MVCV_OBJS += $(patsubst %.asmgen,%_shave.o, $(SHAVE_MVCV_GENASMS))

include $(MV_COMMON_BASE)/resolvetargetpath.mk

.SECONDEXPANSION:
$(DirMvCVObj)%_asm.o: $(DirMvCVKernels)/%.asm $(DirMvCVKernels)/$$(word 1, $$(subst /, , %))/shave/include/*.h
	@echo "Shave ASM : $<"
	@test -d $(dir $(DirMvCVObj)$*) || mkdir -p $(dir $(DirMvCVObj)$*)
	$(ECHO) $(MVASM) $(MVASMOPT)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

$(DirMvCVObj)%_asm_$(MV_SOC_REV).o: $(DirMvCVKernels)/%.asm $(DirMvCVKernels)/$$(word 1, $$(subst /, , %))/shave/include/*.h
	@echo "Shave ASM : $<"
	@test -d $(dir $(DirMvCVObj)$*) || mkdir -p $(dir $(DirMvCVObj)$*)
	$(ECHO) $(MVASM) $(MVASMOPT)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

$(DirMvCVObj)%_shave.o: $(DirMvCVObj)%.asmgen
	@echo "Shave ASM : $<"
	@test -d $(dir $(DirMvCVObj)$*) || mkdir -p $(dir $(DirMvCVObj)$*)
	$(ECHO) $(MVASM) $(MVASMOPT)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

$(DirMvCVObj)%.asmgen: $(DirMvCVKernels)/%.cpp $(DirMvCVKernels)/$$(word 1, $$(subst /, , %))/shave/include/*.h
	@echo "Shave CPP : $<"
	@test -d $(dir $(DirMvCVObj)$*) || mkdir -p $(dir $(DirMvCVObj)$*)
	$(ECHO) $(MVCC) $(MVCCOPT) -I $(DirMvCVKernels)/$*/shave/include $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

$(DirMvCVObj)shared/mvcv_types_shave.o: $(DirMvCVObj)shared/mvcv_types.asmgen
	@echo "Shave ASM : $<"
	@test -d $(DirMvCVObj)shared || mkdir -p $(DirMvCVObj)shared
	$(ECHO) $(MVASM) $(MVASMOPT)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

$(DirMvCVObj)shared/mvcv_types.asmgen: $(DirMvCVShaveShared)/mvcv_types.cpp $(DirMvCVInclude)/mvcv_types.h
	@echo "Shave CPP : $<"
	@test -d $(DirMvCVObj)shared || mkdir -p $(DirMvCVObj)shared
	$(ECHO) $(MVCC) $(MVCCOPT) -I $(DirMvCVInclude) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)


$(DirMvCVObj)f32UnitTestSample/f32UnitTestSample_shave.o: $(DirMvCVObj)f32UnitTestSample/f32UnitTestSample.asmgen
	@echo "Shave ASM : $<"
	@test -d $(DirMvCVObj)f32UnitTestSample || mkdir -p $(DirMvCVObj)f32UnitTestSample
	$(ECHO) $(MVASM) $(MVASMOPT)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

$(DirMvCVObj)f32UnitTestSample/f32UnitTestSample.asmgen: $(DirMvCVKernels)/f32UnitTestSample/shave/src/cpp/f32UnitTestSample.cpp $(DirMvCVKernels)/f32UnitTestSample/shave/include/f32UnitTestSample.h
	@echo "Shave CPP : $<"
	@test -d $(DirMvCVObj)f32UnitTestSample || mkdir -p $(DirMvCVObj)f32UnitTestSample
	$(ECHO) $(MVCC) $(MVCCOPT) -I $(DirMvCVKernels)/f32UnitTestSample/shave/include $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

$(DirMvCVObj)f32UnitTestSample/mvuDotProductStreaming_shave.o: $(DirMvCVObj)f32UnitTestSample/mvuDotProductStreaming.asmgen
	@echo "Shave ASM : $<"
	@test -d $(DirMvCVObj)f32UnitTestSample || mkdir -p $(DirMvCVObj)f32UnitTestSample
	$(ECHO) $(MVASM) $(MVASMOPT)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

$(DirMvCVObj)f32UnitTestSample/mvuDotProductStreaming.asmgen: $(DirMvCVKernels)/f32UnitTestSample/shave/src/cpp/referenceCode.cpp $(DirMvCVKernels)/f32UnitTestSample/shave/include/*.h Makefile
	@echo "Shave CPP : $<"
	@test -d $(DirMvCVObj)f32UnitTestSample || mkdir -p $(DirMvCVObj)f32UnitTestSample
	$(ECHO) $(MVCC) $(MVCCOPT) -I $(DirMvCVKernels)/f32UnitTestSample/shave/include $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

$(DirMvCVObj)shared/mat_shave.o: $(DirMvCVObj)shared/mat.asmgen
	@echo "Shave ASM : $<"
	@test -d $(DirMvCVObj)shared || mkdir -p $(DirMvCVObj)shared
	$(ECHO) $(MVASM) $(MVASMOPT)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

$(DirMvCVObj)shared/mat.asmgen: $(DirMvCVShaveShared)/mat.cpp $(DirMvCVInclude)/mat.h $(DirMvCVInclude)/mvcv_types.h
	@echo "Shave CPP : $<"
	@test -d $(DirMvCVObj)shared || mkdir -p $(DirMvCVObj)shared
	$(ECHO) $(MVCC) $(MVCCOPT) -I $(DirMvCVInclude) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)


#Choosing C sources the dummy application on shave
DummyApp = shave/dummy
SHAVE_CPP_SOURCES_dummy = $(wildcard $ ./shave/*.cpp)

VCS_CPP = $(wildcard $ ./../../../../shared/unittest/shave/*.cpp)
VCS_GENASMS = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(VCS_CPP))
VCS_OBJ = $(patsubst %.asmgen,%_shave.o,$(VCS_GENASMS))

#Choosing ASM sources for the shave dummy app on shave
#no asm files
#SHAVE_ASM_SOURCES_dummy =

#Generating list of required generated assembly files for the dummy app on shave
SHAVE_GENASMS_dummy = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_dummy))
#Generating required objects list from sources
SHAVE_dummy_OBJ = $(patsubst %.asmgen,%_shave.o,$(SHAVE_GENASMS_dummy)) \

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_dummy) $(SHAVE_dummy_OBJ)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_dummy) $(SHAVE_MVCV_GENASMS)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(DummyApp).mvlib

SHAVE0_APPS = $(DummyApp).shv0lib
ifeq ($(VCS_TEST_BUILD), true)
	SHAVE8_APPS = vcstest.shv8lib
endif
# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =00.50.23.0

# Verbose or not, uncomment for more verbosity
#ECHO=

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

MvCVLib = $(DirMvCVOutput)/mvcv.a
MvCVLibRev = $(DirMvCVOutput)/$(MV_SOC_REV)

ENTRYPOINTS = -e _EP_start -u main -u StartAddrPcBoardLength -u savedElf -u nrOfCheckVariable -u pcExpectedOutput -u pcExpectedOutputPointer --gc-sections

#$(SHAVE_MVCV_CPP_SOURCES) $(SHAVE_MVCV_ASM_SOURCES)
$(MvCVLib) : $(SHAVE_MVCV_HEADERS) $(DirMvCVObj)shared/mvcv_types_shave.o $(DirMvCVObj)shared/mat_shave.o $(SHAVE_MVCV_OBJS) $(DirMvCVObj)f32UnitTestSample/f32UnitTestSample_shave.o $(DirMvCVObj)f32UnitTestSample/mvuDotProductStreaming_shave.o Makefile
	@echo "Shave MvCV LIB   : $@"
	@mkdir -p $(dir $@)
	@touch $(MvCVLibRev)
	$(ECHO) $(AR) rs $@ $(DirMvCVObj)shared/mvcv_types_shave.o $(DirMvCVObj)shared/mat_shave.o $(SHAVE_MVCV_OBJS) $(DirMvCVObj)f32UnitTestSample/f32UnitTestSample_shave.o $(DirMvCVObj)f32UnitTestSample/mvuDotProductStreaming_shave.o

$(MvCVLibRev) : FORCE
	@mkdir -p $(dir $@)
	$(ECHO) if [ ! -f $(MvCVLibRev) ]; then if [ -d $(DirMvCVOutput) ]; then rm -rf $(DirMvCVOutput)/*; touch $(MvCVLibRev); else true; fi; fi
	$(MAKE) $(MvCVLib)

FORCE:

PROJECTCLEAN += $(SHAVE_COMPONENT_GENASMS)

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the dummy application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
$(DummyApp).mvlib : $(SHAVE_dummy_OBJ) $(PROJECT_SHAVE_LIBS) $(MvCVLibRev)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(SHAVE_dummy_OBJ) $(PROJECT_SHAVE_LIBS) $(MvCVLib) $(CompilerANSILibs) -o $@

vcstest.mvlib : $(VCS_OBJ)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(VCS_OBJ) -o $@

# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT			+= -DDEBUG

# The following is used to specify which platforms a given unit test is expected to build on
# The default expectation is that they work for myriad2
# In these cases the variable is overidden locally
BUILD_PLATFORMS                 ?="myriad2"
MDK_PROJ_STRING	= "MDK_FALSE"

getBuildPlatforms:
	@echo BUILD_PLATFORMS=$(BUILD_PLATFORMS)

MOVICOMPILE_OPTIMIZED =
ifeq ($(MOVICOMPILE_OPTIMIZED), true)
MVCCOPT        += -DMOVICOMPILE_OPTIMIZED
endif

UNIT_TEST_USE_C_VERSION =
ifeq ($(UNIT_TEST_USE_C_VERSION), true)
MVCCOPT        += -DUNIT_TEST_USE_C_VERSION
endif

ifeq ($(VCS_TEST_BUILD), true)
MVCCOPT        += -DVCS_TESTING_DUMP_ELF
CCOPT          += -DVCS_TESTING_DUMP_ELF
endif

clean_lib :
	@echo "Cleaning MvCV lib"
	$(call SAFE_RM,$(DirMvCVOutput))
