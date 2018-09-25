# ------------------------------[ App ]--------------------------------------------#
# Default application name is the same as the folder it is in.
# This can be overridden here if something different is required
#APPNAME ?= $(notdir $(shell pwd))

EIGEN_APP           = ../../shared



# ------------------------------[ Build overrides ]--------------------------------#
# overrides that need to be set before including generic.mk
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2150

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480))
LinkerScript ?= $(EIGEN_APP)/config/custom_ma2480.ldscript
else
LinkerScript ?= $(EIGEN_APP)/config/custom.ldscript
endif

# Ensure that the we are using the correct rtems libs etc
#MV_SOC_OS = rtems
#RTEMS_BUILD_NAME = b-prebuilt

# ------------------------------[ Components used ]--------------------------------#
#Component lists:
ComponentList = PipePrint UnitTest
ComponentList_LOS += PipePrint
ComponentList_SVE := PipePrint


#Choosing if this project has shave components or not
SHAVE_COMPONENTS = yes
SHAVE_COMPONENT_INCLUDE_PATHS += $(EIGEN_APP)/include
LEON_COMPONENT_HEADERS_LOS += $(wildcard $(addsuffix /*.h,$(EIGEN_APP)/leon))



#-------------------------------[ Settings ]---------------------------------------#

#-------------------------------[ Local shave applications sources ]--------------------#
LEON_APP_CPP_SOURCES +=$(wildcard $(EIGEN_APP)/*leon/*.cpp)
LEON_APP_C_SOURCES +=$(wildcard $(EIGEN_APP)/*leon/*.c)

#Choosing C sources the eigen test application on shave
EigenTestApp = shave/eigenTest
SHAVE_CPP_SOURCES_EIGENTEST = $(wildcard ./shave/*.cpp)
SHAVE_C_SOURCES_EIGENTEST = $(wildcard ./shave/*.c)
SHAVE_CPP_SOURCES_SHAVE = $(wildcard $(EIGEN_APP)/shave/*.cpp)
SHAVE_C_SOURCES_SHAVE = $(wildcard $(EIGEN_APP)/shave/*.c)
#no asm files


#Generating list of required generated assembly files for the eigen test app on shave
SHAVE_GENASMS_EIGENTEST = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_EIGENTEST))
SHAVE_GENASMS_EIGENTEST += $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_EIGENTEST))
SHAVE_GENASMS_EIGENTEST_SHAVE = $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_CPP_SOURCES_SHAVE))
SHAVE_GENASMS_EIGENTEST_SHAVE += $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_SHAVE))
#Generating required objects list from sources
SHAVE_EIGENTEST_OBJS = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_EIGENTEST))
SHAVE_EIGENTEST_OBJS_SHAVE = $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_EIGENTEST_SHAVE))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_EIGENTEST) $(SHAVE_EIGENTEST_OBJS) $(SHAVE_EIGENTEST_OBJS_SHAVE)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_EIGENTEST)



#--------------------------[ Shave applications section ]--------------------------#
SHAVE_APP_LIBS = $(EigenTestApp).mvlib
SHAVE0_APPS = $(EigenTestApp).shv0lib



# ------------------------------[ Tools overrides ]--------------------------------#
# Hardcode tool version here if needed, otherwise defaults to latest stable release
#MV_TOOLS_VERSION ?=
# Verbose or not, uncomment for more verbosity
#ECHO ?=

# enable RTTI
MVCCPPOPT ?= -frtti
# moviCompile optimiser
MVCC_OPT_LEVEL ?= -O0
#MVASMOPT += -a

# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE  ?= ../../../../../../common

#CC_INCLUDE += $(addprefix -I,$(wildcard  $(EIGEN_APP)/include))
# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk
MVCC_INCLUDE += $(patsubst %,-I %, $(MV_COMMON_BASE)/components/Eigen/shave)
MVCC_INCLUDE += $(patsubst %,-I %, $(EIGEN_APP)/include)
                           
CCOPT+=-DeigenTest0___crtinit=eigenTest0__EP_crtinit
CCOPT+=-DeigenTest0___crtfini=eigenTest0__EP_crtfini


#-------------------------------[ Local shave applications build rules ]------------------#
#Describe the rule for building the eigen test application. Simple rule specifying
#which objects build up the said application. The application will be built into a library

ENTRYPOINTS += -e _EP_crtinit -u _EP_crtfini --gc-sections

$(EigenTestApp).mvlib : $(SHAVE_EIGENTEST_OBJS) $(PROJECT_SHAVE_LIBS) $(SHAVE_EIGENTEST_OBJS_SHAVE)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(SHAVE_EIGENTEST_OBJS) $(PROJECT_SHAVE_LIBS) $(SHAVE_EIGENTEST_OBJS_SHAVE) $(CompilerANSILibs) -o $@
CompilerANSILibs  = $(MV_TOOLS_BASE)/common/moviCompile/lib/$(MVISAOPT)/mlibcxx.a
CompilerANSILibs += $(MV_TOOLS_BASE)/common/moviCompile/lib/$(MVISAOPT)/mlibVecUtils.a

CompilerANSILibs += --start-group
CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibc.a
CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibc_lgpl.a
CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibm.a
CompilerANSILibs += $(MV_TOOLS_MOVI_COMPILE_LIB_PATH)/$(MVISAOPT)/mlibcrt.a
CompilerANSILibs += --end-group



# -------------------------------- [ Build Options ] ------------------------------ #
# App related build options
# Extra app related options
#CCOPT			+= -DDEBUG
DISABLE_ERROR_ON_WARNINGS=yes

TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150,TCL_MA2150, MA2450, MA2480"


test:
	@echo "OK"
