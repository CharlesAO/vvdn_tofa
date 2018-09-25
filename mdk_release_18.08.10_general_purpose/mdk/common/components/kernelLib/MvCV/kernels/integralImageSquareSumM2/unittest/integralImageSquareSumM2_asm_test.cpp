#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (4.54)
#define EXPECTED_CC_M2 (2.170203)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerIntegralSquare(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int integralImageSquareSumCycleCount;

void integralImageSquareSum_asm_test(unsigned int* out, unsigned char* in, unsigned int runNr,  unsigned int** prevPointer, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int maxWidth = 1920;
	testRunnerIntegralSquare.Init();
	testRunnerIntegralSquare.SetInput("input", in, width, SHAVE0);
	testRunnerIntegralSquare.SetInput("runNr", runNr, SHAVE0);
	testRunnerIntegralSquare.SetInput("prev", (unsigned char**)prevPointer, width * 4, maxWidth, 1, SHAVE0);
	testRunnerIntegralSquare.SetInput("width", width, SHAVE0);
	testRunnerIntegralSquare.GuardInsert("output", SHAVE0, width * 4, (unsigned char *)out);
	testRunnerIntegralSquare.Run(SHAVE0);
	if(width >= 1280)
	{
		integralImageSquareSumCycleCount = testRunnerIntegralSquare.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(integralImageSquareSumCycleCount - 12)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerIntegralSquare.GetOutput("output", SHAVE0, width * 4, (unsigned char *)out);
	testRunnerIntegralSquare.GuardCheck("output", SHAVE0, width * 4, (unsigned char *)out);
}

