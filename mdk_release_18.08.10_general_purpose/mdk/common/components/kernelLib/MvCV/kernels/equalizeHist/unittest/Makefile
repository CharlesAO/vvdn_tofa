KERNEL_ASM_LABEL = mvcvEqualizeHist_asm

HISTOGRAM_DIR   = ../../histogram/shave
HISTOGRAM_OBJ = ./out/histogram.o

CPPFLAGS += -I$(HISTOGRAM_DIR)

$(HISTOGRAM_OBJ) : $(HISTOGRAM_DIR)/src/cpp/histogram.cpp $(HISTOGRAM_DIR)/include/histogram.h
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

EXTRA_LIBS = $(HISTOGRAM_OBJ)
include ../../../shared/unittest/test_common.mk
