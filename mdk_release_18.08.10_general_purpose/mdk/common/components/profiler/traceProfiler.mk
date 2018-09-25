TRACE_BUFFER_SIZE ?= 1024
PROFILER_ARGUMENTS += traceProfileBufferSize = \"$(TRACE_BUFFER_SIZE)\";

TRACE_PROF_DEFINES += -DTRACE_PROFILER_ENABLED \
					  -DTRACE_BUFFER_SIZE=$(TRACE_BUFFER_SIZE)

ComponentList_LOS += profiler
ComponentList_LRT += profiler
ComponentList_SVE += profiler
SHAVE_COMPONENTS = yes

# enable trace profiling code, otherwise only stubs are in place
CCOPT_EXTRA += $(TRACE_PROF_DEFINES)
CCOPT_EXTRA_LRT += $(TRACE_PROF_DEFINES)

# doesn't fit in all cases
ifdef TRACE_PROFILE_SHAVE
MVCCOPT += $(TRACE_PROF_DEFINES)
endif
