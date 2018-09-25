#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16
#define SIZE 5

#define EXPECTED_CC_M1 (3.40)
#define EXPECTED_CC_M2 (3.028646)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner sumOfSquaredDiff5x5TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int sumOfSquaredDiff5x5CycleCount;

void sumOfSquaredDiff5x5_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	sumOfSquaredDiff5x5TestRunner.Init();
	sumOfSquaredDiff5x5TestRunner.SetInput("input1", input1, width + PADDING, maxWidth, SIZE, SHAVE0);
	sumOfSquaredDiff5x5TestRunner.SetInput("input2", input2, width + PADDING, maxWidth, SIZE, SHAVE0);
	sumOfSquaredDiff5x5TestRunner.SetInput("width", width, SHAVE0);

	sumOfSquaredDiff5x5TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	sumOfSquaredDiff5x5TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		sumOfSquaredDiff5x5CycleCount = sumOfSquaredDiff5x5TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(sumOfSquaredDiff5x5CycleCount - 63) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	sumOfSquaredDiff5x5TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	sumOfSquaredDiff5x5TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
