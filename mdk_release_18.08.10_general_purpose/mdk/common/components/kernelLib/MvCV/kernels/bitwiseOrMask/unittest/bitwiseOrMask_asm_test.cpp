#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.27)
#define EXPECTED_CC_M2 (0.276563)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner bitwiseOrMaskTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int bitwiseOrMaskCycleCount;

void bitwiseOrMask_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned char **mask, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	bitwiseOrMaskTestRunner.Init();
	bitwiseOrMaskTestRunner.SetInput("input1", input1, width, maxWidth, 1, SHAVE0);
	bitwiseOrMaskTestRunner.SetInput("input2", input2, width, maxWidth, 1, SHAVE0);
	bitwiseOrMaskTestRunner.SetInput("mask", 	mask, 	width, maxWidth, 1, SHAVE0);
	bitwiseOrMaskTestRunner.SetInput("output", output, width, maxWidth, 1, SHAVE0);
	bitwiseOrMaskTestRunner.SetInput("width", width, SHAVE0);

	bitwiseOrMaskTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	bitwiseOrMaskTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		bitwiseOrMaskCycleCount = bitwiseOrMaskTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(bitwiseOrMaskCycleCount - 18) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	bitwiseOrMaskTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	bitwiseOrMaskTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
