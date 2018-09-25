KERNEL_ASM_LABEL = mvcvMeshExpand_asm

MESHGENERATE_DIR   = ../../meshGenerate/shave
MESHGENERATE_OBJ = ./out/meshGenerate.o

CPPFLAGS += -I$(MESHGENERATE_DIR)

$(MESHGENERATE_OBJ) : $(MESHGENERATE_DIR)/src/cpp/meshGenerate.cpp $(MESHGENERATE_DIR)/include/meshGenerate.h
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

EXTRA_LIBS = $(MESHGENERATE_OBJ)
include ../../../shared/unittest/test_common.mk
