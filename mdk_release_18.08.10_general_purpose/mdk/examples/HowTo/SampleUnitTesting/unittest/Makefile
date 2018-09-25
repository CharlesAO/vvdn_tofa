# set MDK_BASE
MDK_BASE = ../../../..

# the path to unttitest.mk
UNIT_TEST_DIR ?= $(MDK_BASE)/regression/unittest

# set TEST_NAME (ex: sampleApp_unittest)
TEST_NAME = sampleApp_unittest

# set TEST_DIR
TEST_DIR = .

# set KERNEL_APP_PATH
KERNEL_APP_PATH = dummy

# set KERNEL_ASM_LABEL 
KERNEL_ASM_LABEL = sampleApp_asm

# set KERNEL_APP_NAME
KERNEL_APP_NAME = dummy

# set TARRGET_PREFIX
TARGET_PREFIX = dummy

# set EXTRA_LIBS
EXTRA_LIBS = $(SampleApp_OBJ)

# here is the reference code
SampleApp_REF_DIR  = ..

# make the objects from the ref code

SampleApp_OBJ   = $(patsubst %, $(TEST_OUT_DIR)/%, $(notdir $(patsubst %.c, %.o, $(wildcard $(SampleApp_REF_DIR)/*.c))))
SampleApp_OBJ  += $(patsubst %, $(TEST_OUT_DIR)/%, $(notdir $(patsubst %.cpp, %.o, $(wildcard $(SampleApp_REF_DIR)/*.cpp))))

# required headers
CPPFLAGS += $(patsubst %,-I %,$(SampleApp_REF_DIR))

CPPFLAGS += -D"TOLERANCE_MVCV_CC=0"

include $(UNIT_TEST_DIR)/unittest.mk

GTEST_PROJ_STRING = "GTEST_FALSE"

$(SampleApp_OBJ) : $(SampleApp_REF_DIR)/*.c
	@echo Generating: $@
	$(ECHO) @mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(UNIT_TEST_OBJECTS) : $(TEST_OUT_DIR)/%.o : $(APPDIR)/%.cpp $(SampleApp_OBJ) $(UNIT_TEST_OBJ) $(UNIT_TEST_MODULES) $(UNIT_TEST_HEADERS)
	@echo Generating: $@
	@mkdir -p $(dir $@)
	$(ECHO) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

start_server:
	$(ECHO) cd $(KERNEL_APP_PATH); make start_server srvPort=30001
start_simulator:
	$(ECHO) cd $(KERNEL_APP_PATH); make start_simulator srvPort=30000

# don't consider it a MDK Project
MDK_PROJ_STRING = "MDK_FALSE"


TEST_TAGS:= "MA2150, MA2450"
