#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.42)


TestRunner arithmeticSubU16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int arithmeticSubU16CycleCount;

void arithmeticSubU16_asm_test(unsigned short **input1, unsigned short **input2, short **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	arithmeticSubU16TestRunner.Init();
	arithmeticSubU16TestRunner.SetInput("input1", input1, width, maxWidth, 1, SHAVE0);
	arithmeticSubU16TestRunner.SetInput("input2", input2, width, maxWidth, 1, SHAVE0);
	arithmeticSubU16TestRunner.SetInput("width", width, SHAVE0);

	arithmeticSubU16TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, (unsigned short **)output);
	arithmeticSubU16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		arithmeticSubU16CycleCount = arithmeticSubU16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(arithmeticSubU16CycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	arithmeticSubU16TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, (unsigned short **)output);
	arithmeticSubU16TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, (unsigned short **)output);

}
