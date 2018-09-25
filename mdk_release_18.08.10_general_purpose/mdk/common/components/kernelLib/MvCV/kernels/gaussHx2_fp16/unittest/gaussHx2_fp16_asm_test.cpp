#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include "half.h"

#define EXPECTED_CC_M1 (0.83)
#define EXPECTED_CC_M2 (0.592317)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif


TestRunner testRunnerGaussHx2_fp16(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int GaussHx2_fp16CycleCount;

void GaussHx2_fp16_asm_test(half** input, half** output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int padding = 16;
	unsigned int maxWidth = 1920 + padding;
	testRunnerGaussHx2_fp16.Init();
	testRunnerGaussHx2_fp16.SetInput("input", input, width + padding, maxWidth, 1, SHAVE0);
	testRunnerGaussHx2_fp16.SetInput("width", width, SHAVE0);
	testRunnerGaussHx2_fp16.GuardInsert(string("output"), SHAVE0, width / 2, maxWidth, 1, output);
	testRunnerGaussHx2_fp16.Run(SHAVE0);
	if(width >= 1280)
	{
		GaussHx2_fp16CycleCount = testRunnerGaussHx2_fp16.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(GaussHx2_fp16CycleCount - 12)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerGaussHx2_fp16.GetOutput(string("output"), SHAVE0, width / 2, maxWidth, 1, output);
	testRunnerGaussHx2_fp16.GuardCheck(string("output"), SHAVE0, width / 2, maxWidth, 1, output);
}
