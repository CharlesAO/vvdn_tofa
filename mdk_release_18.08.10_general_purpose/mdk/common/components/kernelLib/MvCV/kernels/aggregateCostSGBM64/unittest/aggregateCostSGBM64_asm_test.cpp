#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.83)
#define WIDTH 64
#define PADDING 16 

TestRunner mvcvAggregateCostSGBM64TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvAggregateCostSGBM64CycleCount;

void mvcvAggregateCostSGBM64_asm_test(unsigned char *input1, unsigned char *input2, unsigned char *output, unsigned char min2, unsigned char penaltyP2)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvAggregateCostSGBM64TestRunner.Init();
	mvcvAggregateCostSGBM64TestRunner.SetInput(   "input1", input1, WIDTH, SHAVE0);
	mvcvAggregateCostSGBM64TestRunner.SetInput(   "input2", input2, WIDTH, SHAVE0);
	mvcvAggregateCostSGBM64TestRunner.SetInput(   "output", output, WIDTH + PADDING, SHAVE0);
	mvcvAggregateCostSGBM64TestRunner.SetInput(     "min2",      (unsigned char)min2, SHAVE0);
    mvcvAggregateCostSGBM64TestRunner.SetInput("penaltyP2", (unsigned char)penaltyP2, SHAVE0);

	mvcvAggregateCostSGBM64TestRunner.GuardInsert(string("output"), SHAVE0, WIDTH + PADDING, output);
	mvcvAggregateCostSGBM64TestRunner.Run(SHAVE0);

	mvcvAggregateCostSGBM64CycleCount = (mvcvAggregateCostSGBM64TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvAggregateCostSGBM64CycleCount)/ (float)(WIDTH));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	mvcvAggregateCostSGBM64TestRunner.GetOutput(string("output"), SHAVE0, WIDTH + PADDING, output);
	mvcvAggregateCostSGBM64TestRunner.GuardCheck(string("output"), SHAVE0, WIDTH + PADDING, output);
}
