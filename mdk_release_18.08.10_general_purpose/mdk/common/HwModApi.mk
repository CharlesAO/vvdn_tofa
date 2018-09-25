
include $(MV_COMMON_BASE)/pcModels.mk

hwModelApiBinDir = $(MV_COMMON_BASE)/components/HwModInt/pcModels/binary
hwModelsApiSrcDir = $(MV_COMMON_BASE)/components/HwModInt/pcModels/src

pcmodel_Cnn_INCFLAGS ?=	-I$(MV_COMMON_BASE)/components/HwModInt/pcModels/inc/                           \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/cnn/                                       \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/cnn/External/sippHwIncludes/               \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/cnn/External/moviToolsIncludes/            \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/cnn/Include                                \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/cnn/External/sippTestUtils/                \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/cnn/External/sippHwIncludes/sippVisitors/

pcmodel_MinMax_INCFLAGS ?= -I$(MV_COMMON_BASE)/components/HwModInt/pcModels/inc/ \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/minMax/Includes/                                  \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/minMax/External/sippTestUtils/                   \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/minMax/External/sippHwIncludes/                  \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/minMax/External/sippHwIncludes/sippDecorators/   \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/minMax/External/sippHwIncludes/sippVisitors/     \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/minMax/External/sippHwIncludes/sippStrategies/   \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/minMax/External/moviToolsIncludes/


pcmodel_Stereo_INCFLAGS ?= 	-I$(MV_COMMON_BASE)/components/HwModInt/pcModels/inc/ \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/stereo/Include/                                  \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/stereo/External/sippTestUtils/                   \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/stereo/External/sippHwIncludes/                  \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/stereo/External/sippHwIncludes/sippDecorators/   \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/stereo/External/sippHwIncludes/sippVisitors/     \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/stereo/External/sippHwIncludes/sippStrategies/   \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/stereo/External/moviToolsIncludes/

pcmodel_Warp_INCFLAGS ?=  \
		-I$(pcModel)/ma2x8x/warp/MODELS/src \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/ \
		-I$(MV_COMMON_BASE)/components/HwModInt/pcModels/inc \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/sippDecorators/ \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/sippBuilders/ \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/sippStrategies/ \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/sippVisitors/ \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/testUtils/ \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/external/moviToolsCommonInc/

pcmodel_Edge_INCFLAGS ?= -I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/ \
		-I$(MV_COMMON_BASE)/components/HwModInt/pcModels/inc \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/sippDecorators/ \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/sippBuilders/ \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/sippStrategies/ \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/sippVisitors/ \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/testUtils/ \
 		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/external/moviToolsCommonInc/


pcmodel_MotEst_INCFLAGS ?= 	-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/ \
		-I$(MV_COMMON_BASE)/components/HwModInt/pcModels/inc \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include/sippVisitors      \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/sipp/MODELS/sipp_hw/include                   \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/ma2x8x/motEst/MODELS/MotEstSW/src


Cnn_Api_CPP_FLAGS ?= -std=c++0x -Wall -O0 -g -fopenmp -fpermissive -Wno-unknown-pragmas -Wno-comment -Wno-reorder -m32
MinMax_Api_CPP_FLAGS ?= -std=c++0x -Wall -O0 -g -fopenmp -fpermissive -Wno-unknown-pragmas -Wno-comment -Wno-reorder -m32
Stereo_Api_CPP_FLAGS ?= -std=c++0x -Wall -O0 -g -fopenmp -fpermissive -Wno-unknown-pragmas -Wno-comment -Wno-reorder -Wmissing-include-dirs -m32
Warp_Api_CPP_FLAGS ?= -std=c++0x -Wall -O0 -g -fopenmp -fpermissive -Wno-unknown-pragmas -Wno-comment -Wno-reorder
Edge_Api_CPP_FLAGS ?= -std=c++0x -Wall -O0 -g -fopenmp -fpermissive -Wno-unknown-pragmas -Wno-comment -Wno-reorder -m32
MotEst_Api_CPP_FLAGS ?=  -O2 -g -m32

Cnn_Api_LD_FLAGS ?= -m elf_i386
MinMax_Api_LD_FLAGS ?= -m elf_i386
Stereo_Api_LD_FLAGS ?=  -m elf_i386
Warp_Api_LD_FLAGS ?= -m elf_x86_64
Edge_Api_LD_FLAGS ?= -m elf_i386
MotEst_Api_LD_FLAGS ?= -m elf_i386

Cnn_Api_CPP ?= g++
MinMax_Api_CPP ?= g++
Stereo_Api_CPP ?= g++
Warp_Api_CPP ?= g++
Edge_Api_CPP ?= g++
MotEst_Api_CPP ?= g++

Cnn_Api_LD ?= ld
MinMax_Api_LD ?= ld
Stereo_Api_LD ?= ld
Warp_Api_LD ?= ld
Edge_Api_LD ?= ld
MotEst_Api_LD ?= ld

LIST_OF_API_NAMES ?= Warp Cnn Stereo Edge MinMax MotEst

hwModelsApiObjects :=
hwModelsApiLibs :=

define ApiBuild_template
hwModelsApiLibs += $$(LIB_PATH)/$$(libHwModules)$(1).o

$$(LIB_PATH)/$$(libHwModules)$(1).o: $$(hwModelApiBinDir)/hw$(1)Api.o
	@echo " ===== Linking $(1)Api object file toghether"
	$(ECHO)rm -fv $$(LIB_PATH)/$$(libHwModules)$(1).o
	$(ECHO)$$($(1)_Api_LD) $$($(1)_Api_LD_FLAGS) -r -o $$@ $$< $$(LIB_PATH)/$$(libHwModules).a
	@echo " ===== finished creating $$(LIB_PATH)/$$(libHwModules)$(1).o"

hwModelsApiObjects += $$(hwModelApiBinDir)/hw$(1)Api.o

$$(hwModelApiBinDir)/hw$(1)Api.o : $$(hwModelsApiSrcDir)/hw$(1)Api.cpp
	$(ECHO)mkdir -p $$(hwModelApiBinDir)
	@echo "==== Building $(1)Api...."
	rm -fv $$(hwModelApiBinDir)/hw$(1)Api.o
	$(ECHO)$$($(1)_Api_CPP) $$(pcmodel_$(1)_INCFLAGS) $$($(1)_Api_CPP_FLAGS) -c $$< -o $$@
	@echo "==== Dumping localy defined symbols list"
	$(ECHO)nm --defined-only $$@ > $$@_defined_sym
	@echo "... strip filenames from list"
	$(ECHO)sed -i -r '/^[^ ]+.o/D' $$@_defined_sym
	@echo "... strip empty lines from list"
	$(ECHO)sed -i -r '/^\s*$$$$/D' $$@_defined_sym
	@echo "... strip address and other chars from before the symbols"
	$(ECHO)sed -i -r 's/^[^ ]*..[ ]//' $$@_defined_sym
	@echo "... sort list and remove duplicates"
	$(ECHO)sort -u -o $$@_sort_defined_sym $$@_defined_sym
	$(ECHO)rm -fv $$@_defined_sym
	@echo "... create a local copy of the _$(1)X_libHwModules.sym list"
	$(ECHO)cp -f $$(LIB_PATH)/_$(1)X_$$(libHwModules).sym $$@.sym
	$(ECHO)make hw$(1)Api.o_symbols $(1)_API_DEF_SYM_LIST=$$@_sort_defined_sym $(1)_API_SYM_LIST=$$@.sym $(1)_API_OBJ_FILE=$$@ ECHO=$$(ECHO)
	@echo "... Cleaning up garbage symbols files..."
	$(ECHO)rm -fv $$@_sort_defined_sym $$@.sym

hw$(1)Api.o_symbols:
	@echo "=====  Removing locally defined symbols from the copy of symbols to prefix, to prevent double symbol definitions:"
	@$(DEBUG_ECHO_SYMBOLS)
	$(ECHO)$$(foreach sym, $$(shell cat $$($(1)_API_DEF_SYM_LIST)), sed -i '/$$(sym)/D' $$($(1)_API_SYM_LIST); )
	@echo "=====  Prefixing symbols that are not locally defined and are part of the edge pcmodel"
	$(ECHO)objcopy --redefine-syms=$$($(1)_API_SYM_LIST) $$($(1)_API_OBJ_FILE)
endef

$(foreach block,$(LIST_OF_API_NAMES),$(eval $(call ApiBuild_template,$(block))))

$(LIB_PATH)/$(libHwModules)full.a: $(hwModelsApiLibs)
	@echo " ===== Linking Api object files toghether"
	$(ECHO)ar -rcs $@ $^

createHwModulesLib: $(LIB_PATH)/$(libHwModules)full.a
	@echo "done... building HwModels Api library"

cleanHwModelsApiLibs:
	@$(call SAFE_RM, $(LIB_PATH)/$(libHwModules)full.a)
	@$(call SAFE_RM, $(hwModelsApiLibs))
	@$(call SAFE_RM, $(hwModelsApiObjects))

clean_all: cleanHwModelsApiLibs

# Target to print the content of a variable
print-%  : ; @echo $* = $($*)
