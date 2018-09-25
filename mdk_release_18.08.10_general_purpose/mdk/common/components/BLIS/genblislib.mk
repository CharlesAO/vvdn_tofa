#-------------------------------[ Local shave applications sources ]--------------------#
#Choosing C sources the BLIS application on shave
BLISTestApp = $(DirAppOutput)/BLISTest

MV_SOC_REV  ?= ma2150
LAMA_DIR     = LAMA
# ------------------------------[ Build overrides ]--------------------------------#
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155 ma2450 ma2455))
    MV_SOC_REV_DIR:=ma2x5x
else
    ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480))
	    CCOPT += -DCVMA2480
            MVCCOPT += -DCVMA2480
        MV_SOC_REV_DIR:=ma2x8x
    else
        $(error "$(MV_SOC_REV) variant not supported by BLIS")
    endif
endif

KERNEL_LIB_DIR = $(MV_COMMON_BASE)/components/kernelLib
SHAVE_C_SOURCES_BLISTest = $(wildcard $(DirAppRoot)/shave/*.c)
SHAVE_C_SOURCES_BLISTest += $(wildcard $(MV_COMMON_BASE)/components/BLIS/shave/*.c)
SHAVE_CPP_SOURCES_BLISTest = $(wildcard $(KERNEL_LIB_DIR)/$(LAMA_DIR)/kernels/*/shave/src/cpp/*.cpp)
SHAVE_APPS_DIR = $(wildcard $(KERNEL_LIB_DIR)/$(LAMA_DIR)/kernels/*/shave/include)
SHAVE_APPS_INCLUDE_DIR = $(patsubst %, -I%, $(SHAVE_APPS_DIR))

#Choosing ASM sources for the shave BLISTestApp app on shave
SHAVE_ASM_SOURCES_BLISTest = $(wildcard $(KERNEL_LIB_DIR)/$(LAMA_DIR)/kernels/*/arch/$(MV_SOC_REV_DIR)/shave/src/*.asm)

#Generating list of required generated assembly files for the BLISKernelTest app on shave
# BUILD_USE_GENASM
SHAVE_GENASMS_C_BLISTest = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_BLISTest))
SHAVE_GENASMS_CPP_BLISTest = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_BLISTest))

#Generating required objects list from sources
SHAVE_OBJS_BLISTest = $(patsubst %.asm, $(DirAppObjBase)%_shave.o, $(SHAVE_ASM_SOURCES_BLISTest)) \
                      $(patsubst %.asmgen,%_shave.o,$(SHAVE_GENASMS_C_BLISTest)) \
                      $(patsubst %.asmgen,%_shave.o,$(SHAVE_GENASMS_CPP_BLISTest))

#Update clean rules with our generated files
PROJECTCLEAN  += $(SHAVE_GENASMS_C_BLISTest) $(SHAVE_GENASMS_CPP_BLISTest) $(SHAVE_OBJS_BLISTest)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_BLISTest)

CCOPT   += -DBLIS -DVERSION -DBLIS_VERSION_STRING=\"$(VERS_STR)\" -DLEON_USE_REAL_NUMBERS_ONLY
CCOPT   += $(BLIS_LEON_HEADERS_I)

MVCCOPT +=-I$(MV_COMMON_BASE)/components/BLIS/shave
MVCCOPT +=-I$(MV_COMMON_BASE)/components/kernelLib/MvCV/include
MVCCOPT +=$(SHAVE_APPS_INCLUDE_DIR)

#-------------------------------[ Local leon applications sources ]--------------------#
BLIS_LEON_FILES_PATH = $(MV_COMMON_BASE)/components/BLIS/leon
#Choosing C sources the BLIS application on leon
BLIS_LEON_C_FILES = $(shell find $(BLIS_LEON_FILES_PATH)/ -type f -name '*.c')
BLIS_LEON_HEADERS = $(shell find $(BLIS_LEON_FILES_PATH)/ -type f -name '*.h')
BLIS_LEON_HEADERS_I = $(foreach includedir, $(sort $(dir $(BLIS_LEON_HEADERS))), -I$(includedir))

BLIS_OBJ_LEON = $(patsubst %.c,%.o,$(BLIS_LEON_C_FILES))
PROJECTCLEAN += $(BLIS_OBJ_LEON)

#-------------------------------[ Local shave applications build rules ]------------------#
$(BLIS_LEON_FILES_PATH)/%.o : $(BLIS_LEON_FILES_PATH)/%.c $(BLIS_LEON_HEADERS) Makefile
	@echo "Leon CC   : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c $(CONLY_OPT) $(CCOPT) $< -o $@

blis_lib : $(BLIS_OBJ_LEON) $(SHAVE_OBJS_BLISTest)
	mkdir -p $(dir $(BLIS_LIB_LEON))
	mkdir -p $(dir $(BLIS_LIB_SHAVE))
	$(ECHO) $(AR) rs $(BLIS_LIB_LEON) $(BLIS_OBJ_LEON)
	$(ECHO) $(AR) rs $(BLIS_LIB_SHAVE) $(SHAVE_OBJS_BLISTest)

ifeq ($(MV_SOC_OS), rtems)
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155 ma2450 ma2455))
      BLIS_LIB_LEON = $(MV_COMMON_BASE)/../packages/movidius/BLIS/rtems/arch/ma2x5x/blis_lib_leon.a
      BLIS_LIB_SHAVE = $(MV_COMMON_BASE)/../packages/movidius/BLIS/rtems/arch/ma2x5x/blis_lib_shave.a
else
    ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480))
      BLIS_LIB_LEON = $(MV_COMMON_BASE)/../packages/movidius/BLIS/rtems/arch/ma2x8x/blis_lib_leon.a
      BLIS_LIB_SHAVE = $(MV_COMMON_BASE)/../packages/movidius/BLIS/rtems/arch/ma2x8x/blis_lib_shave.a
    else
        $(error "$(MV_SOC_REV) variant not supported by BLIS")
    endif
endif
else
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2150 ma2155 ma2450 ma2455))
      BLIS_LIB_LEON = $(MV_COMMON_BASE)/../packages/movidius/BLIS/bm/arch/ma2x5x/blis_lib_leon.a
      BLIS_LIB_SHAVE = $(MV_COMMON_BASE)/../packages/movidius/BLIS/bm/arch/ma2x5x/blis_lib_shave.a
else
    ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480))
      BLIS_LIB_LEON = $(MV_COMMON_BASE)/../packages/movidius/BLIS/bm/arch/ma2x8x/blis_lib_leon.a
      BLIS_LIB_SHAVE = $(MV_COMMON_BASE)/../packages/movidius/BLIS/bm/arch/ma2x8x/blis_lib_shave.a
    else
        $(error "$(MV_SOC_REV) variant not supported by BLIS")
    endif
endif
endif

