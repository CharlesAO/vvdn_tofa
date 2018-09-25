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

EXTRA_LIBS += $(LAMA_OBJ)
LAMA_DIR   = $(MV_COMMON_BASE)/components/kernelLib/LAMA
#MDK_KERNELS_DIR = $(MDK_BASE)/common/components/MvCV/kernels
LAMA_APPS_DIR = $(wildcard $(LAMA_DIR)/kernels/*/shave/include)
LAMA_APPS_HEADERS = $(patsubst %, %/*.h, $(LAMA_APPS_DIR))
LAMA_APPS_INCLUDE_DIR = $(patsubst %, -I%, $(LAMA_APPS_DIR))
#LAMA_INCLUDE_DIR =
# include mvcv_macro.h mat.h
MVCV_DIR = $(MV_COMMON_BASE)/components/kernelLib/MvCV

CPPFLAGS += -I$(MVCV_DIR)/include

LAMA_SHAVE_DIR   = $(LAMA_DIR)/kernels/$(APPNAME)/shave/src/cpp
TEST_DIR    ?= $(LAMA_DIR)/kernels/$(APPNAME)/unittest
#CPPFLAGS += -I$(LAMA_INCLUDE_DIR)
CPPFLAGS += $(LAMA_APPS_INCLUDE_DIR)
ALL_LAMA_LIST = $(wildcard $(LAMA_SHAVE_DIR)/*.cpp)
EXCLUDED_KERNELS =
EXCLUDE_LIST = $(patsubst %.cpp, $(LAMA_SHAVE_DIR)/%.cpp, $(EXCLUDED_KERNELS))
LAMA_KERNELS = $(filter-out $(EXCLUDE_LIST),$(ALL_LAMA_LIST))

LAMA_OBJ = $(patsubst %, $(TEST_OUT_DIR)/%, $(notdir $(patsubst %.cpp, %.o, $(LAMA_KERNELS))))

MVCV_KERNELS_DEPS_INC_PATH = $(foreach kernel, $(MVCV_KERNELS_DEPS), $(MVCV_DIR)/kernels/$(kernel)/shave/include)
MVCV_KERNELS_DEPS_INC_DIR = $(foreach path, $(MVCV_KERNELS_DEPS_INC_PATH), -I$(path))
MVCV_KERNELS_DEPS_H =  $(foreach path, $(MVCV_KERNELS_DEPS_INC_PATH), $(wildcard $(path)/*.h))
MVCV_KERNELS_DEPS_SRC_DIR = $(foreach kernel, $(MVCV_KERNELS_DEPS), $(MVCV_DIR)/kernels/$(kernel)/shave/src/cpp)
MVCV_KERNELS_DEPS_SRC = $(foreach kernel, $(MVCV_KERNELS_DEPS), $(MVCV_DIR)/kernels/$(kernel)/shave/src/cpp/$(kernel).cpp)
MVCV_KERNELS_DEPS_OBJ = $(patsubst %, $(TEST_OUT_DIR)/%, $(notdir $(patsubst %.cpp, %.o, $(MVCV_KERNELS_DEPS_SRC))))

MVISP_KERNELS_DEPS_INC_PATH = $(foreach kernel, $(MVISP_KERNELS_DEPS), $(MVISP_DIR)/kernels/$(kernel)/shave/include)
MVISP_KERNELS_DEPS_INC_DIR = $(foreach path, $(MVISP_KERNELS_DEPS_INC_PATH), -I$(path))
MVISP_KERNELS_DEPS_H =  $(foreach path, $(MVISP_KERNELS_DEPS_INC_PATH), $(wildcard $(path)/*.h))
MVISP_KERNELS_DEPS_SRC_DIR = $(foreach kernel, $(MVISP_KERNELS_DEPS), $(MVISP_DIR)/kernels/$(kernel)/shave/src/cpp)
MVISP_KERNELS_DEPS_SRC = $(foreach kernel, $(MVISP_KERNELS_DEPS), $(MVISP_DIR)/kernels/$(kernel)/shave/src/cpp/$(kernel).cpp)
MVISP_KERNELS_DEPS_OBJ = $(patsubst %, $(TEST_OUT_DIR)/%, $(notdir $(patsubst %.cpp, %.o, $(MVISP_KERNELS_DEPS_SRC))))

LAMA_KERNELS_DEPS_INC_PATH = $(foreach kernel, $(LAMA_KERNELS_DEPS), $(LAMA_DIR)/kernels/$(kernel)/shave/include)
LAMA_KERNELS_DEPS_INC_DIR = $(foreach path, $(LAMA_KERNELS_DEPS_INC_PATH), -I$(path))
LAMA_KERNELS_DEPS_H =  $(foreach path, $(LAMA_KERNELS_DEPS_INC_PATH), $(wildcard $(path)/*.h))
LAMA_KERNELS_DEPS_SRC_DIR = $(foreach kernel, $(LAMA_KERNELS_DEPS), $(LAMA_DIR)/kernels/$(kernel)/shave/src/cpp)
LAMA_KERNELS_DEPS_SRC = $(foreach kernel, $(LAMA_KERNELS_DEPS), $(LAMA_DIR)/kernels/$(kernel)/shave/src/cpp/$(kernel).cpp)
LAMA_KERNELS_DEPS_OBJ = $(patsubst %, $(TEST_OUT_DIR)/%, $(notdir $(patsubst %.cpp, %.o, $(LAMA_KERNELS_DEPS_SRC))))

EXTRA_LIBS += $(MVCV_KERNELS_DEPS_OBJ) $(MVISP_KERNELS_DEPS_OBJ) $(LAMA_KERNELS_DEPS_OBJ)


include $(UNIT_TEST_DIR)/unittest.mk

#MVCV dependency
.SECONDEXPANSION:
$(MVCV_KERNELS_DEPS_OBJ) : $(UNIT_TEST_OUT_DIR)/%.o : $(MVCV_DIR)/kernels/$$*/shave/src/cpp/$$*.cpp $(MVCV_KERNELS_DEPS_H)
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#MVISP dependency
$(MVISP_KERNELS_DEPS_OBJ) : $(UNIT_TEST_	OUT_DIR)/%.o : $(MVISP_DIR)/kernels/$$*/shave/src/cpp/$$*.cpp $(MVISP_KERNELS_DEPS_H)
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#LAMA dependency
$(LAMA_KERNELS_DEPS_OBJ) : $(UNIT_TEST_OUT_DIR)/%.o : $(LAMA_DIR)/kernels/$$*/shave/src/cpp/$$*.cpp $(LAMA_KERNELS_DEPS_H)
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(LAMA_OBJ) : $(TEST_OUT_DIR)/%.o : $(LAMA_SHAVE_DIR)/%.cpp $ $(UNIT_TEST_MODULES)
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(UNIT_TEST_OBJECTS) : $(TEST_OUT_DIR)/%.o : $(APPDIR)/%.cpp $(LAMA_OBJ) $(UNIT_TEST_OBJ) $(UNIT_TEST_MODULES) $(UNIT_TEST_HEADERS)
	@echo Generating: $@
	@mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

SEED_VAL ?= 0
srvPort ?= 30000

run_test_seed:
	$(Test_Executable) -srvPort: $(srvPort) -useSeed: $(SEED_VAL)
clean_lib:
	$(ECHO) @cd $(KERNEL_APP_PATH); make clean_lib
start_server:
	$(ECHO) cd $(KERNEL_APP_PATH); make start_server srvPort=30001
start_simulator:
	$(ECHO) cd $(KERNEL_APP_PATH); make start_simulator srvPort=30000


