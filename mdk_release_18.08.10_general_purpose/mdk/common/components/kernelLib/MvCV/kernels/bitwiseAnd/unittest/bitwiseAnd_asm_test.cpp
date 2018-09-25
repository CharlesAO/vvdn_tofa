#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.21)
#define EXPECTED_CC_M2 (0.211979)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner bitwiseAndTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int bitwiseAndCycleCount;

void bitwiseAnd_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	bitwiseAndTestRunner.Init();
	bitwiseAndTestRunner.SetInput("input1", input1, width, maxWidth, 1, SHAVE0);
	bitwiseAndTestRunner.SetInput("input2", input2, width, maxWidth, 1, SHAVE0);
	bitwiseAndTestRunner.SetInput("width", width, SHAVE0);

	bitwiseAndTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	bitwiseAndTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		bitwiseAndCycleCount = bitwiseAndTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(bitwiseAndCycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	bitwiseAndTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	bitwiseAndTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
