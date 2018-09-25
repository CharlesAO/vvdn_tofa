#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (4.06)
#define EXPECTED_CC_M2 (1.814512)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerIntegralF(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int integralImageSumFCycleCount;

void integralImageSumF_asm_test(float* out, unsigned char* in, unsigned int runNr, float** prevPointer, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int maxWidth = 1920;

	testRunnerIntegralF.Init();
	testRunnerIntegralF.SetInput("input", in, width, SHAVE0);
	testRunnerIntegralF.SetInput("prev", (unsigned char**)prevPointer, width * 4, maxWidth, 1, SHAVE0);
	testRunnerIntegralF.SetInput("runNr", runNr, SHAVE0);
	testRunnerIntegralF.SetInput("width", width, SHAVE0);
	testRunnerIntegralF.GuardInsert("output", SHAVE0, width * sizeof(float), (unsigned char*) out);
	testRunnerIntegralF.Run(SHAVE0);
	if(width >= 1280)
	{
		integralImageSumFCycleCount = testRunnerIntegralF.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(integralImageSumFCycleCount - 12)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerIntegralF.GetOutput("output", SHAVE0, width * sizeof(float), (unsigned char*) out);
	testRunnerIntegralF.GuardCheck("output", SHAVE0, width * sizeof(float), (unsigned char*) out);
	
}

