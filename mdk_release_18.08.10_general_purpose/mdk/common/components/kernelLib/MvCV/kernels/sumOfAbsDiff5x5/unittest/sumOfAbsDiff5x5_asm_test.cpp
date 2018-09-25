#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16
#define SIZE 5

#define EXPECTED_CC_M1 (2.64)
#define EXPECTED_CC_M2 (2.017708)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner sumOfAbsDiff5x5TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int sumOfAbsDiff5x5CycleCount;

void sumOfAbsDiff5x5_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	sumOfAbsDiff5x5TestRunner.Init();
	sumOfAbsDiff5x5TestRunner.SetInput("input1", input1, width + PADDING, maxWidth, SIZE, SHAVE0);
	sumOfAbsDiff5x5TestRunner.SetInput("input2", input2, width + PADDING, maxWidth, SIZE, SHAVE0);
	sumOfAbsDiff5x5TestRunner.SetInput("width", width, SHAVE0);

	sumOfAbsDiff5x5TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	sumOfAbsDiff5x5TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		sumOfAbsDiff5x5CycleCount = sumOfAbsDiff5x5TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(sumOfAbsDiff5x5CycleCount - 63) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	sumOfAbsDiff5x5TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	sumOfAbsDiff5x5TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
