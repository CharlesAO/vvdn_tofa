KERNEL_ASM_LABEL = mvcvWarpMeshSample8bitStride_asm

WARPMESH_DIR   = ../../warpMeshExpand/shave
WARPMESH_OBJ = ./out/warpMeshExpand.o

CPPFLAGS += -I$(WARPMESH_DIR)

$(WARPMESH_OBJ) : $(WARPMESH_DIR)/src/cpp/warpMeshExpand.cpp $(WARPMESH_DIR)/include/warpMeshExpand.h
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

EXTRA_LIBS = $(WARPMESH_OBJ)
include ../../../shared/unittest/test_common.mk
TEST_TIMEOUT_SEC := 2500
