#  ---------------------------------------------------------------------------
#  Copyright (C) 2017-2018 Movidius Ltd. All rights reserved
#  ---------------------------------------------------------------------------
#  File       :   Makefile
#  Description:   A simple Makefile for building a standalone ISO C/C++
#                 conventional program using 'main' on only SHAVE #0.
#
#                 When executed this will print:
#                   'moviCompile' test passed
#                 or:
#                   'moviCompile' test failed
#                 with additional information about the test.
#
#                 All sources for the test found in the directory named by the
#                 variable 'TEST_PATH'.  The name for the test is derived from
#                 the path.  Optionally, the test can also be given a name that
#                 is not derived from its path using the variable 'TEST_NAME'.
#
#                 The CPU to be used is specified by 'TARGET_CPU' and defaults
#                 to 'ma2150'.
#
#                 The variable 'MV_TOOLS_DIR' must be set to point to the root
#                 of the 'moviTools' to be used.  The default assumes that the
#                 root is at '../../..' relative to the Makefile.
#
#                 The variable 'BUILD_ROOT' is the base directory for the test
#                 artefacts, and this defaults to '/tmp/moviTestStub'.
#
#                 If the variable 'SAVE_TEMPS' is set to any value, then the
#                 intermediate build artefacts such as the pre-processed source
#                 and generated assembly will be retained.
#  ---------------------------------------------------------------------------


# The directory containing the tools, this defaults to '../../..'
MV_TOOLS_DIR		?= ../../..


# Name of the directory containing the test
TEST_PATH			?= helloWorld

# Name to use for the test
TEST_NAME			?= $(notdir $(realpath $(TEST_PATH)))


# Which CPU to target, this defaults to 'ma2150'
TARGET_CPU			?= ma2150
#
# Verify that the provided CPU is supported and setup other flags accordingly
#
ifeq ($(TARGET_CPU),ma2100)
MV_CPU				= ma2100
MV_CPU_LIB			= ma2100
MV_MDK_LIB			= ma2100
# Handling the MA2x5x family
#   MA215[05x]
else ifeq ($(TARGET_CPU),ma2150)
MV_CPU				= ma2150
MV_CPU_LIB			= ma2x5x
MV_MDK_LIB			= ma215x
else ifeq ($(TARGET_CPU),ma2155)
MV_CPU				= ma2155
MV_CPU_LIB			= ma2x5x
MV_MDK_LIB			= ma215x
else ifeq ($(TARGET_CPU),ma215x)
MV_CPU				= ma2150
MV_CPU_LIB			= ma2x5x
MV_MDK_LIB			= ma215x
else ifeq ($(TARGET_CPU),ma2x5x)
MV_CPU				= ma2150
MV_CPU_LIB			= ma2x5x
MV_MDK_LIB			= ma215x
#   MA245[05x]
else ifeq ($(TARGET_CPU),ma2450)
MV_CPU				= ma2450
MV_CPU_LIB			= ma2x5x
MV_MDK_LIB			= ma245x
else ifeq ($(TARGET_CPU),ma2455)
MV_CPU				= ma2455
MV_CPU_LIB			= ma2x5x
MV_MDK_LIB			= ma245x
else ifeq ($(TARGET_CPU),ma245x)
MV_CPU				= ma2450
MV_CPU_LIB			= ma2x5x
MV_MDK_LIB			= ma245x
# Handling the MA2x8x family
#   MA248[05x]
else ifeq ($(TARGET_CPU),ma2480)
MV_CPU				= ma2480
MV_CPU_LIB			= ma2x8x
MV_MDK_LIB			= ma248x
else ifeq ($(TARGET_CPU),ma2485)
MV_CPU				= ma2485
MV_CPU_LIB			= ma2x8x
MV_MDK_LIB			= ma248x
else ifeq ($(TARGET_CPU),ma248x)
MV_CPU				= ma2480
MV_CPU_LIB			= ma2x8x
MV_MDK_LIB			= ma248x
else
$(error Unknown TARGET_CPU '$(TARGET_CPU)')
endif


# Is this running on Cygwin?  If not assume Linux
OSTYPE				:= $(shell uname -o | sed -e 's/GNU\///' | tr '[:upper:]' '[:lower:]' )
#
# Configure platform specific parts of the project
#
ifeq ($(OSTYPE),cygwin)
MV_PLATFORM			= win32
else
MV_PLATFORM			= linux64
endif
MV_CC				= $(MV_TOOLS_DIR)/$(MV_PLATFORM)/bin/moviCompile
MV_SIM				= $(MV_TOOLS_DIR)/$(MV_PLATFORM)/bin/moviSim
MV_DEBUG			= $(MV_TOOLS_DIR)/$(MV_PLATFORM)/bin/moviDebug2
MV_LD				= $(MV_TOOLS_DIR)/$(MV_PLATFORM)/sparc-myriad-rtems-6.3.0/bin/sparc-myriad-rtems-ld
MV_OBJCOPY			= $(MV_TOOLS_DIR)/$(MV_PLATFORM)/sparc-myriad-rtems-6.3.0/bin/sparc-myriad-rtems-objcopy

ifneq ($(USE_FP64_PRINTF),)
_FP64_PRINTF		= $(MV_TOOLS)/common/moviCompile/lib/$(MV_CPU)/ldbl2stri.o
else
_FP64_PRINTF		=
endif

ifeq ($(USE_MLIBC),)
_LITE				= _lite
else
_LITE				=
endif


# Target dependent options
ifeq ($(TARGET_CPU),ma2100)
OBJCOPY_FLAGS		= --prefix-alloc-sections=.shv0. --prefix-symbols=__llvmTest_
_MEMMAP				= ./mdkstub/$(MV_MDK_LIB)/lib/memmap.o
else
OBJCOPY_FLAGS		= --prefix-alloc-sections=.shv0. --prefix-symbols=__llvmTest0_
_MEMMAP				=
endif

# Where to place the build artefacts
BUILD_ROOT			?= /tmp/moviTestStub
BUILD_DIR			= $(BUILD_ROOT)/$(MV_CPU)/$(TEST_NAME)


# Save the intermediate files if requested
SAVE_TEMPS			?=

ifneq ($(SAVE_TEMPS),)
SAVE_FLAGS			= -save-temps=obj
else
SAVE_FLAGS			=
endif


# Find all the sources for this test
vpath			%.c   $(realpath $(TEST_PATH))
vpath			%.cc  $(realpath $(TEST_PATH))
vpath			%.cpp $(realpath $(TEST_PATH))
vpath			%.cxx $(realpath $(TEST_PATH))
vpath			%.S   $(realpath $(TEST_PATH))
vpath			%.s   $(realpath $(TEST_PATH))


C_SRCS			= $(wildcard $(realpath $(TEST_PATH))/*.c)

CC_SRCS			= $(wildcard $(realpath $(TEST_PATH))/*.cc)
CPP_SRCS		= $(wildcard $(realpath $(TEST_PATH))/*.cpp)
CXX_SRCS		= $(wildcard $(realpath $(TEST_PATH))/*.cxx)

ASM_SRCS		= $(wildcard $(realpath $(TEST_PATH))/*.s)
S_SRCS			= $(wildcard $(realpath $(TEST_PATH))/*.S)

USER_OBJS		= $(wildcard $(realpath $(TEST_PATH))/$(MV_MDK_LIB)/*.o)

OBJS			= $(patsubst %,$(BUILD_DIR)/%,$(notdir $(C_SRCS:.c=.o) $(CC_SRCS:.cc=.o) $(CPP_SRCS:.cpp=.o) $(CXX_SRCS:.cxx=.o) $(ASM_SRCS:.s=.o) $(S_SRCS:.s=.o)))

FINAL_TEST_ELF	= $(BUILD_DIR)/$(TEST_NAME).elf


# List of the phony targets used
.PHONY:		help help_targets help_variables info
.PHONY:		build
.PHONY:		clean cleanall
.PHONY:		board sim
.PHONE:		_gap1 _gap2



# --------------------------------------------------------------------------------------------------------------------------
# Display information about the targets for this Makefile
help:	help_targets _gap1 help_variables _gap2 info

# --------------------------------------------------------------------------------------------------------------------------
# Display information about the targets for this Makefile
help_targets:
	@echo "Select from the following targets:"
	@echo "  General targets:"
	@echo "    help                      - Displays information about using this Makefile {this is the default target}"
	@echo "    help_targets              - Displays information about the targets for this Makefile"
	@echo "    help_variables            - Displays information about useful variables for this Makefile"
	@echo "    info                      - Displays information about this configuration"
	@echo "  Build targets:"
	@echo "    build                     - Builds the current test '$(TEST_NAME)', but does not run it"
	@echo "  Clean targets:"
	@echo "    clean                     - Removes intermediate build files for the current test '$(TEST_NAME)'"
	@echo "    cleanall                  - Removes all intermediate build files"
	@echo "  Execution targets:"
	@echo "    board                     - NYI: Runs the current test '$(TEST_NAME)' on the Myriad development board"
	@echo "    sim                       - Runs the current test '$(TEST_NAME)' on the simulator 'moviSim'"

# --------------------------------------------------------------------------------------------------------------------------
# Display information about variables that can be used to override the configuration
help_variables:
	@echo "Variables:"
	@echo "  The are many several used by this Makefile to manage the build and test process.  These all have reasonable"
	@echo "  defaults based on the conventions for locating sources used by 'moviCompile', but they may be overridden by"
	@echo "  specifying them on the 'make' command-line using the following format:"
	@echo "     make <VARIABLE>=<value> <targets>"
	@echo "  where:"
	@echo "     <VARIABLE>               - The name of the variable you wish to change"
	@echo "     <value>                  - The new value you want to set the variable to"
	@echo "     <targets>                - The set of 'make' targets you want to run {default is 'help'}"
	@echo "  Select <VARIABLE> from the following:"
	@echo "     BUILD_ROOT               - This allows the root directory for the build artefacts to be specified"
	@echo "                                  Default is '/tmp/moviTestStub'"
	@echo "     CFLAGS                   - Additional flags to pass to the C compiler"
	@echo "     CPPFLAGS                 - Additional flags to pass to the pre-processor"
	@echo "     CXXFLAGS                 - Additional flags to pass to the C++ compiler"
	@echo "     MV_TOOLS_DIR             - Path to the 'moviTools' to be used for the test {default is '$(realpath ../../..)'}"
	@echo "     SAVE_TEMPS=1             - If this is set, the intermediate build files will be kept, which includes"
	@echo "                                the pre-processed sources and generated assembly code {default is not set}"
	@echo "     TARGET_CPU               - The Myriad chip to run the test for {default 'ma2150'}"
	@echo "     TEST_NAME                - The name to be used for the test.  If not specified, this defaults"
	@echo "                                to the final part of the value specified by 'TEST_PATH'"
	@echo "     TEST_PATH                - Path to the directory containing the program to be tested"
	@echo "                                  Default is '$(realpath ./helloWorld)'"
	@echo "     USE_FP64_PRINTF=1        - If this is set, the test will be linked with 'printf' support for 64-bit"
	@echo "                                floating-point values {default is not set}"
	@echo "     USE_MLIBC=1              - If this is set, the full Newlib ISO C library will be used instead of the"
	@echo "                                smaller 'lite' library {default is not set}"

# --------------------------------------------------------------------------------------------------------------------------
# Display information about this build configuration
info:
	@echo "Configuration information:"
	@echo "  Detected OS is:                       $(MV_PLATFORM)"
	@echo "  moviAsm version is:                   TBD"
	@echo "  moviCompile version is:               TBD"
	@echo "  moviDebug version is:                 TBD"
	@echo "  moviSim version is:                   TBD"
	@echo "  sparc-myriad-rtems-6.3.0 version is:  TBD"
	@echo ""
	@echo "  BUILD_ROOT is:                        $(BUILD_ROOT)"
	@echo "  CFLAGS is:                            $(CFLAGS)"
	@echo "  CPPFLAGS is:                          $(CPPFLAGS)"
	@echo "  CXXFLAGS is:                          $(CXXFLAGS)"
	@echo "  MV_TOOLS_DIR is:                      $(realpath $(MV_TOOLS_DIR))"
ifeq ($(SAVE_TEMPS),)
	@echo "  SAVE_TEMPS is:                        Not enabled"
else
	@echo "  SAVE_TEMPS is:                        Enabled"
endif
	@echo "  TARGET_CPU is:                        $(TARGET_CPU)"
	@echo "  TEST_NAME is:                         $(TEST_NAME)"
	@echo "  TEST_PATH is:                         $(realpath $(TEST_PATH))"
ifeq ($(USE_FP64_PRINTF),)
	@echo "  USE_FP64_PRINTF is:                   Is not selected"
else
	@echo "  USE_FP64_PRINTF is:                   Is selected"
endif

_gap1:
	@echo ""
	@echo "----------------------------------------------------"
	@echo ""

_gap2:
	@echo ""
	@echo "----------------------------------------------------"
	@echo ""


build:		$(FINAL_TEST_ELF)

clean:
	@rm -rf $(BUILD_DIR)

cleanall:
	@rm -rf $(BUILD_ROOT)

sim:		$(FINAL_TEST_ELF)
ifneq ($(OSTYPE),cygwin)
	@$(MV_SIM) -nodasm -cv:$(MV_CPU) -l:LOS:"$<"
else
	@$(MV_SIM) -nodasm -cv:$(MV_CPU) -l:LOS:"`cygpath -m $<`"
endif

board:
	@echo 'board' target is not yet implemented



# Inference rules for building the intermediate artefacts contributing to the test
#
#   First the rules for creating object files from the sources
ifneq ($(OSTYPE),cygwin)
$(BUILD_DIR)/%.o:	%.c
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c $(CPPFLAGS) $(CFLAGS) -fverbose-asm $(SAVE_FLAGS) -mcpu=$(MV_CPU) -Wall -Wextra $< -o $@

$(BUILD_DIR)/%.o:	%.cc
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c $(CPPFLAGS) $(CXXFLAGS) -fverbose-asm $(SAVE_FLAGS) -mcpu=$(MV_CPU) -Wall -Wextra $< -o $@

$(BUILD_DIR)/%.o:	%.cpp
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c $(CPPFLAGS) $(CXXFLAGS) -fverbose-asm $(SAVE_FLAGS) -mcpu=$(MV_CPU) -Wall -Wextra $< -o $@

$(BUILD_DIR)/%.o:	%.cxx
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c $(CPPFLAGS) $(CXXFLAGS) -fverbose-asm $(SAVE_FLAGS) -mcpu=$(MV_CPU) -Wall -Wextra $< -o $@

$(BUILD_DIR)/%.o:	%.S
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c $(CPPFLAGS) $(SAVE_FLAGS) -mcpu=$(MV_CPU) $< -o $@

$(BUILD_DIR)/%.o:	%.s
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c -mcpu=$(MV_CPU) $< -o $@
else
$(BUILD_DIR)/%.o:	%.c
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c $(CPPFLAGS) $(CFLAGS) -fverbose-asm $(SAVE_FLAGS) -mcpu=$(MV_CPU) -Wall -Wextra `cygpath -m $<` -o `cygpath -m $@`

$(BUILD_DIR)/%.o:	%.cc
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c $(CPPFLAGS) $(CXXFLAGS) -fverbose-asm $(SAVE_FLAGS) -mcpu=$(MV_CPU) -Wall -Wextra `cygpath -m $<` -o `cygpath -m $@`

$(BUILD_DIR)/%.o:	%.cpp
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c $(CPPFLAGS) $(CXXFLAGS) -fverbose-asm $(SAVE_FLAGS) -mcpu=$(MV_CPU) -Wall -Wextra `cygpath -m $<` -o `cygpath -m $@`

$(BUILD_DIR)/%.o:	%.cxx
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c $(CPPFLAGS) $(CXXFLAGS) -fverbose-asm $(SAVE_FLAGS) -mcpu=$(MV_CPU) -Wall -Wextra `cygpath -m $<` -o `cygpath -m $@`

$(BUILD_DIR)/%.o:	%.S
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c $(CPPFLAGS) $(SAVE_FLAGS) -mcpu=$(MV_CPU) `cygpath -m $<` -o `cygpath -m $@`

$(BUILD_DIR)/%.o:	%.s
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(MV_CC) -c -mcpu=$(MV_CPU) `cygpath -m $<` -o `cygpath -m $@`
endif


#   First phase link combining all of the SHAVE components
$(BUILD_DIR)/$(TEST_NAME).mvlib:		$(OBJS)
	@$(MV_LD) -EL -Ur -O9 --demangle -e _EP_start -u _EP_setheap -u __stackHighWater --gc-sections \
		-T $(MV_TOOLS_DIR)/common/moviCompile/ldscripts/shave_first_phase.ld \
		$^ $(USER_OBJS) \
		$(_FP64_PRINTF) \
		$(MV_TOOLS_DIR)/common/moviCompile/lib/$(MV_CPU_LIB)/mlibcxx.a \
		$(MV_TOOLS_DIR)/common/moviCompile/lib/$(MV_CPU_LIB)/mlibneon.a \
		$(MV_TOOLS_DIR)/common/moviCompile/lib/$(MV_CPU_LIB)/mlibprofile.a \
		$(MV_TOOLS_DIR)/common/moviCompile/lib/$(MV_CPU_LIB)/mlibVecUtils.a \
		--start-group \
			$(MV_TOOLS_DIR)/common/moviCompile/lib/$(MV_CPU_LIB)/mlibm.a \
			$(MV_TOOLS_DIR)/common/moviCompile/lib/$(MV_CPU_LIB)/mlibc$(_LITE).a \
			$(MV_TOOLS_DIR)/common/moviCompile/lib/$(MV_CPU_LIB)/mlibc$(_LITE)_lgpl.a \
			$(MV_TOOLS_DIR)/common/moviCompile/lib/$(MV_CPU_LIB)/mlibcrt.a \
		--end-group \
		-o $@


#   Symbol and section renaming to avoid symbol conflict with the Leon components
$(BUILD_DIR)/$(TEST_NAME).shv0lib:	$(BUILD_DIR)/$(TEST_NAME).mvlib
	@$(MV_OBJCOPY) $(OBJCOPY_FLAGS) $< $@


#   Final link stage to create the ELF from the pre-cooked Leon stub code and the just built SHAVE code
$(FINAL_TEST_ELF):	$(BUILD_DIR)/$(TEST_NAME).shv0lib
	@$(MV_LD) -O9 --demangle --gc-sections -EL \
		-L $(MV_TOOLS_DIR)/$(MV_PLATFORM)/sparc-myriad-rtems-6.3.0/sparc-myriad-rtems/lib \
		-L $(MV_TOOLS_DIR)/$(MV_PLATFORM)/sparc-myriad-rtems-6.3.0/lib/gcc/sparc-myriad-rtems/6.3.0 \
		-L ./mdkstub/$(MV_MDK_LIB)/lib \
		-L ./mdkstub/$(MV_MDK_LIB)/ldscripts \
		-T./mdkstub/$(MV_MDK_LIB)/ldscripts/default_memory_map_elf.ldscript \
		./mdkstub/$(MV_MDK_LIB)/lib/main.o \
		./mdkstub/$(MV_MDK_LIB)/lib/rtems-stubs.o \
		$(_MEMMAP) \
		-lLeonObjects \
		--start-group \
			-lc \
			-lgcc \
		--end-group \
		$< -o $@
