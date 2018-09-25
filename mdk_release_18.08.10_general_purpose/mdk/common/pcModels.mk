ECHO?=@
SOC_REV?=ma2x8x

packages_base?=$(MV_COMMON_BASE)/../packages/movidius/pcModel
pcModel?=$(MV_COMMON_BASE)/swCommon/pcModel

libHwModules?=libHwModules

OS_Name:=$(shell cat /etc/*-release | grep '^ID=' | sed 's/ID=//;')

ifeq (fedora,$(OS_Name))
  # this works only on ubuntu by default:
  OS:=$(shell cat /etc/*-release | grep '^NAME' | sed 's/NAME=//;')
  VER:=$(shell cat /etc/*-release | grep '^VERSION_ID' | sed 's/VERSION_ID=//;')
endif
ifeq (ubuntu,$(OS_Name))
  # this works only on ubuntu by default:
  OS:=$(shell lsb_release -si)
  VER:=$(shell lsb_release -sr)
endif

ifneq ($(findstring $(firstword $(subst ., ,$(VER))),12 14 15 16 25),)
LINUX_VARIANT := $(OS)$(firstword $(subst ., ,$(VER)))
endif

ifeq ($(LINUX_VARIANT),"")
$(error "not suported Linux version")
endif

WARP?=warp/MODELS/tests/ts_warp_01
CNN_REF?=cnn/cnntest
STEREO_REF?=stereo/stereotest
minMax_REF?=minMax/minmaxtest
EDGE_REF?=sipp/MODELS/sipp_hw/tests/edgeoperatortest
MOTEST_REF?=motEst/MODELS/MotEstSW/build/libBuild

LIST_OF_API_NAMES := Warp Cnn MinMax Stereo Edge MotEst
hwBlocksPrefs =

define HwBlocksPrefsGen_template
hwBlocksPrefs += _$(1)X_
endef

$(foreach hwBlock,$(LIST_OF_API_NAMES),$(eval $(call HwBlocksPrefsGen_template,$(hwBlock))))

LdConf_CnnX_    = -m elf_i386
LdConf_MinMaxX_ = -m elf_i386
LdConf_StereoX_ = -m elf_i386
LdConf_WarpX_   = -m elf_x86_64
LdConf_EdgeX_   = -m elf_i386
LdConf_MotEstX_ = -m elf_i386

# define the output folders for each pcmodel that will be built for the common library
OBJ_FOLDER_CnnX_    = $(pcModel)/$(SOC_REV)/$(CNN_REF)/../Build/obj
OBJ_FOLDER_MinMaxX_ = $(pcModel)/$(SOC_REV)/$(minMax_REF)/obj
OBJ_FOLDER_StereoX_ = $(pcModel)/$(SOC_REV)/$(STEREO_REF)/obj
OBJ_FOLDER_WarpX_   = $(pcModel)/$(SOC_REV)/$(WARP)/obj
OBJ_FOLDER_EdgeX_   = $(pcModel)/$(SOC_REV)/$(EDGE_REF)/obj
OBJ_FOLDER_MotEstX_ = $(pcModel)/$(SOC_REV)/$(MOTEST_REF)/output

# collect all the object files resulting after the build of one test application for each of the pcmodels
OBJ_CnnX_    := $(wildcard $(OBJ_FOLDER_CnnX_)/*.o)
OBJ_MinMaxX_ := $(wildcard $(OBJ_FOLDER_MinMaxX_)/*.o)
OBJ_StereoX_ := $(wildcard $(OBJ_FOLDER_StereoX_)/*.o)
OBJ_WarpX_   := $(wildcard $(OBJ_FOLDER_WarpX_)/*.o)
OBJ_EdgeX_   := $(wildcard $(OBJ_FOLDER_EdgeX_)/*.o)
OBJ_MotEstX_ := $(wildcard $(OBJ_FOLDER_MotEstX_)/*.o)

# rename each object file to allow correct prefixing and storing in one common static library
RenOBJ_CnnX_    := $(patsubst %.o,%.o_CnnX_o,$(OBJ_CnnX_))
RenOBJ_MinMaxX_ := $(patsubst %.o,%.o_MinMaxX_o,$(OBJ_MinMaxX_))
RenOBJ_StereoX_ := $(patsubst %.o,%.o_StereoX_o,$(OBJ_StereoX_))
RenOBJ_WarpX_   := $(patsubst %.o,%.o_WarpX_o,$(OBJ_WarpX_))
RenOBJ_EdgeX_   := $(patsubst %.o,%.o_EdgeX_o,$(OBJ_EdgeX_))
RenOBJ_MotEstX_   := $(patsubst %.o,%.o_MotEstX_o,$(OBJ_MotEstX_))

LIB_PATH = $(packages_base)/$(SOC_REV)/libs/$(LINUX_VARIANT)LTS
LIB_TEMP_PATH = $(LIB_PATH)/temp

$(LIB_TEMP_PATH):
	$(ECHO)mkdir -pv $(LIB_TEMP_PATH)

#define what needs to be done for each of the libraryes and the symbols inside them
#also added as a dependency the .o files and if those change then this needs to be rebuilt
define PcModelLibCreate_template
$$(LIB_TEMP_PATH)/$(1)$$(libHwModules).o: $$(OBJ$(1))
#create a copy of each compiled object file and rename them
	$(ECHO)echo "======  Creating a renamed copy of each object file for $(1) ===="
	$(ECHO)$$(foreach file,$$(OBJ$(1)),cp -fu $$(file) $$(file)$(1)o;)
#create one library for each of the pcmodels
	$(ECHO)echo "======  Pack each object file in to a static library stored: $$(LIB_PATH)/$(1)$$(libHwModules).o   ===="
	$(ECHO)ld $$(LdConf$(1)) -r -o $$(LIB_TEMP_PATH)/$(1)$$(libHwModules).o $$(RenOBJ$(1))
#create a copy of the library where we will prefix all the symbols
	$(ECHO)echo "-----  Creating a copy of the library and prefixing all symbols with $(1) ---"
	$(ECHO)cp $$(LIB_TEMP_PATH)/$(1)$$(libHwModules).o $$(LIB_PATH)/$(1)$$(libHwModules)_prefixed
	$(ECHO)objcopy --prefix-symbols=$(1) $$(LIB_PATH)/$(1)$$(libHwModules)_prefixed
# create list of prefixed symbols
	$(ECHO)echo "-----  Creating list of already prefixed symbols that are defined in the library to get a list of prefixed symbol names ---"
	$(ECHO)nm --defined-only $$(LIB_PATH)/$(1)$$(libHwModules)_prefixed > $$(LIB_PATH)/$(1)$$(libHwModules).prefixed_sym
	$(ECHO)echo "-----  Removing prefixed library -----"
	$(ECHO)rm -f $$(LIB_PATH)/$(1)$$(libHwModules)_prefixed
# create list of symbols to prefix
	$(ECHO)echo "-----  Creating list of symbols that are defined in the library and need to be prefixed ---"
	$(ECHO)nm --defined-only $$(LIB_TEMP_PATH)/$(1)$$(libHwModules).o > $$(LIB_PATH)/$(1)$$(libHwModules).defined_sym
# strip filenames from list
	$(ECHO)sed -i -r '/^[^ ]+.o/D' $$(LIB_PATH)/$(1)$$(libHwModules).defined_sym
# strip empty lines from list
	$(ECHO)sed -i -r '/^\s*$$$$/D' $$(LIB_PATH)/$(1)$$(libHwModules).defined_sym
# strip address and other chars from before the symbols
	$(ECHO)sed -i -r 's/^[^ ]*..[ ]//' $$(LIB_PATH)/$(1)$$(libHwModules).defined_sym
# sort list and remove duplicates
	$(ECHO)sort -u -o $$(LIB_PATH)/$(1)$$(libHwModules).sort_defined_sym $$(LIB_PATH)/$(1)$$(libHwModules).defined_sym
# strip filenames from list
	$(ECHO)sed -i -r '/^[^ ]+.o/D' $$(LIB_PATH)/$(1)$$(libHwModules).prefixed_sym
# strip empty lines from list
	$(ECHO)sed -i -r '/^\s*$$$$/D' $$(LIB_PATH)/$(1)$$(libHwModules).prefixed_sym
# strip address and other chars from before the symbols and add some space
	$(ECHO)sed -i -r 's/^[^ ]*..[ ]/    /' $$(LIB_PATH)/$(1)$$(libHwModules).prefixed_sym
# sort list and remove duplicates
	$(ECHO)sort -u -o $$(LIB_PATH)/$(1)$$(libHwModules).sort_prefixed_sym $$(LIB_PATH)/$(1)$$(libHwModules).prefixed_sym
# create a list that stores each symbol to be renamed and then next to it the renamed name
	$(ECHO)echo "-----  Creating symbol redefine list for objcopy ---"
	$(ECHO)paste $$(LIB_PATH)/$(1)$$(libHwModules).sort_defined_sym $$(LIB_PATH)/$(1)$$(libHwModules).sort_prefixed_sym > $$(LIB_PATH)/$(1)$$(libHwModules).sym
# rename the internal symbols in the static library of the pcmodel
	$(ECHO)echo "-----  Redefine symbols from the list prepared above ---"
	$(ECHO)objcopy --redefine-syms=$$(LIB_PATH)/$(1)$$(libHwModules).sym $$(LIB_TEMP_PATH)/$(1)$$(libHwModules).o
	$(ECHO)echo "-------------  Removing obsolete symbol lists generated -------------"
	$(ECHO)rm $$(LIB_PATH)/$(1)$$(libHwModules).sort_defined_sym $$(LIB_PATH)/$(1)$$(libHwModules).sort_prefixed_sym $$(LIB_PATH)/$(1)$$(libHwModules).prefixed_sym $$(LIB_PATH)/$(1)$$(libHwModules).defined_sym

$(1): $$(LIB_TEMP_PATH) $(LIB_TEMP_PATH)/$(1)$$(libHwModules).o
# nothing to do
endef

$(foreach block,$(hwBlocksPrefs),$(eval $(call PcModelLibCreate_template,$(block))))

CleanPrefixedObjFiles:
	$(ECHO)echo " ==== Removing CNN renamed object files ==="
	$(ECHO)rm -f -v $(OBJ_FOLDER_CnnX_)/*.*_o
	$(ECHO)echo " ==== Removing MinMax renamed object files ==="
	$(ECHO)rm -f -v $(OBJ_FOLDER_MinMaxX_)/*.*_o
	$(ECHO)echo " ==== Removing stereo renamed object files ==="
	$(ECHO)rm -f -v $(OBJ_FOLDER_StereoX_)/*.*_o
	$(ECHO)echo " ==== Removing warp renamed object files ==="
	$(ECHO)rm -f -v $(OBJ_FOLDER_WarpX_)/*.*_o
	$(ECHO)echo " ==== Removing edge renamed object files ==="
	$(ECHO)rm -f -v $(OBJ_FOLDER_EdgeX_)/*.*_o
	$(ECHO)echo " ==== Removing motest renamed object files ==="
	$(ECHO)rm -f -v $(OBJ_FOLDER_MotEstX_)/*.*_o

#for each PCmodel implementation of APIs we need to build the api in such way that
#it links against the prefixed library but it's symbols are not prefixed so that
#it may be used by developers as in the header files

createPcModels: $(LIB_TEMP_PATH) buildtestapps
ifeq ("$(wildcard $(packages_base)/$(SOC_REV))","")
	$(ECHO)mkdir $(packages_base)/$(SOC_REV)
	$(ECHO)mkdir $(LIB_PATH)
	$(ECHO)mkdir $(LIB_TEMP_PATH)
	$(ECHO)mkdir $(packages_base)/$(SOC_REV)/includes
endif
	$(ECHO)echo "+++++++++++ Cleaning old prefixed libraries +++++++++"
	$(ECHO)make CleanPrefixedObjFiles
	$(ECHO)echo "+++++++++++++++ Creating the new library  +++++++++++"
	$(ECHO)make createPcModelsLib
	$(ECHO)echo "========>>>>   Size libHwModules   <<<=========="
	$(ECHO)stat $(LIB_PATH)/$(libHwModules).a

#build warp cnn stereo edge harris minMax
buildtestapps:
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      START BUILDING >>>  CNN  <<<<      !!!!!"
	$(ECHO)make all -j4 -C $(pcModel)/$(SOC_REV)/$(CNN_REF)
	$(ECHO)echo "=============   finished Building CNN     ==============="
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      START BUILDING >>>  WARP  <<<<      !!!!!"
	$(ECHO)make all -j4 -C $(pcModel)/$(SOC_REV)/$(WARP)
	$(ECHO)echo "=============   finished Building WARP     ==============="
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      START BUILDING >>>  STEREO  <<<<      !!!!!"
	$(ECHO)make all -j4 -C $(pcModel)/$(SOC_REV)/$(STEREO_REF)
	$(ECHO)echo "=============   finished Building STEREO     ==============="
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      START BUILDING >>>  EDGE  <<<<      !!!!!"
	$(ECHO)make all -j4 -C $(pcModel)/$(SOC_REV)/$(EDGE_REF)
	$(ECHO)echo "=============   finished Building EDGE     ==============="
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      START BUILDING >>>  MOTEST  <<<<      !!!!!"
	$(ECHO)make all -j4 -C $(pcModel)/$(SOC_REV)/$(MOTEST_REF)
	$(ECHO)echo "=============   finished Building MOTEST     ==============="
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      START BUILDING >>>  MIN MAX  <<<<      !!!!!"
	$(ECHO)make all -j4 -C $(pcModel)/$(SOC_REV)/$(minMax_REF)
	$(ECHO)echo "=============   finished Building MIN MAX     ==============="

cleantestapps:
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      cleaning >>>  CNN  <<<<      !!!!!"
	$(ECHO)make clean -j4 -C $(pcModel)/$(SOC_REV)/$(CNN_REF)
	$(ECHO)echo "=============   finished cleaning CNN     ==============="
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      cleaning >>>  WARP  <<<<      !!!!!"
	$(ECHO)make clean -j4 -C $(pcModel)/$(SOC_REV)/$(WARP)
	$(ECHO)echo "=============   finished cleaning WARP     ==============="
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      cleaning >>>  STEREO  <<<<      !!!!!"
	$(ECHO)make clean -j4 -C $(pcModel)/$(SOC_REV)/$(STEREO_REF)
	$(ECHO)echo "=============   finished cleaning STEREO     ==============="
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      cleaning >>>  EDGE  <<<<      !!!!!"
	$(ECHO)make clean -j4 -C $(pcModel)/$(SOC_REV)/$(EDGE_REF)
	$(ECHO)echo "=============   finished cleaning EDGE     ==============="
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      cleaning >>>  MOTEST  <<<<      !!!!!"
	$(ECHO)make clean -j4 -C $(pcModel)/$(SOC_REV)/$(MOTEST_REF)
	$(ECHO)echo "=============   finished cleaning MOTEST     ==============="
	$(ECHO)echo " "
	$(ECHO)echo "!!!!!      cleaning >>>  MIN MAX  <<<<      !!!!!"
	$(ECHO)make clean -j4 -C $(pcModel)/$(SOC_REV)/$(minMax_REF)
	$(ECHO)echo "=============   finished cleaning MIN MAX     ==============="

cleanPcModels: cleantestapps cleanPcModelLib
	$(ECHO)echo "===> cleaning pcmodels"
	$(ECHO)rm -rf $(RenOBJ_CnnX_) $(RenOBJ_MinMaxX_) $(RenOBJ_StereoX_) $(RenOBJ_WarpX_) $(RenOBJ_EdgeX_) $(RenOBJ_MotEstX_)

cleanPcModelLib:
	$(ECHO)echo "Deleting $(LIB_PATH)/$(libHwModules)* pls wait..."
	$(ECHO)if [ -d $(LIB_PATH) ]; then rm -rf -v $(LIB_PATH)/*; fi
	$(ECHO)if [ -d $(packages_base)/$(SOC_REV)/includes ]; then rm -rf -v $(packages_base)/$(SOC_REV)/includes/*; fi

createPcModelsLib: $(LIB_TEMP_PATH) $(hwBlocksPrefs)
	$(ECHO)echo "_________________[ Building library ]__________________"
	$(ECHO)echo "    Packing prefixed object files in static library "
	$(ECHO)echo "    list of files:  $(wildcard $(LIB_TEMP_PATH)/*.o)  "
	$(ECHO) if [ -d $(LIB_PATH) ]; then rm -rfv $(LIB_PATH)/$(libHwModules).a; fi
	$(ECHO)ar -rcs $(LIB_PATH)/$(libHwModules).a $(wildcard $(LIB_TEMP_PATH)/*.o)
	$(ECHO)echo "____________[ Finished building library ]______________"

print1-%  : ; @echo $* = $($*)
