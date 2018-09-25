#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.42)
#define EXPECTED_CC_M2 (0.492708)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner arithmeticSubMaskTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int arithmeticSubmaskCycleCount;

void arithmeticSubmask_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned char **mask, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + 8;
	arithmeticSubMaskTestRunner.Init();
	arithmeticSubMaskTestRunner.SetInput("input1", input1, width, maxWidth, 1, SHAVE0);
	arithmeticSubMaskTestRunner.SetInput("input2", input2, width, maxWidth, 1, SHAVE0);
	arithmeticSubMaskTestRunner.SetInput("width", width, SHAVE0);
	arithmeticSubMaskTestRunner.SetInput("mask", mask, width, maxWidth, 1, SHAVE0);
	
	arithmeticSubMaskTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	arithmeticSubMaskTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		arithmeticSubmaskCycleCount = arithmeticSubMaskTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(arithmeticSubmaskCycleCount - 45) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	arithmeticSubMaskTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	arithmeticSubMaskTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
