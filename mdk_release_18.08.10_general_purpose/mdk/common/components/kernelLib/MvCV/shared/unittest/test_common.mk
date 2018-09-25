#
# MvCV unittest structure:
#
# kernel -> include test_common.mk (this file)
# test_common.mk -> MDK/regression/unittest/unittest.mk
#    - invokes make all on 'dummy' subdir of the unittest
#
ifndef APPNAME
APPNAME := $(basename $(notdir $(abspath $(CURDIR)/../)))
endif
MDK_BASE ?= ../../../../../../..
MV_EXTRA_DATA = $(MV_COMMON_BASE)/../../mv_extra_data
UNIT_TEST_DIR ?= $(MDK_BASE)/regression/unittest

KERNEL_APP_NAME = dummy
KERNEL_APP_PATH = dummy
TARGET_PREFIX = dummy

CPPFLAGS += -D"TOLERANCE_MVCV_CC=15"
#CPPFLAGS += -D VCS_TESTING_DUMP_ELF
CPPFLAGS += -Wno-multichar

# shave prefix used to create shave symbol names
TEST_NAME  = $(APPNAME)_unittest

# Extra app related options
VCS_TEST_BUILD ?=
ifeq ($(VCS_TEST_BUILD), true)
CPPFLAGS         += -D VCS_TESTING_DUMP_ELF
endif

ifeq ($(USES_OPENCV), yes)
    ifndef OPENCV_INCLUDE_DIR
        $(error The project is marked as using OpenCV but OPENCV_INCLUDE_DIR is not defined)
    endif
    ifndef OPENCV_LIB_DIR
        $(error The project is marked as using OpenCV but OPENCV_LIB_DIR is not defined)
    endif

	# Add OpenCV related flags
	CPPFLAGS += -I$(OPENCV_INCLUDE_DIR)/opencv
	CPPFLAGS += -I$(OPENCV_INCLUDE_DIR)/opencv2
	CPPFLAGS += -I$(OPENCV_INCLUDE_DIR)
	CPPFLAGS += -L$(OPENCV_LIB_DIR) -lopencv_calib3d -lopencv_imgproc -lopencv_contrib -lopencv_legacy \
				-lopencv_core -lopencv_ml -lopencv_features2d -lopencv_objdetect -lopencv_flann \
				-lopencv_video -lopencv_highgui
else
	# Exclude files that use OpenCV
	FILES_TO_EXCLUDE = debug.cpp yuv.cpp
	HELPERS_SOURCES = $(filter-out $(patsubst %,$(HELPERS_SRC_DIR)/%,$(FILES_TO_EXCLUDE)), $(wildcard $(HELPERS_SRC_DIR)/*.cpp))
endif

EXTRA_LIBS += $(MV_CV_OBJ)
MVCV_DIR   = $(MV_COMMON_BASE)/components/kernelLib/MvCV
#MDK_KERNELS_DIR = $(MDK_BASE)/common/components/MvCV/kernels
MVCV_APPS_DIR = $(wildcard $(MVCV_DIR)/kernels/*/shave/include)
MVCV_APPS_HEADERS = $(patsubst %, %/*.h, $(MVCV_APPS_DIR))
MVCV_APPS_INCLUDE_DIR = $(patsubst %, -I%, $(MVCV_APPS_DIR))
MVCV_INCLUDE_DIR = $(MVCV_DIR)/include
MVCV_SHAVE_DIR   = $(MVCV_DIR)/kernels/$(APPNAME)/shave/src/cpp

CPPFLAGS += -I$(MVCV_INCLUDE_DIR)
CPPFLAGS += $(MVCV_APPS_INCLUDE_DIR)
ALL_MV_CV_LIST = $(wildcard $(MVCV_SHAVE_DIR)/*.cpp)

EXCLUDED_KERNELS = harris.cpp mvcv.cpp referenceCode.cpp
EXCLUDE_LIST = $(patsubst %.cpp, $(MVCV_SHAVE_DIR)/%.cpp, $(EXCLUDED_KERNELS))
MV_CV_KERNELS = $(filter-out $(EXCLUDE_LIST),$(ALL_MV_CV_LIST))
MV_CV_OBJ = $(patsubst %, $(TEST_OUT_DIR)/%, $(notdir $(patsubst %.cpp, %.o, $(MV_CV_KERNELS))))

MVCV_DATA_TYPES_DIR = $(MVCV_DIR)/shave
MVCV_DATA_TYPES_SRC = $(wildcard $(MVCV_DATA_TYPES_DIR)/*.cpp)
MVCV_DATA_TYPES_INC_DIR	= $(MVCV_DIR)/include
MVCV_DATA_TYPES_H 	= $(wildcard $(MVCV_DATA_TYPES_INC_DIR)/*.h)
MVCV_DATA_TYPES_OBJ = $(patsubst %, $(UNIT_TEST_OUT_DIR)/%, $(notdir $(patsubst %.cpp, %.o, $(MVCV_DATA_TYPES_SRC))))

MVCV_KERNELS_DEPS_INC_PATH = $(foreach kernel, $(MVCV_KERNELS_DEPS), $(MVCV_DIR)/kernels/$(kernel)/shave/include)
MVCV_KERNELS_DEPS_INC_DIR = $(foreach path, $(MVCV_KERNELS_DEPS_INC_PATH), -I$(path))
MVCV_KERNELS_DEPS_H =  $(foreach path, $(MVCV_KERNELS_DEPS_INC_PATH), $(wildcard $(path)/*.h))
MVCV_KERNELS_DEPS_SRC_DIR = $(foreach kernel, $(MVCV_KERNELS_DEPS), $(MVCV_DIR)/kernels/$(kernel)/shave/src/cpp)
MVCV_KERNELS_DEPS_SRC = $(foreach kernel, $(MVCV_KERNELS_DEPS), $(MVCV_DIR)/kernels/$(kernel)/shave/src/cpp/$(kernel).cpp)
MVCV_KERNELS_DEPS_OBJ = $(patsubst %, $(TEST_OUT_DIR)/%, $(notdir $(patsubst %.cpp, %.o, $(MVCV_KERNELS_DEPS_SRC))))

EXTRA_LIBS += $(MVCV_DATA_TYPES_OBJ) $(MVCV_KERNELS_DEPS_OBJ)

CPPFLAGS += -I$(MVCV_DATA_TYPES_INC_DIR)

# Most kernels don't have dependencies on other kernels so avoid adding an empty variable
ifneq ($(MVCV_KERNELS_DEPS_INC_DIR), )
CPPFLAGS += -I$(MVCV_KERNELS_DEPS_INC_DIR)
endif

include $(UNIT_TEST_DIR)/unittest.mk

#MVCV stuff
.SECONDEXPANSION:
$(MVCV_KERNELS_DEPS_OBJ) : $(UNIT_TEST_OUT_DIR)/%.o : $(MVCV_DIR)/kernels/$$*/shave/src/cpp/$$*.cpp $(MVCV_KERNELS_DEPS_H)
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(MVCV_DATA_TYPES_OBJ) : $(UNIT_TEST_OUT_DIR)/%.o : $(MVCV_DATA_TYPES_DIR)/%.cpp $(MVCV_DATA_TYPES_H)
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(MV_CV_OBJ) : $(TEST_OUT_DIR)/%.o : $(MVCV_SHAVE_DIR)/%.cpp $(MVCV_INCLUDE_DIR)/*.h $(UNIT_TEST_MODULES)
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
#TESTED_HEADERS = $(addprefix $(MVCV_INCLUDE_DIR)/, $(notdir $(patsubst %.cpp, %.h, $(TESTED_KERNELS))))
$(UNIT_TEST_OBJECTS) : $(TEST_OUT_DIR)/%.o : $(APPDIR)/%.cpp $(MV_CV_OBJ) $(UNIT_TEST_OBJ) $(UNIT_TEST_MODULES) $(UNIT_TEST_HEADERS)
	@echo Generating: $@
	@mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

SEED_VAL ?= 0
srvPort ?= 30000

run_test_seed:
	$(Test_Executable) -srvPort: $(srvPort) -useSeed: $(SEED_VAL)
clean_lib:
	$(ECHO) cd $(KERNEL_APP_PATH); make clean_lib
start_server:
	$(ECHO) cd $(KERNEL_APP_PATH); make start_server srvPort=30001
start_simulator:
	$(ECHO) cd $(KERNEL_APP_PATH); make start_simulator srvPort=30000

