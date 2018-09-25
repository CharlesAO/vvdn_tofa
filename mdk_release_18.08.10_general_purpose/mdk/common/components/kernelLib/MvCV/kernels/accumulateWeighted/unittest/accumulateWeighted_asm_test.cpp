///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.39)
#define EXPECTED_CC_M2 (1.452083)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner AccumulateWeightedTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int AccumulateWeightedCycleCount;

void AccumulateWeighted_asm_test(unsigned char** srcAddr, unsigned char** maskAddr, float** destAddr, unsigned int width, float alpha)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	AccumulateWeightedTestRunner.Init();
	AccumulateWeightedTestRunner.SetInput("input", srcAddr, width, maxWidth, 1, SHAVE0);
	AccumulateWeightedTestRunner.SetInput("mask", maskAddr, width, maxWidth, 1, SHAVE0);
	AccumulateWeightedTestRunner.SetInput("output", (unsigned char**)destAddr, width * 4, maxWidth * 4, 1, SHAVE0);
	AccumulateWeightedTestRunner.SetInput("width", width, SHAVE0);
	AccumulateWeightedTestRunner.SetInput("alpha", alpha, SHAVE0);

	AccumulateWeightedTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, destAddr);
	AccumulateWeightedTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		AccumulateWeightedCycleCount = AccumulateWeightedTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(AccumulateWeightedCycleCount - 22) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	AccumulateWeightedTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, destAddr);
	AccumulateWeightedTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, destAddr);
}