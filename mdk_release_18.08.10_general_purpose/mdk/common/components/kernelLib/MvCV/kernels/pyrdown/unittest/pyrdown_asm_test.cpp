#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (2.15)
#define EXPECTED_CC_M2 (2.460938)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerPyrDown(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int PyrDownCycleCount;

void pyrdown_asm_test(unsigned char** input, unsigned char** output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int padding = 16;
	unsigned int maxWidth = 1920 + padding;

	testRunnerPyrDown.Init();
	testRunnerPyrDown.SetInput("input", input, width + padding, maxWidth, 5, SHAVE0);
	testRunnerPyrDown.SetInput("width", width, SHAVE0);
	testRunnerPyrDown.GuardInsert("output", SHAVE0, (width - 1) / 2 + 1, maxWidth, 1, output);
	testRunnerPyrDown.Run(SHAVE0);
	if(width >= 320)
	{
		PyrDownCycleCount = testRunnerPyrDown.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(PyrDownCycleCount - 12)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerPyrDown.GetOutput("output", SHAVE0, (width - 1) / 2 + 1, maxWidth, 1, output);
	testRunnerPyrDown.GuardCheck("output", SHAVE0, (width - 1) / 2 + 1, maxWidth, 1, output);
}
