#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (3.46)
#define WIDTH 64
#define PADDING 16 

TestRunner mvcvAggregateCostSGBM64clampTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvAggregateCostSGBM64clampCycleCount;

void mvcvAggregateCostSGBM64_clamp_asm_test(unsigned char *input1, unsigned char *input2, unsigned char *output, unsigned short penaltyP1,  unsigned short penaltyP2,  unsigned char min2, unsigned char activateClamp)

{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvAggregateCostSGBM64clampTestRunner.Init();
	mvcvAggregateCostSGBM64clampTestRunner.SetInput(	"input1", input1, WIDTH, SHAVE0);
	mvcvAggregateCostSGBM64clampTestRunner.SetInput(	"input2", input2, WIDTH, SHAVE0);
	mvcvAggregateCostSGBM64clampTestRunner.SetInput(	"output", output, WIDTH + PADDING, SHAVE0);
	mvcvAggregateCostSGBM64clampTestRunner.SetInput(	"penaltyP1", penaltyP1, SHAVE0);
	mvcvAggregateCostSGBM64clampTestRunner.SetInput(	"penaltyP2", penaltyP2, SHAVE0);
	mvcvAggregateCostSGBM64clampTestRunner.SetInput(    "min2", min2, SHAVE0);
	mvcvAggregateCostSGBM64clampTestRunner.SetInput(	"activateClamp", activateClamp, SHAVE0);
 	mvcvAggregateCostSGBM64clampTestRunner.GuardInsert(string("output"), SHAVE0, WIDTH + PADDING, output);
 	mvcvAggregateCostSGBM64clampTestRunner.Run(SHAVE0);

	mvcvAggregateCostSGBM64clampCycleCount = (mvcvAggregateCostSGBM64clampTestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvAggregateCostSGBM64clampCycleCount)/ (float)(WIDTH));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	mvcvAggregateCostSGBM64clampTestRunner.GetOutput(string("output"), SHAVE0, WIDTH + PADDING, output);
	mvcvAggregateCostSGBM64clampTestRunner.GuardCheck(string("output"), SHAVE0, WIDTH + PADDING, output);
}
