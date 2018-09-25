MV_COMMON_BASE   ?= $(COMMON_DIR)
LEON_STARTUP_MASK?= LRT
M2_SW_UNIT_TEST  ?= yes
ComponentList    := UnitTestVcs VcsHooks Opipe Fp16Convert
MAKEDEPEND       ?= no

CCOPT += -D'CDMA_USE_FAST_MTX'
CCOPT += -D'DEFAULT_HEAP_SIZE=512'

#Include the top makefile
include $(MV_COMMON_BASE)/generic.mk

getSym:
	$(OBJDUMP) -t ./$(ELF_FILE) | sort > ./$(DirAppOutput)/$(APPNAME)_leon.sym
