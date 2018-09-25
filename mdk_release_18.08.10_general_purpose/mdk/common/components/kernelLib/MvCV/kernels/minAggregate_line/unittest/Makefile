KERNEL_ASM_LABEL = mvcvMinAggregate_line_asm



MINKERNEL32_DIR   = ../../minKernel32/shave
MINKERNEL32_OBJ = ./out/minKernel32.o

CPPFLAGS += -I$(MINKERNEL32_DIR)

$(MINKERNEL32_OBJ) : $(MINKERNEL32_DIR)/src/cpp/minKernel32.cpp $(MINKERNEL32_DIR)/include/minKernel32.h
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

EXTRA_LIBS += $(MINKERNEL32_OBJ)


MINKERNEL64_DIR   = ../../minKernel64/shave
MINKERNEL64_OBJ = ./out/minKernel64.o

CPPFLAGS += -I$(MINKERNEL64_DIR)

$(MINKERNEL64_OBJ) : $(MINKERNEL64_DIR)/src/cpp/minKernel64.cpp $(MINKERNEL64_DIR)/include/minKernel64.h
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

EXTRA_LIBS += $(MINKERNEL64_OBJ)




AGGREGATECOSTSGBM32_DIR   = ../../aggregateCostSGBM32_clamp/shave
AGGREGATECOSTSGBM32_OBJ = ./out/aggregateCostSGBM32_clamp.o

CPPFLAGS += -I$(AGGREGATECOSTSGBM32_DIR)

$(AGGREGATECOSTSGBM32_OBJ) : $(AGGREGATECOSTSGBM32_DIR)/src/cpp/aggregateCostSGBM32_clamp.cpp $(AGGREGATECOSTSGBM32_DIR)/include/aggregateCostSGBM32_clamp.h
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

EXTRA_LIBS += $(AGGREGATECOSTSGBM32_OBJ)


AGGREGATECOSTSGBM64_DIR   = ../../aggregateCostSGBM64_clamp/shave
AGGREGATECOSTSGBM64_OBJ = ./out/aggregateCostSGBM64_clamp.o

CPPFLAGS += -I$(AGGREGATECOSTSGBM64_DIR)

$(AGGREGATECOSTSGBM64_OBJ) : $(AGGREGATECOSTSGBM64_DIR)/src/cpp/aggregateCostSGBM64_clamp.cpp $(AGGREGATECOSTSGBM64_DIR)/include/aggregateCostSGBM64_clamp.h
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

EXTRA_LIBS += $(AGGREGATECOSTSGBM64_OBJ)



include ../../../shared/unittest/test_common.mk










