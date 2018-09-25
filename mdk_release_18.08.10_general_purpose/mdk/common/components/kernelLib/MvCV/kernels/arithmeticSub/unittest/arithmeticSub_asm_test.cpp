#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.22)
#define EXPECTED_CC_M2 (0.214583)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner arithmeticSubTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int arithmeticSubCycleCount;

void arithmeticSub_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + 8;
	arithmeticSubTestRunner.Init();
	arithmeticSubTestRunner.SetInput("input1", input1, width, maxWidth, 1, SHAVE0);
	arithmeticSubTestRunner.SetInput("input2", input2, width, maxWidth, 1, SHAVE0);
	arithmeticSubTestRunner.SetInput("width", width, SHAVE0);

	arithmeticSubTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	arithmeticSubTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		arithmeticSubCycleCount = arithmeticSubTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(arithmeticSubCycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	arithmeticSubTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	arithmeticSubTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
