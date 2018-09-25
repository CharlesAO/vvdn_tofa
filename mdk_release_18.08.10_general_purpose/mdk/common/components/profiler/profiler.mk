PGO_OUT_FILE ?= ./mvprof.profraw
ifndef PROFILE_INFO
# not in a profiling mode
ifneq ("$(wildcard $(PGO_OUT_FILE))", "")
# and have some results
MVCCOPT +=-fprofile-instr-use=$(PGO_OUT_FILE) -Wno-profile-instr-unprofiled
# TODO: moviCompile give profile-instr-out-of-date warning also for unmodified files. investigate why
MVCCOPT += -Wno-profile-instr-out-of-date
endif
endif

ifdef PROFILE_INFO

ifeq ($(OS),Windows_NT)
PROFILE_DECODER =  $(shell cygpath -m $(realpath $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviProf.exe))
else
PROFILE_DECODER = $(shell realpath $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)/bin/moviProf)
endif

# Usage: $(call mkrule,list,var,value)
# for each file from list, var += value
# used to set compile flags per file
define PROFILER_CODE_RULE
$(1): $(2) += $(3)
endef
define mkrule
$(eval $(foreach f,$(1),$(eval $(call PROFILER_CODE_RULE,$(f),$(2),$(3)))))
endef

SELF_PATH := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
COMPONENT_PATH = $(MV_COMMON_BASE)/components/profiler
PROFILER_INIT_HOOK_SCRIPT ?= $(COMPONENT_PATH)/init.tcl
PROFILER_EXEC_TIME := $(shell date "+%Y.%m.%d-%H.%M.%S")
PROFILER_OUTPUT_DIR ?= $(DirAppOutput)/profile
PROFILER_DURATION_MS ?= 60000

MDBG_INIT_HOOK = --script $(PROFILER_INIT_HOOK_SCRIPT)

# User defined string. Freetext filled by the user
#PROFILER_SESSION_DESCRIPTION ?=

###############################################
## Function Profiling
###############################################
ifneq (,$(findstring function,$(PROFILE_INFO)))
override PROFILE_INFO+=trace
include $(SELF_PATH)/functionProfiler.mk
endif # PROFILE == function

###############################################
## Sample Profiling
###############################################
ifneq (,$(findstring sample,$(PROFILE_INFO)))
override PROFILE_INFO+=trace
include $(SELF_PATH)/sampleProfiler.mk
endif # PROFILE == sample

###############################################
## Trace Profiling
###############################################
ifneq (,$(findstring trace,$(PROFILE_INFO)))
include $(SELF_PATH)/traceProfiler.mk
endif

###############################################
## PGO Profiling
###############################################
ifneq (,$(findstring pgo,$(PROFILE_INFO)))
include $(SELF_PATH)/pgoProfiler.mk
endif

DirAppOutput := $(PROFILER_OUTPUT_DIR)

PROFILER_SESSION_PATH ?= profile_session
PROFILER_SESSION_NAME ?= profile_$(PROFILER_EXEC_TIME)
PROFILER_SESSION_NAME := $(subst %now%,$(PROFILER_EXEC_TIME),$(PROFILER_SESSION_NAME))
PROFILER_SESSION_DIR := $(PROFILER_SESSION_PATH)/$(PROFILER_SESSION_NAME)

# list of arguments separated by ;
PROFILER_ARGUMENTS += profilerDescription = \"$(PROFILER_SESSION_DESCRIPTION)\";
GIT_AVAIL := $(shell command -v git 2>/dev/null 1>&2 && git -C . rev-parse 2>/dev/null && echo "yes" || echo "no")
ifeq ($(GIT_AVAIL),yes)
PROFILER_ARGUMENTS += gitVersion = \"$(shell git rev-parse --short HEAD) ($(shell git symbolic-ref --quiet HEAD))\";
endif
PROFILER_ARGUMENTS += profilerType = \"$(PROFILE_INFO)\";
PROFILER_ARGUMENTS += MV_SOC_REV = \"$(MV_SOC_REV)\";
PROFILER_ARGUMENTS += MV_TOOLS_BASE = \"$(MV_TOOLS_BASE)\";
PROFILER_ARGUMENTS += MV_BUILD_CONFIG = \"$(MV_BUILD_CONFIG)\";

MDBG_INIT_HOOK += -D:moviProf="$(PROFILE_DECODER) -o $(PROFILER_SESSION_DIR)" \
	-D:timeout=$(PROFILER_DURATION_MS) -D:profArgs="$(PROFILER_ARGUMENTS)"
MDBG_INIT_HOOK += -DSESSION_DIR=$(PROFILER_SESSION_DIR)

ifdef PRF_SYM_FILE
MDBG_INIT_HOOK += -DPRF_SYM_FILE=$(PRF_SYM_FILE)
endif

run debug: MVDBG_SCRIPT_OPT += $(MDBG_INIT_HOOK)
debug: PROFILER_DURATION_MS=-1
profile: run

endif # PROFILE_INFO defined
