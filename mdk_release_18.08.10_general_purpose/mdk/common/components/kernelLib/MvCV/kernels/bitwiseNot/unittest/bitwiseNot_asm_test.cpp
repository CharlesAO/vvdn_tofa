#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.14)
#define EXPECTED_CC_M2 (0.146354)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner bitwiseNotTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int bitwiseNotCycleCount;

void bitwiseNot_asm_test(unsigned char **input, unsigned char **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	bitwiseNotTestRunner.Init();
	bitwiseNotTestRunner.SetInput("input", input, width, maxWidth, 1, SHAVE0);
	bitwiseNotTestRunner.SetInput("width", width, SHAVE0);

	bitwiseNotTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	bitwiseNotTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		bitwiseNotCycleCount = bitwiseNotTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(bitwiseNotCycleCount - 11) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	bitwiseNotTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	bitwiseNotTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
