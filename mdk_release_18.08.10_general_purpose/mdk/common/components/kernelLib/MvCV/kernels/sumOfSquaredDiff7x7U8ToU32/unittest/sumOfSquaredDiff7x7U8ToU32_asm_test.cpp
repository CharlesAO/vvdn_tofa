#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16
#define SIZE 7

#define EXPECTED_CC_M2 (6.412500)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#endif

TestRunner sumOfSquaredDiff7x7U8ToU32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int sumOfSquaredDiff7x7U8ToU32CycleCount;

void sumOfSquaredDiff7x7U8ToU32_asm_test(unsigned char **input1, unsigned char **input2, unsigned int **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	sumOfSquaredDiff7x7U8ToU32TestRunner.Init();
	sumOfSquaredDiff7x7U8ToU32TestRunner.SetInput("input1", input1, width + PADDING, maxWidth, SIZE, SHAVE0);
	sumOfSquaredDiff7x7U8ToU32TestRunner.SetInput("input2", input2, width + PADDING, maxWidth, SIZE, SHAVE0);
	sumOfSquaredDiff7x7U8ToU32TestRunner.SetInput("width", width, SHAVE0);

	sumOfSquaredDiff7x7U8ToU32TestRunner.GuardInsert("output", SHAVE0, width * 4, maxWidth * 4, 1, (unsigned char **)output);
	sumOfSquaredDiff7x7U8ToU32TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		sumOfSquaredDiff7x7U8ToU32CycleCount = sumOfSquaredDiff7x7U8ToU32TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(sumOfSquaredDiff7x7U8ToU32CycleCount - 63) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	sumOfSquaredDiff7x7U8ToU32TestRunner.GetOutput("output", SHAVE0, width * 4, maxWidth * 4, 1, (unsigned char **)output);
	sumOfSquaredDiff7x7U8ToU32TestRunner.GuardCheck("output", SHAVE0, width * 4, maxWidth * 4, 1, (unsigned char **)output);

}
