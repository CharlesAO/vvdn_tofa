# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
APPNAME ?= RANSAC

# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk

MV_SOC_PLATFORM =myriad2
LinkerScript = $(DirAppRoot)/config/custom.ldscript
# Ensure that the we are using the correct rtems libs etc
MV_SOC_OS = rtems

# ------------------------------[ Components used ]--------------------------------#
#ComponentList = 
ComponentList += VcsHooks
#No components used by RANSAC app
#Choosing if this project has shave components or not
SHAVE_COMPONENTS = yes
MVCV_KERNELS_DEPS = calcEpipolarDistance svd
#-------------------------------[ Settings ]---------------------------------------#
MV_SOC_PLATFORM = myriad2

#-------------------------------[ Local shave applications sources ]--------------------#

#Choosing C sources the RANSAC application on shave
RANSACApp = ../shave/dummy
SHAVE_CPP_SOURCES_RANSAC += $(wildcard $(DirAppRoot)/../../../shave/src/cpp/*.cpp)


SHAVE_CPP_SOURCES_RANSAC += $(wildcard $(DirAppRoot)/../../../../svd/shave/src/cpp/*.cpp)
SHAVE_CPP_SOURCES_RANSAC += $(wildcard $(DirAppRoot)/../../../../calcEpipolarDistance/shave/src/cpp/*.cpp)
SHAVE_ASM_SOURCES_RANSAC += $(wildcard $(DirAppRoot)/../../../../calcEpipolarDistance/shave/src/*.asm)

#SHAVE_CPP_SOURCES_RANSAC += $(wildcard $(DirAppRoot)/../Components/*.cpp)
#SHAVE_CPP_SOURCES_RANSAC += $(wildcard $(DirAppRoot)/../Components/svd/*.cpp)
#SHAVE_CPP_SOURCES_RANSAC += $(wildcard $(DirAppRoot)/../Components/CalcEpipolar/*.cpp)
$(info $$DirAppRoot is [${DirAppRoot}])
SHAVE_CPP_SOURCES_RANSAC += $(wildcard $(DirAppRoot)/../shave/*.cpp)
#Choosing ASM sources for the shave RANSAC app on shave
#no asm files
#SHAVE_ASM_SOURCES_RANSAC =

#Generating list of required generated assembly files for the RANSAC app on shave
SHAVE_GENASMS_RANSAC = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_RANSAC))
#Generating required objects list from sources
SHAVE_RANSAC_OBJS = $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_RANSAC)) \
                    $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_RANSAC))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_RANSAC) $(SHAVE_RANSAC_OBJS) $(SHAVE0_APPS) $(SHAVE_APP_LIBS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_RANSAC)

#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(RANSACApp).mvlib
SHAVE0_APPS = $(RANSACApp).shv0lib

# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION =00.50.23.0

# Verbose or not, uncomment for more verbosity
#ECHO=



# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../../../../..

# Sampler Profiler
SAMPLE_PERIOD_MICRO = 10
SAMPLE_DURATION_MS = 5000
SAMPLE_PROFILER_GRAPHS_ENABLED = 1

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the RANSACApp application. Simple rule specifying 
#which objects build up the said application. The application will be built into a library


ENTRYPOINTS = -e start  --gc-sections
$(RANSACApp).mvlib : $(SHAVE_RANSAC_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_RANSAC_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@


# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options 

# Extra app related options
#CCOPT			+= -DDEBUG
CCOPT         += -I ../shared
CCOPT         += -I ../../../../svd/shave/include
CCOPT         += -I ../../../../calcEpipolarDistance/shave/include
CCOPT         += -I ../../../shave/include
CCOPT         += -I ../../../../../include

MVCCOPT         += -I ../shared
MVCCOPT         += -I ../../../../svd/shave/include
MVCCOPT         += -I ../../../../calcEpipolarDistance/shave/include
MVCCOPT         += -I ../../../shave/include
MVCCOPT         += -I ../../../../../include




# ------------------------------[ Extra Rules ]-------------------------------------------#

DDR_DATA = .ddr.data
REVERSE_BYTES=

# Add to MVASM include path
#$(DirAppObjDir)/testframe.o: $(MY_RESOURCE) Makefile
#	@mkdir -p $(dir $@)
#	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
#	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=inputFrame \
#	-O elf32-sparc -B sparc $< $@
