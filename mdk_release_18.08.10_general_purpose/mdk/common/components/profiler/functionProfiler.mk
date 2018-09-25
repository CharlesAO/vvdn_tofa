########################################################################################
##
##	Configuration section
##
##  The following variables affect the profiling, you can define them in your Makefile
##	in order to fine tune the profiling
##
########################################################################################

# by default profile everything
FP_CORE_PROFILE ?= los lrt shave

# Default buffer size. More memory increases the accuracy
FP_LOS_BUFFER_SIZE ?= 4194304
FP_LRT_BUFFER_SIZE ?= 4194304
FP_SHV_BUFFER_SIZE ?= 1048576

# In what section to place the buffer
FP_LOS_DATA_SECTION ?= .ddr_direct.bss
FP_LRT_DATA_SECTION ?= .ddr_direct.bss
FP_SHV_DATA_SECTION ?= .ddr_direct.bss

FP_LOS_TIMER ?= TIM0_BASE_ADR
FP_LRT_TIMER ?= TIM0_BASE_ADR
FP_SHV_TIMER ?= TIM0_BASE_ADR

ifeq ($(FP_COLLECT_STALLS), true)
$(info Stalls collect activated)
FP_ENABLE_COUNTER0 := 0x4
FP_ENABLE_COUNTER1 := 0x7FF0001
# next two are untested with eclipse/lttng
override undefine FP_ENABLE_COUNTER2
override undefine FP_ENABLE_COUNTER3
endif

# Hooks section
# FP_LEON_FCT_ENTER_HOOK - if defined, represent code that will be executed on leon BM
# every time a function is entered
# FP_LEON_FCT_EXIT_HOOK - exacly the same, but on exit
# FP_RTEMS_FCT_ENTER_HOOK, FP_RTEMS_FCT_EXIT_HOOK - on leon os (with rtems)
# FP_SHV_INIT_HOOK - at init shave before bzero-ing the buffer
# FP_SHV_FCT_ENTER_HOOK, FP_SHV_FCT_EXIT_HOOK - for shaves

# The following objects will be compiled with internal profiling flags
# If new code is required to alter profiling functionality, add objects here
FP_SHV_CODE ?= %/shaveHooks.asmgen %/functionProfiler.asmgen %/profile.asmgen
FP_LOS_CODE ?= %/profiler/leon/leonHooks.o \
			   %/profiler/leon/fpBareMetalHooks.o \
			   %/profiler/leon/fpRtemsHooks.o \
			   %/profiler/shared/functionProfiler.o \
			   %/profiler/shared/profile.o
FP_LRT_CODE ?= %/profiler/leon/leonHooks_lrt.o \
			   %/profiler/leon/fpBareMetalHooks_lrt.o \
			   %/profiler/leon/fpRtemsHooks_lrt.o \
			   %/profiler/shared/functionProfiler_lrt.o \
			   %/profiler/shared/profile_lrt.o

PROFILER_ARGUMENTS += fpCore = \"$(FP_CORE_PROFILE)\";
PROFILER_ARGUMENTS += fpBufferSize = \"$(FP_LOS_BUFFER_SIZE) $(FP_LRT_BUFFER_SIZE) $(FP_SHV_BUFFER_SIZE)\";
PROFILER_ARGUMENTS += fpCollectStalls = \"$(FP_COLLECT_STALLS)\";
PROFILER_ARGUMENTS += fpDataSection = \"$(FP_LOS_DATA_SECTION) $(FP_LRT_DATA_SECTION) $(FP_SHV_DATA_SECTION)\";


########################################################################################
##
##	END of configuration section
##
########################################################################################

ifeq ($(FP_LTO), true)
LOS_PROFILER_OPTIONS += -flto
LRT_PROFILER_OPTIONS += -flto
SHV_PROFILER_OPTIONS += -flto
LDOPT += -flto
endif

########################################################################################
# if Leon OS is being profiled
########################################################################################
ifneq (,$(findstring los,$(FP_CORE_PROFILE)))
ComponentList_LOS += profiler
LOS_PROFILER_OPTIONS += -DFPROFILER_BUFFER_SIZE="$(FP_LOS_BUFFER_SIZE)" \
	-DFPROFILER_DATA_SECTION="__attribute__((section(\"$(FP_LOS_DATA_SECTION)\")))" \
	-DFP_TIMER_ADDR=$(FP_LOS_TIMER) \
	-DLOS_PROFILE

ifeq ($(FP_32BIT_TIMESTAMP_OPT_MODE), true)
LOS_PROFILER_OPTIONS += -DFP_32BIT_TIMESTAMP_OPT_MODE
endif

ifneq (,$(findstring ddr,$(FP_LOS_DATA_SECTION)))
# TODO: same for other cores
LOS_PROFILER_OPTIONS += "-DFP_TODO_CHECK_IF_DDR_IS_INITIALIZED"
endif

$(call mkrule,$(FP_LOS_CODE),CCOPT,$(LOS_PROFILER_OPTIONS))

ifdef FP_LOS_INSTRUMENT_ONLY
$(call mkrule,$(FP_LOS_INSTRUMENT_ONLY),CCOPT_EXTRA,-finstrument-functions)
PROFILER_ARGUMENTS += fpLOSInstrumentOnly = \"$(FP_LOS_INSTRUMENT_ONLY)\";
else
CCOPT_EXTRA		+=	-finstrument-functions
endif # FP_LOS_INSTRUMENT_ONLY

ifdef FP_LOS_EXCLUDE_FILE_LIST
CCOPT_EXTRA += -finstrument-functions-exclude-file-list=$(FP_LOS_EXCLUDE_FILE_LIST)
endif
ifdef FP_LOS_EXCLUDE_FUNCTION_LIST
CCOPT_EXTRA += -finstrument-functions-exclude-function-list=$(FP_LOS_EXCLUDE_FUNCTION_LIST)
endif

$(info Leon OS profiling activated)
endif
########################################################################################


########################################################################################
# if Leon RT is being profiled
########################################################################################
ifneq (,$(findstring lrt,$(FP_CORE_PROFILE)))
ComponentList_LRT += profiler
# TODO: the following should be available only for hooks.c compile, and not for all sources
LRT_PROFILER_OPTIONS += -DFPROFILER_BUFFER_SIZE="$(FP_LRT_BUFFER_SIZE)" \
	-DFPROFILER_DATA_SECTION="__attribute__((section(\"$(FP_LRT_DATA_SECTION)\")))" \
	-DFP_TIMER_ADDR="$(FP_LRT_TIMER)" \
	-DLRT_PROFILE

ifeq ($(FP_32BIT_TIMESTAMP_OPT_MODE), true)
LRT_PROFILER_OPTIONS += -DFP_32BIT_TIMESTAMP_OPT_MODE
endif

$(call mkrule,$(FP_LRT_CODE),CCOPT_LRT,$(LRT_PROFILER_OPTIONS))

ifdef FP_LRT_INSTRUMENT_ONLY
$(call mkrule,$(FP_LRT_INSTRUMENT_ONLY),CCOPT_EXTRA_LRT,-finstrument-functions)
PROFILER_ARGUMENTS += fpLRTInstrumentOnly = \"$(FP_LRT_INSTRUMENT_ONLY)\";
else
CCOPT_EXTRA_LRT	+=	-finstrument-functions
endif # FP_LRT_INSTRUMENT_ONLY

ifdef FP_LRT_EXCLUDE_FILE_LIST
CCOPT_EXTRA_LRT += -finstrument-functions-exclude-file-list=$(FP_LRT_EXCLUDE_FILE_LIST)
endif
ifdef FP_LRT_EXCLUDE_FUNCTION_LIST
CCOPT_EXTRA_LRT += -finstrument-functions-exclude-function-list=$(FP_LRT_EXCLUDE_FUNCTION_LIST)
endif

# hook on target
$(info Leon RT profiling activated)
endif
########################################################################################


########################################################################################
# if shaves are being profiled
########################################################################################
ifneq (,$(findstring shave,$(FP_CORE_PROFILE)))
# "Spin me a lie and we'll slip through the haze
#  Just like Louise we'll get lost in the SHAVES"
#				Tristania - Year of the Rat

ComponentList_SVE += profiler
SHAVE_COMPONENTS = yes

SHV_PROFILER_OPTIONS += -DFPROFILER_BUFFER_SIZE="$(FP_SHV_BUFFER_SIZE)" \
	-DFPROFILER_DATA_SECTION="__attribute__((section(\"$(FP_SHV_DATA_SECTION)\")))" \
	-DFP_TIMER_ADDR="$(FP_SHV_TIMER)" \
	-DSHAVE_FPROFILE

ifeq ($(FP_32BIT_TIMESTAMP_OPT_MODE), true)
SHV_PROFILER_OPTIONS += -DFP_32BIT_TIMESTAMP_OPT_MODE
endif

ifdef FP_ENABLE_COUNTER0
SHV_PROFILER_OPTIONS += "-DFP_ENABLE_COUNTER0=$(FP_ENABLE_COUNTER0)"
endif
ifdef FP_ENABLE_COUNTER1
SHV_PROFILER_OPTIONS += "-DFP_ENABLE_COUNTER1=$(FP_ENABLE_COUNTER1)"
endif
ifdef FP_ENABLE_COUNTER2
SHV_PROFILER_OPTIONS += "-DFP_ENABLE_COUNTER2=$(FP_ENABLE_COUNTER2)"
endif
ifdef FP_ENABLE_COUNTER3
SHV_PROFILER_OPTIONS += "-DFP_ENABLE_COUNTER3=$(FP_ENABLE_COUNTER3)"
endif

$(call mkrule,$(FP_SHV_CODE),MVCCOPT,$(SHV_PROFILER_OPTIONS))

FP_SHV_INSTRUMENT_FLAG ?= -finstrument-functions-after-inlining

ifdef FP_SHV_INSTRUMENT_ONLY
$(call mkrule,$(FP_SHV_INSTRUMENT_ONLY),MVCCOPT,$(FP_SHV_INSTRUMENT_FLAG))
PROFILER_ARGUMENTS += fpSHVInstrumentOnly = \"$(FP_SHV_INSTRUMENT_ONLY)\";
else
MVCCOPT += $(FP_SHV_INSTRUMENT_FLAG)
endif # FP_SHV_INSTRUMENT_ONLY

ifdef FP_SHV_EXCLUDE_FILE_LIST
MVCCOPT += -finstrument-functions-exclude-file-list=$(FP_SHV_EXCLUDE_FILE_LIST)
endif
ifdef FP_SHV_EXCLUDE_FUNCTION_LIST
MVCCOPT += -finstrument-functions-exclude-function-list=$(FP_SHV_EXCLUDE_FUNCTION_LIST)
endif

ifdef FP_SHV_CTORS_CALLED
MVCCOPT += -DFP_SHV_CTORS_CALLED
endif

$(info Shave profiling activated)
endif # FP_CORE_PROFILE contains shave
########################################################################################
