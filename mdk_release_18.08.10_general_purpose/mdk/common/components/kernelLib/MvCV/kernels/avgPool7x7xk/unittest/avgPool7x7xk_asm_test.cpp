#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (8.9)

TestRunner avgPool7x7xkTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int avgPool7x7xkCycleCount;

void avgPool7x7xk_asm_test(half** input1, half** output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 240 + 32;
	avgPool7x7xkTestRunner.Init();
	avgPool7x7xkTestRunner.SetInput("input1", input1, width, maxWidth, 49, SHAVE0);
	avgPool7x7xkTestRunner.SetInput("width", width, SHAVE0);
	avgPool7x7xkTestRunner.SetInput("output", output, width + 32, maxWidth, 1, SHAVE0);	
	avgPool7x7xkTestRunner.GuardInsert("output", SHAVE0, width + 32, maxWidth, 1, output);	
	avgPool7x7xkTestRunner.Run(SHAVE0);
	
	if(width >= 240)
	{
		avgPool7x7xkCycleCount = avgPool7x7xkTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(avgPool7x7xkCycleCount - 38) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	avgPool7x7xkTestRunner.GetOutput("output", SHAVE0, width + 32, maxWidth, 1, output);	
	avgPool7x7xkTestRunner.GuardCheck("output", SHAVE0, width + 32, maxWidth, 1, output);	
}
