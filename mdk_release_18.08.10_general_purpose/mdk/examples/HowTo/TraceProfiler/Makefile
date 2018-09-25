# ------------------------------[  Trace Settings  ]--------------------------------------#

# Use TraceProfiling Sink
PROFILE_INFO+=,trace
CCOPT_EXTRA+=-DSINK_FUNCTION=_trace_print
TRACE_BUFFER_SIZE=1000000

#CCOPT_EXTRA+=-DSINK_FUNCTION=puts
# CCOPT_EXTRA+=-DSINK_FUNCTION=printf
#CCOPT_EXTRA+=-DSINK_FUNCTION=\(void\)

#MVCCOPT+=-DMV_UNIT_NAME=mysv

# disable logging with 0
# MVCCOPT+=-DDEFAULT_LOG_LEVEL='0'
# can be disabled with
# MVCCOPT+=-DMAX_STATIC_LOG_LEVEL='0'

# max log level
# MVCCOPT+=-DDEFAULT_LOG_LEVEL='6'

# MVCCOPT+=-DSINK_FUNCTION=puts

# bulk data
CCOPT_EXTRA+=-DSINK_BULK=_vcs_save_file

CCOPT_EXTRA+=-DDBG_ARG1=DBG_PRINT_FILE_LINE \
		 -DDBG_ARG2=DBG_PRINT_MODULE_NAME \
		 -DDBG_ARG3=DBG_PRINT_TIMESTAMP \
		 -DDBG_ARG4=DBG_PRINT_LOG_LEVEL \
		 -DDBG_ARG5=DBG_PRINT_THREAD \
		 -DDBG_ARG6=DBG_PRINT_CORE_ID
CCOPT_EXTRA_LRT+=-DDBG_ARG1=DBG_PRINT_FILE_LINE \
		 -DDBG_ARG4=DBG_PRINT_LOG_LEVEL \
		 -DDBG_ARG5=DBG_PRINT_THREAD \
		 -DDBG_ARG6=DBG_PRINT_CORE_ID
MVCCOPT+=-DDBG_ARG2=DBG_PRINT_FILE_LINE \
		 -DDBG_ARG3=DBG_PRINT_TIMESTAMP \
		 -DDBG_ARG4=DBG_PRINT_CORE_ID

MVCCOPT+=-DSYNC_LOG_MSG

# ------------------------------[ General Settings ]--------------------------------------#
MV_SOC_PLATFORM ?= myriad2
MV_SOC_REV      ?= ma2450

LEON_RT_BUILD = yes

MV_SOC_OS = rtems


# Set MV_COMMON_BASE relative to mdk directory location (but allow user to override in environment)
MV_COMMON_BASE   ?= ../../../common

# ------------------------------[ Tools ]------------------------------------------#
# Hardcode tool version here if needed, otherwise defaults to revision defined in mdk/common/tools_settings.mk
# Warning: This is really not recommended

#------------------------------[ Build overrides ]--------------------------------#
# Note: If your project needs a custom linker script create a file called custom.ldscript
# in the project config folder. This will be automatically picked up.


# ------------------------------[ Components used ]--------------------------------#

# This provides the unit test framework and hooks into the VCS environment

ComponentList_LOS  := VcsHooks PipePrint FatalExtension
ComponentList_LRT  := VcsHooks PipePrint
ComponentList_SVE := kernelLib/MvCV
SHAVE_COMPONENTS = no

#----------------------[ Local SHAVE applications sources ]-----------------------------#
#Application
TraceProfillerApp = shave/TraceProfiller

#Choosing C shave local sources
SHAVE_C_SOURCES_TraceProfiller = $(wildcard $(DirAppRoot)/shave/*.c)
#Choosing asm shave local sources
SHAVE_ASM_SOURCES_TraceProfiller  = $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution7x7/arch/ma2x5x/shave/src/convolution7x7.asm
SHAVE_ASM_SOURCES_TraceProfiller  += $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution3x3/arch/ma2x5x/shave/src/convolution3x3.asm

# C compilation is a two stage process, this rule declares the intermediate asmgen file list
SHAVE_GENASMS_TraceProfiller = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_TraceProfiller))

#Generating the required objects list from  all local shave sources
SHAVE_OBJ_TraceProfiller = $(patsubst %.c,$(DirAppObjBase)%_shave.o,$(SHAVE_C_SOURCES_TraceProfiller) )
SHAVE_OBJ_TraceProfiller += $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_TraceProfiller) )

#Update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_TraceProfiller) $(SHAVE_OBJ_TraceProfiller)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_TraceProfiller)
#---------------------------------------------------------------------------------------#


#--------------------------[ SHAVE SYSTEM BUILD ]--------------------------#

#Creating the mvlib file containing the application
SHAVE_APP_LIBS = $(TraceProfillerApp).mvlib

#Placing application libs on the 12 cores
SHAVE0_APPS = $(TraceProfillerApp).shv0lib
SHAVE1_APPS = $(TraceProfillerApp).shv1lib
SHAVE2_APPS = $(TraceProfillerApp).shv2lib

SHAVE3_APPS = $(TraceProfillerApp).shv3lib
SHAVE4_APPS = $(TraceProfillerApp).shv4lib
SHAVE5_APPS = $(TraceProfillerApp).shv5lib

SHAVE6_APPS = $(TraceProfillerApp).shv6lib
SHAVE7_APPS = $(TraceProfillerApp).shv7lib
SHAVE8_APPS = $(TraceProfillerApp).shv8lib

SHAVE9_APPS = $(TraceProfillerApp).shv9lib
SHAVE10_APPS = $(TraceProfillerApp).shv10lib
SHAVE11_APPS = $(TraceProfillerApp).shv11lib

#add the input image object to RAWDATAOBJECTFILES
RAWDATAOBJECTFILES += $(DirAppObjDir)/testframe.o

# Include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk


SourceDebugScript = ./scripts/trace_run_mdbg2.tcl

#Describe the rule for building the TraceProfiller application.
#Simple rule specifying which objects build up the application.
#The application will be built into a library

ENTRYPOINTS = -e ApplicationStart  --gc-sections

$(TraceProfillerApp).mvlib : $(SHAVE_OBJ_TraceProfiller) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS) $(MVLIBOPT) $(SHAVE_OBJ_TraceProfiller) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

#----------[ Include the TraceProfiller application into GLOBAL system build ]------------#
#---------------------------------[ Include the generic Makefile ]------------------------#


#-----------------------------------------------------------------------------------------#


#----------[ Add an input image ]------------------------------------------------#
#The input image location
MY_RESOURCE = $(MV_EXTRA_DATA)/CobKitchen_1280x720.yuv

$(DirAppObjDir)/testframe.o: $(MY_RESOURCE) Makefile
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary $(REVERSE_BYTES) --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=lrt_inputFrame \
	-O elf32-sparc -B sparc $< $@
#---------------------------------------------------------------------#

# Automated Test Settings
# TEST_TAGS        := "MA2150,TCL_MA2150, MA2450"
TEST_TYPE:="MANUAL"

