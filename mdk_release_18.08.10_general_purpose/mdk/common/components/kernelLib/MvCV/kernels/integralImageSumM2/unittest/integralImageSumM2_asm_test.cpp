#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (4.05)
#define EXPECTED_CC_M2 (1.296142)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerIntegral(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int integralImageSumCycleCount;

void integralImageSum_asm_test(unsigned int* out, unsigned char* in, unsigned int runNr, unsigned int** prevPointer, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int maxWidth = 1920;

	testRunnerIntegral.Init();
	testRunnerIntegral.SetInput("input", in, width, SHAVE0);
	testRunnerIntegral.SetInput("prev", (unsigned char**)prevPointer, width * 4, maxWidth, 1, SHAVE0);
	testRunnerIntegral.SetInput("runNr", runNr, SHAVE0);
	testRunnerIntegral.SetInput("width", width, SHAVE0);
	testRunnerIntegral.GuardInsert("output", SHAVE0, width * 4, (unsigned char*)out);
	testRunnerIntegral.Run(SHAVE0);
	if(width >= 1280)
	{
	integralImageSumCycleCount = testRunnerIntegral.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(integralImageSumCycleCount - 12)/ (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerIntegral.GetOutput("output", SHAVE0, width * 4, (unsigned char*)out);
	testRunnerIntegral.GuardCheck("output", SHAVE0, width * 4, (unsigned char*)out);

}

