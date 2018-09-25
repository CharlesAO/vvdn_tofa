#Activate the flag that disables debug mode build and keep release mode as default
BUILD_WITH_CODE_SIZE_OPTIMIZATION=yes
# ----------------------------------[ App ]----------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk

MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450
LEON_RT_BUILD = yes

# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems


# ------------------------------[ Components used ]--------------------------------#
#Component lists:
ComponentList_LOS += PipePrint FatalExtension UnitTestVcs VcsHooks
ComponentList_LRT += PipePrint

#Choosing if this project has shave components or not
SHAVE_COMPONENTS=no

#----------------------------------[ Settings ]------------------------------------#

#----------------------[ Local shave applications sources ]------------------------#

#Choosing C sources the example application on shave
spExample = output/spExample
SH_COMPONENTS_HEADERS_PATHS +=  $(wildcard $(DirAppRoot)/shave*)

SHAVE_C_SOURCES_sampleProfiler = $(wildcard $(DirAppRoot)/shave/*.c)
#Choosing ASM sources for the sample profiler example app on shave
#SHAVE_ASM_SOURCES_sampleProfiler = $(wildcard $(DirAppRoot)/shave/$(MYRIAD)/*.asm)

#Generating list of required generated assembly files for the sample profiler example app on shave
SHAVE_GENASMS_sampleProfiler = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_sampleProfiler))
#Generating required objects list from sources
SHAVE_sampleProfiler_OBJS = $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_sampleProfiler)) \
                   $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_sampleProfiler))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_sampleProfiler) $(SHAVE_sampleProfiler_OBJS) $(SHAVE0_APPS) $(SHAVE_APP_LIBS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_sampleProfiler)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(spExample).mvlib

define DEFINE_SHAVE_APPS
SHAVE$(1)_APPS = $(spExample).shv$(1)lib
endef

SHAVES_IDX = 0 1 2 3 4 5 6 7 8 9 10 11
$(foreach idx,$(SHAVES_IDX),$(eval $(call DEFINE_SHAVE_APPS,$(idx))))

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
# MV_TOOLS_VERSION =

# Verbose or not, uncomment for more verbosity
#ECHO=

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../common

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the spExample application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
ENTRYPOINTS += -e profile --gc-sections
$(spExample).mvlib : $(SHAVE_sampleProfiler_OBJS) $(PROJECT_SHAVE_LIBS)
	@mkdir -p $(dir $@)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(SHAVE_sampleProfiler_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@


# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options

# Extra app related options
#CCOPT			+= -DDEBUG

# The sample profiler will use DDR. The needed DDR buffer size can be determined using the following two configuration parameters.
# Please check the readme how to calculate the free DDR space needed.
# The sample duration should be configured. For most of the applications, this value should be ok. If the application runs shaves for a long time, bigger values can be used for decreasing the used DDR size
SAMPLE_PERIOD_MICRO=10
SAMPLE_PERIOD_MICRO_LRT=10

# This value should be configured to cover the total runtime of shaves. In the current example we are using 12 shaves, ~128 ms each. This means 1536 ms. We are setting a bigger value for this, to be on the safe side
SAMPLE_DURATION_MS=5000
SAMPLE_DURATION_MS_LRT=5000

# Enable Graph creation. Value 0 means text output only. 2 benefits: easier setup on Windows, faster execution
SAMPLE_PROFILER_GRAPHS_ENABLED=1

TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, MA2450, MOVISIM_LONG_RUN_TEST_MA2150"
TEST_HW_PLATFORM := "MV0182_MA2150, MV0212_MA2450, MOVISIM_MA2150, MOVISIM_MA2450"
