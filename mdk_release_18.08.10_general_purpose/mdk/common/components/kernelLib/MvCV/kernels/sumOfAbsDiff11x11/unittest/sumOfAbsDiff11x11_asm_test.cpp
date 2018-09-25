#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16
#define SIZE 11

#define EXPECTED_CC_M1 (5.68)
#define EXPECTED_CC_M2 (4.426562)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner sumOfAbsDiff11x11TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int sumOfAbsDiff11x11CycleCount;

void sumOfAbsDiff11x11_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	sumOfAbsDiff11x11TestRunner.Init();
	sumOfAbsDiff11x11TestRunner.SetInput("input1", input1, width + PADDING, maxWidth, SIZE, SHAVE0);
	sumOfAbsDiff11x11TestRunner.SetInput("input2", input2, width + PADDING, maxWidth, SIZE, SHAVE0);
	sumOfAbsDiff11x11TestRunner.SetInput("width", width, SHAVE0);

	sumOfAbsDiff11x11TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	sumOfAbsDiff11x11TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		sumOfAbsDiff11x11CycleCount = sumOfAbsDiff11x11TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(sumOfAbsDiff11x11CycleCount - 63) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	sumOfAbsDiff11x11TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	sumOfAbsDiff11x11TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
