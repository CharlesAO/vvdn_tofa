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

TestRunner arithmeticAddMaskTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int arithmeticAddmaskCycleCount;

void arithmeticAddmask_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned char **mask, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	arithmeticAddMaskTestRunner.Init();
	arithmeticAddMaskTestRunner.SetInput("input1", input1, width, maxWidth, 1, SHAVE0);
	arithmeticAddMaskTestRunner.SetInput("input2", input2, width, maxWidth, 1, SHAVE0);
	arithmeticAddMaskTestRunner.SetInput("width", width, SHAVE0);
	arithmeticAddMaskTestRunner.SetInput("mask", mask, width, maxWidth, 1, SHAVE0);
	arithmeticAddMaskTestRunner.SetInput("output", output, width, maxWidth, 1, SHAVE0);
	arithmeticAddMaskTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	arithmeticAddMaskTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{	
		arithmeticAddmaskCycleCount = arithmeticAddMaskTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(arithmeticAddmaskCycleCount - 35) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	arithmeticAddMaskTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	arithmeticAddMaskTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
