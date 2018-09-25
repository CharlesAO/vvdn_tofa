#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC_M2 (3.2)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#endif

TestRunner testRunnerScharr_fp16(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int scharr_fp16CycleCount;

void scharr_fp16_asm_test(half** input, half** output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int padding = 16;
	unsigned int maxWidth = 1920 + padding;

	testRunnerScharr_fp16.Init();
	testRunnerScharr_fp16.SetInput("input", input, width + padding, maxWidth, 3, SHAVE0);
	testRunnerScharr_fp16.SetInput("width", width, SHAVE0);

	testRunnerScharr_fp16.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	testRunnerScharr_fp16.Run(SHAVE0);
	if(width >= 1280)
	{
		scharr_fp16CycleCount = testRunnerScharr_fp16.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(scharr_fp16CycleCount - 38)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerScharr_fp16.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	testRunnerScharr_fp16.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);
}

