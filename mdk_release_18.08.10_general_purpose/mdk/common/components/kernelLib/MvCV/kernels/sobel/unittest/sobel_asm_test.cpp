#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (4.13)
#define EXPECTED_CC_M2 (3.639583)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerSobel(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int SobelCycleCount;

void sobel_asm_test(unsigned char** input, unsigned char** output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int padding = 16;
	unsigned int maxWidth = 1920 + padding;

	testRunnerSobel.Init();
	testRunnerSobel.SetInput("input", input, width + padding, maxWidth, 3, SHAVE0);
	testRunnerSobel.SetInput("width", width, SHAVE0);

	testRunnerSobel.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	testRunnerSobel.Run(SHAVE0);
	if(width >= 1280)
	{
		SobelCycleCount = testRunnerSobel.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(SobelCycleCount - 12)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerSobel.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	testRunnerSobel.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);
}

