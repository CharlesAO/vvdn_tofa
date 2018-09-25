#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.22)
#define EXPECTED_CC_M2 (0.218229)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner arithmeticAddTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int arithmeticAddCycleCount;

void arithmeticAdd_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + 8;
	arithmeticAddTestRunner.Init();
	arithmeticAddTestRunner.SetInput("input1", input1, width, maxWidth, 1, SHAVE0);
	arithmeticAddTestRunner.SetInput("input2", input2, width, maxWidth, 1, SHAVE0);
	arithmeticAddTestRunner.SetInput("width", width, SHAVE0);

	arithmeticAddTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	arithmeticAddTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		arithmeticAddCycleCount = arithmeticAddTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(arithmeticAddCycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	arithmeticAddTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	arithmeticAddTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
