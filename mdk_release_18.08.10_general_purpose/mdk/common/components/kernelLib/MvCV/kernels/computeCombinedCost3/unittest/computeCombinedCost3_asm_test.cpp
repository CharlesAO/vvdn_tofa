#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (0.39)
#define DISPARITIES 3

TestRunner mvcvComputeCombinedCost3TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvComputeCombinedCost3CycleCount;

void mvcvComputeCombinedCost3_asm_test(unsigned char *disparityCost, unsigned char *adCost, unsigned int alpha, unsigned int beta, unsigned int width, unsigned int normFactor)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvComputeCombinedCost3TestRunner.Init();
	mvcvComputeCombinedCost3TestRunner.SetInput("disparityCost", disparityCost, width * DISPARITIES, SHAVE0);
	mvcvComputeCombinedCost3TestRunner.SetInput("adCost", adCost, width * DISPARITIES, SHAVE0);
	mvcvComputeCombinedCost3TestRunner.SetInput("alpha", (unsigned int)alpha, SHAVE0);
	mvcvComputeCombinedCost3TestRunner.SetInput("beta", (unsigned int)beta, SHAVE0);
	mvcvComputeCombinedCost3TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	mvcvComputeCombinedCost3TestRunner.SetInput("normFactor", (unsigned int)normFactor, SHAVE0);
	
	mvcvComputeCombinedCost3TestRunner.GuardInsert(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
	mvcvComputeCombinedCost3TestRunner.Run(SHAVE0);
	if(width >= 960)
	{
		mvcvComputeCombinedCost3CycleCount = (mvcvComputeCombinedCost3TestRunner.GetVariableValue(std::string("cycleCount"))) - 2;
		functionInfo.AddCyclePerPixelInfo((float)(mvcvComputeCombinedCost3CycleCount)/ (float)(width * DISPARITIES));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvComputeCombinedCost3TestRunner.GetOutput(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
	mvcvComputeCombinedCost3TestRunner.GuardCheck(string("disparityCost"), SHAVE0, width * DISPARITIES, disparityCost);
}
