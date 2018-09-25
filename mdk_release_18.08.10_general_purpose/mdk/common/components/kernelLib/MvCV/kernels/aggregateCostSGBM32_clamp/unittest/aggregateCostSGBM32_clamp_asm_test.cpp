#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (1.85)
#define WIDTH 32
#define PADDING 16 

TestRunner mvcvAggregateCostSGBM32clampTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvAggregateCostSGBM32clampCycleCount;

void mvcvAggregateCostSGBM32_clamp_asm_test(unsigned char *input1, unsigned char *input2, unsigned char *output, unsigned short penaltyP1,  unsigned short penaltyP2,  unsigned char min2, unsigned char activateClamp)

{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvAggregateCostSGBM32clampTestRunner.Init();
	mvcvAggregateCostSGBM32clampTestRunner.SetInput(	"input1", input1, WIDTH, SHAVE0);
	mvcvAggregateCostSGBM32clampTestRunner.SetInput(	"input2", input2, WIDTH, SHAVE0);
	mvcvAggregateCostSGBM32clampTestRunner.SetInput(	"output", output, WIDTH + PADDING, SHAVE0);
	mvcvAggregateCostSGBM32clampTestRunner.SetInput(	"penaltyP1", penaltyP1, SHAVE0);
	mvcvAggregateCostSGBM32clampTestRunner.SetInput(	"penaltyP2", penaltyP2, SHAVE0);
	mvcvAggregateCostSGBM32clampTestRunner.SetInput(    "min2", min2, SHAVE0);
	mvcvAggregateCostSGBM32clampTestRunner.SetInput(	"activateClamp", activateClamp, SHAVE0);
 	mvcvAggregateCostSGBM32clampTestRunner.GuardInsert(string("output"), SHAVE0, WIDTH + PADDING, output);
 	mvcvAggregateCostSGBM32clampTestRunner.Run(SHAVE0);

	mvcvAggregateCostSGBM32clampCycleCount = (mvcvAggregateCostSGBM32clampTestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvAggregateCostSGBM32clampCycleCount)/ (float)(WIDTH));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	mvcvAggregateCostSGBM32clampTestRunner.GetOutput(string("output"), SHAVE0, WIDTH + PADDING, output);
	mvcvAggregateCostSGBM32clampTestRunner.GuardCheck(string("output"), SHAVE0, WIDTH + PADDING, output);
}
