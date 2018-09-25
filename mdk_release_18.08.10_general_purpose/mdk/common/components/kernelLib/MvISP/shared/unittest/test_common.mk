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

EXTRA_LIBS += $(MVISP_OBJ)

MVISP_DIR = $(MV_COMMON_BASE)/components/kernelLib/MvISP
MVCV_DIR   = $(MV_COMMON_BASE)/components/kernelLib/MvCV
MVISP_APPS_DIR = $(wildcard $(MVISP_DIR)/kernels/*/shave/include)
MVISP_APPS_HEADERS = $(patsubst %, %/*.h, $(MVISP_APPS_DIR))
MVISP_APPS_INCLUDE_DIR = $(patsubst %, -I%, $(MVISP_APPS_DIR))
MVISP_INCLUDE_DIR = $(MVISP_DIR)/include
MVCV_INCLUDE_DIR = $(MVCV_DIR)/include
MVISP_SHAVE_DIR   = $(MVISP_DIR)/kernels/$(APPNAME)/shave/src/cpp
CPPFLAGS += -I$(MVISP_INCLUDE_DIR)
CPPFLAGS += -I$(MVCV_INCLUDE_DIR)
CPPFLAGS += $(MVISP_APPS_INCLUDE_DIR)
MVISP_KERNELS = $(wildcard $(MVISP_SHAVE_DIR)/*.cpp)
MVISP_OBJ = $(patsubst %, $(TEST_OUT_DIR)/%, $(notdir $(patsubst %.cpp, %.o, $(MVISP_KERNELS))))

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

EXTRA_LIBS += $(MVCV_KERNELS_DEPS_OBJ) $(MVISP_KERNELS_DEPS_OBJ)

# Most kernels don't have dependencies on other kernels so avoid adding an empty variable
ifneq ($(MVCV_KERNELS_DEPS_INC_DIR), )
CPPFLAGS += -I$(MVCV_KERNELS_DEPS_INC_DIR)
endif

include $(UNIT_TEST_DIR)/unittest.mk

#MVCV dependency
.SECONDEXPANSION:
$(MVCV_KERNELS_DEPS_OBJ) : $(UNIT_TEST_OUT_DIR)/%.o : $(MVCV_DIR)/kernels/$$*/shave/src/cpp/$$*.cpp $(MVCV_KERNELS_DEPS_H)
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#MVISP dependency
.SECONDEXPANSION:
$(MVISP_KERNELS_DEPS_OBJ) : $(UNIT_TEST_OUT_DIR)/%.o : $(MVISP_DIR)/kernels/$$*/shave/src/cpp/$$*.cpp $(MVISP_KERNELS_DEPS_H)
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(MVISP_OBJ) : $(TEST_OUT_DIR)/%.o : $(MVISP_SHAVE_DIR)/%.cpp $(MVISP_INCLUDE_DIR)/*.h $(MVCV_INCLUDE_DIR)/*.h $(UNIT_TEST_MODULES)
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(UNIT_TEST_OBJECTS) : $(TEST_OUT_DIR)/%.o : $(APPDIR)/%.cpp $(MVISP_OBJ) $(UNIT_TEST_OBJ) $(UNIT_TEST_MODULES) $(UNIT_TEST_HEADERS)
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


