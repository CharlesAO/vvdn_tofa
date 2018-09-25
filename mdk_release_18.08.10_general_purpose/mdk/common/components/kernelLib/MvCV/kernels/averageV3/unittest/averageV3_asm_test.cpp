#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.6)

TestRunner AverageV3TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int AverageV3CycleCount;

void AverageV3_asm_test(half** in, half** out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;
	AverageV3TestRunner.Init();
	AverageV3TestRunner.SetInput("input", in, width + padding, maxWidth, 3, SHAVE0);
	AverageV3TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	AverageV3TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	AverageV3TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		AverageV3CycleCount = AverageV3TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(AverageV3CycleCount - 44)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	AverageV3TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	AverageV3TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
