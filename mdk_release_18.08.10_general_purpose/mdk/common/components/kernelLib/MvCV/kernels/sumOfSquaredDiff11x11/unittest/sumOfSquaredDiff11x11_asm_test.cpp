#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16
#define SIZE 11

#define EXPECTED_CC_M1 (7.19)
#define EXPECTED_CC_M2 (6.069271)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner sumOfSquaredDiff11x11TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int sumOfSquaredDiff11x11CycleCount;

void sumOfSquaredDiff11x11_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	sumOfSquaredDiff11x11TestRunner.Init();
	sumOfSquaredDiff11x11TestRunner.SetInput("input1", input1, width + PADDING, maxWidth, SIZE, SHAVE0);
	sumOfSquaredDiff11x11TestRunner.SetInput("input2", input2, width + PADDING, maxWidth, SIZE, SHAVE0);
	sumOfSquaredDiff11x11TestRunner.SetInput("width", width, SHAVE0);

	sumOfSquaredDiff11x11TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	sumOfSquaredDiff11x11TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		sumOfSquaredDiff11x11CycleCount = sumOfSquaredDiff11x11TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(sumOfSquaredDiff11x11CycleCount - 63) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	sumOfSquaredDiff11x11TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	sumOfSquaredDiff11x11TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
