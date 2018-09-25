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

TestRunner bitwiseXorTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int bitwiseXorCycleCount;

void bitwiseXor_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	bitwiseXorTestRunner.Init();
	bitwiseXorTestRunner.SetInput("input1", input1, width, maxWidth, 1, SHAVE0);
	bitwiseXorTestRunner.SetInput("input2", input2, width, maxWidth, 1, SHAVE0);
	bitwiseXorTestRunner.SetInput("width", width, SHAVE0);

	bitwiseXorTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	bitwiseXorTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		bitwiseXorCycleCount = bitwiseXorTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(bitwiseXorCycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	bitwiseXorTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	bitwiseXorTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
