$(info Sample profiling activated)

########################################################################################
##
##	Configuration section
##
##  The following variables affect the profiling, you can define them in your Makefile
##	in order to fine tune the profiling
##
########################################################################################

SAMPLE_PERIOD_MICRO?=10
SP_TIMER_INTERRUPT_LEVEL?=13
SAMPLE_DURATION_MS?=14000
SAMPLE_PROFILER_DATA_SECTION?=__attribute__((section(\".ddr_direct.bss\")))

MDBG_INIT_HOOK += -D:SAMPLE_PERIOD_MICRO=$(SAMPLE_PERIOD_MICRO)

########################################################################################
##
##	END of configuration section
##
########################################################################################


# sample profiler code configuration
PROFILER_OPTIONS = -DSAMPLE_PERIOD_MICRO="$(SAMPLE_PERIOD_MICRO)" \
    -DSAMPLE_PROFILER_DATA_SECTION="$(SAMPLE_PROFILER_DATA_SECTION)" \
    -DSP_TIMER_INTERRUPT_LEVEL="$(SP_TIMER_INTERRUPT_LEVEL)" \
    -DSAMPLE_DURATION_MS="$(SAMPLE_DURATION_MS)" \
    -DSAMPLE_PROFILING

ifdef SAMPLE_PROF_COMPUTE_WINDOW
PROFILER_OPTIONS += -DSAMPLE_PROF_COMPUTE_WINDOW
PROFILER_ARGUMENTS += sampleProfComputeWnd = \"yes\";
endif

SAMPLE_PROFILER_OBJ=%/leon/sampleProfiler.o
SAMPLE_PROFILER_OBJ_LRT=%/leon/sampleProfiler_lrt.o
$(call mkrule,$(SAMPLE_PROFILER_OBJ),CCOPT_EXTRA,$(PROFILER_OPTIONS))
$(call mkrule,$(SAMPLE_PROFILER_OBJ_LRT),CCOPT_EXTRA_LRT,$(PROFILER_OPTIONS))

# redefine symbols in swcShaveLoader during compile time
SWC_SHAVE_LOADER_SYM_REDEFINE := -DswcWaitShave=swcWaitShave_original \
	-DswcWaitShaves=swcWaitShaves_original \
	-DswcStartShave=swcStartShave_original \
	-DswcDynStartShave=swcDynStartShave_original \
	-DswcRunShaveAlgo=swcRunShaveAlgo_original \
	-DswcRunShaveAlgoCC=swcRunShaveAlgoCC_original \
	-DswcStartShaveCC=swcStartShaveCC_original \
	-DswcStartShaveAsync=swcStartShaveAsync_original \
	-DswcShaveStartAsync=swcShaveStartAsync_original \
	-DswcStartShaveAsyncCC=swcStartShaveAsyncCC_original \
	-DswcAssignShaveCallback=swcAssignShaveCallback_original

# $(call mkrule,%/swcShaveLoader.o,CCOPT_EXTRA,$(SWC_SHAVE_LOADER_SYM_REDEFINE))
# $(call mkrule,%/swcShaveLoader_lrt.o,CCOPT_EXTRA_LRT,$(SWC_SHAVE_LOADER_SYM_REDEFINE))

# insert the profiler into the build chain
ComponentList_LOS += profiler
ComponentList_LRT += profiler

