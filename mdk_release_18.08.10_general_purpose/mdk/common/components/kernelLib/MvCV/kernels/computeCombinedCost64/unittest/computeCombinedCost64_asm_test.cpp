#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.50)
#define DISPARITIES 64

TestRunner mvcvComputeCombinedCost64TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvComputeCombinedCost64CycleCount;

void mvcvComputeCombinedCost64_asm_test(unsigned char* disparityCost, unsigned char* adCost, unsigned int alpha, unsigned int beta, unsigned int width, unsigned int normFactor)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvComputeCombinedCost64TestRunner.Init();
	mvcvComputeCombinedCost64TestRunner.SetInput(   "disparityCost", disparityCost, width * DISPARITIES, SHAVE0);
	mvcvComputeCombinedCost64TestRunner.SetInput(   "adCost", adCost, width * DISPARITIES, SHAVE0);
    mvcvComputeCombinedCost64TestRunner.SetInput(   "alpha", (unsigned int)alpha, SHAVE0);
    mvcvComputeCombinedCost64TestRunner.SetInput(   "beta", (unsigned int)beta, SHAVE0);
    mvcvComputeCombinedCost64TestRunner.SetInput(   "width", (unsigned int)width, SHAVE0);
    mvcvComputeCombinedCost64TestRunner.SetInput(   "normFactor", (unsigned int)normFactor, SHAVE0);

	mvcvComputeCombinedCost64TestRunner.GuardInsert(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
	mvcvComputeCombinedCost64TestRunner.Run(SHAVE0);

	mvcvComputeCombinedCost64CycleCount = (mvcvComputeCombinedCost64TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvComputeCombinedCost64CycleCount)/ (float)(width * DISPARITIES));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	mvcvComputeCombinedCost64TestRunner.GetOutput(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
	mvcvComputeCombinedCost64TestRunner.GuardCheck(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
}
