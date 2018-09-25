#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.891276)
#define EXPECTED_CC_M2 (0.9)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerGaussVx2(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int GaussVx2CycleCount;

void GaussVx2_asm_test(unsigned char** in, unsigned char* out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;

	testRunnerGaussVx2.Init();
	testRunnerGaussVx2.SetInput("input", in, width, maxWidth, 5, SHAVE0);
	testRunnerGaussVx2.SetInput("width", width, SHAVE0);

	testRunnerGaussVx2.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, (unsigned char**)&out);
	testRunnerGaussVx2.Run(SHAVE0);

	if(width >= 1280)
	{
	GaussVx2CycleCount = testRunnerGaussVx2.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(GaussVx2CycleCount - 9)/ (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerGaussVx2.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, (unsigned char**)&out);
	testRunnerGaussVx2.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, (unsigned char**)&out);
}
