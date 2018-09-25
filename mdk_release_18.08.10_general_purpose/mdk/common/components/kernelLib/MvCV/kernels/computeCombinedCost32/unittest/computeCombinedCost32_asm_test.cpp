#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.50)
#define DISPARITIES 32

TestRunner mvcvComputeCombinedCost32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvComputeCombinedCost32CycleCount;

void mvcvComputeCombinedCost32_asm_test(unsigned char* disparityCost, unsigned char* adCost, unsigned int alpha, unsigned int beta, unsigned int width, unsigned int normFactor)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvComputeCombinedCost32TestRunner.Init();
	mvcvComputeCombinedCost32TestRunner.SetInput(   "disparityCost", disparityCost, width * DISPARITIES, SHAVE0);
	mvcvComputeCombinedCost32TestRunner.SetInput(   "adCost", adCost, width * DISPARITIES, SHAVE0);
    mvcvComputeCombinedCost32TestRunner.SetInput(   "alpha", (unsigned int)alpha, SHAVE0);
    mvcvComputeCombinedCost32TestRunner.SetInput(   "beta", (unsigned int)beta, SHAVE0);
    mvcvComputeCombinedCost32TestRunner.SetInput(   "width", (unsigned int)width, SHAVE0);
    mvcvComputeCombinedCost32TestRunner.SetInput(   "normFactor", (unsigned int)normFactor, SHAVE0);

	mvcvComputeCombinedCost32TestRunner.GuardInsert(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
	mvcvComputeCombinedCost32TestRunner.Run(SHAVE0);

	mvcvComputeCombinedCost32CycleCount = (mvcvComputeCombinedCost32TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvComputeCombinedCost32CycleCount)/ (float)(width * DISPARITIES));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	mvcvComputeCombinedCost32TestRunner.GetOutput(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
	mvcvComputeCombinedCost32TestRunner.GuardCheck(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
}
