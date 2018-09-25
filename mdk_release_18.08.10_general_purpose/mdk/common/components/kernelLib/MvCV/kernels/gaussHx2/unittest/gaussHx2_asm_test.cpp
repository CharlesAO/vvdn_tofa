#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.891276)
#define EXPECTED_CC_M2 (1.03)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerGaussHx2(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int GaussHx2CycleCount;

void GaussHx2_asm_test(unsigned char* input, unsigned char* output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int padding = 4;
	testRunnerGaussHx2.Init();
	testRunnerGaussHx2.SetInput("input", input, width * 2 + padding, SHAVE0);
	testRunnerGaussHx2.SetInput("width", width, SHAVE0);
	testRunnerGaussHx2.GuardInsert("output", SHAVE0, width, output);
	testRunnerGaussHx2.Run(SHAVE0);
	if(width >= 1280)
	{
		GaussHx2CycleCount = testRunnerGaussHx2.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(GaussHx2CycleCount - 2)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerGaussHx2.GetOutput("output", SHAVE0, width, output);
	testRunnerGaussHx2.GuardCheck("output", SHAVE0, width, output);
}
