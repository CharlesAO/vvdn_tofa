#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (5.790104)
#define EXPECTED_CC_M2 (3.059375)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerIntegralSquareF(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int integralImageSquareSumFCycleCount;

void integralImageSquareSumFloat_asm_test(float* out, unsigned char* in, unsigned int runNr, float** prevPointer, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int maxWidth = 1920;

	testRunnerIntegralSquareF.Init();
	testRunnerIntegralSquareF.SetInput("input", in, width, SHAVE0);
	testRunnerIntegralSquareF.SetInput("prev", (unsigned char**)prevPointer, width * 4, maxWidth, 1, SHAVE0);
	testRunnerIntegralSquareF.SetInput("width", width, SHAVE0);
	testRunnerIntegralSquareF.SetInput("runNr", runNr, SHAVE0);
	testRunnerIntegralSquareF.GuardInsert("output", SHAVE0, width * sizeof(float), (unsigned char *)out);
	testRunnerIntegralSquareF.Run(SHAVE0);
	
	if(width >= 1280)
	{
	integralImageSquareSumFCycleCount = testRunnerIntegralSquareF.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(integralImageSquareSumFCycleCount - 12)/ (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerIntegralSquareF.GetOutput("output", SHAVE0, width * sizeof(float), (unsigned char *)out);
	testRunnerIntegralSquareF.GuardCheck("output", SHAVE0, width * sizeof(float), (unsigned char *)out);
}

