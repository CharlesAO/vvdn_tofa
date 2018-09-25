#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (0.39)
#define DISPARITIES 5

TestRunner mvcvComputeCombinedCost3WindowTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvComputeCombinedCost3WindowCycleCount;

void mvcvComputeCombinedCost3Window_asm_test(unsigned char *disparityCost, unsigned char *adCost, unsigned int alpha, unsigned int beta, unsigned int width, unsigned int normFactor)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvComputeCombinedCost3WindowTestRunner.Init();
	mvcvComputeCombinedCost3WindowTestRunner.SetInput("disparityCost", disparityCost, width * DISPARITIES, SHAVE0);
	mvcvComputeCombinedCost3WindowTestRunner.SetInput("adCost", adCost, width * DISPARITIES, SHAVE0);
	mvcvComputeCombinedCost3WindowTestRunner.SetInput("alpha", (unsigned int)alpha, SHAVE0);
	mvcvComputeCombinedCost3WindowTestRunner.SetInput("beta", (unsigned int)beta, SHAVE0);
	mvcvComputeCombinedCost3WindowTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	mvcvComputeCombinedCost3WindowTestRunner.SetInput("normFactor", (unsigned int)normFactor, SHAVE0);
	
	mvcvComputeCombinedCost3WindowTestRunner.GuardInsert(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
	mvcvComputeCombinedCost3WindowTestRunner.Run(SHAVE0);
	if(width >= 960)
	{
		mvcvComputeCombinedCost3WindowCycleCount = (mvcvComputeCombinedCost3WindowTestRunner.GetVariableValue(std::string("cycleCount"))) - 2;
		functionInfo.AddCyclePerPixelInfo((float)(mvcvComputeCombinedCost3WindowCycleCount)/ (float)(width * DISPARITIES));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvComputeCombinedCost3WindowTestRunner.GetOutput(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
	mvcvComputeCombinedCost3WindowTestRunner.GuardCheck(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
}
