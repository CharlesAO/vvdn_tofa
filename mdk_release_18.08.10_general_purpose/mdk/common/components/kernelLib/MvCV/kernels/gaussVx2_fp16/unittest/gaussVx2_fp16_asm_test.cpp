#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include "half.h"

#define EXPECTED_CC_M1 (1.03)
#define EXPECTED_CC_M2 (1.026070)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif


TestRunner testRunnerGaussVx2_fp16(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int GaussVx2_fp16CycleCount;

void GaussVx2_fp16_asm_test(half** input, half** output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;

	testRunnerGaussVx2_fp16.Init();
	testRunnerGaussVx2_fp16.SetInput("input", input, width, maxWidth, 5, SHAVE0);
	testRunnerGaussVx2_fp16.SetInput("width", width, SHAVE0);

	testRunnerGaussVx2_fp16.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, output);
	testRunnerGaussVx2_fp16.Run(SHAVE0);

		if(width >= 1280)
	{
	GaussVx2_fp16CycleCount = testRunnerGaussVx2_fp16.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(GaussVx2_fp16CycleCount - 14)/ (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerGaussVx2_fp16.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, output);
	testRunnerGaussVx2_fp16.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, output);
}
