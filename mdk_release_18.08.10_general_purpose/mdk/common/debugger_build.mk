#################################################################################
#                         Build rules for the trace converter                   #
#################################################################################

MVDBGCONV_SOURCES = $(wildcard $(MV_COMMON_BASE)/utils/dbgConverter/*.c)
MVDBGCONV_HEADERS = $(wildcard $(MV_COMMON_BASE)/utils/dbgConverter/*.h)
PC_GCC = gcc

$(MVDBGCONV) : $(MVDBGCONV_SOURCES) $(MVDBGCONV_HEADERS)
	@echo "Building the waves converter PC utility"
	$(ECHO) $(PC_GCC) -Wall $(MVDBGCONV_SOURCES) -o $(MVDBGCONV)
