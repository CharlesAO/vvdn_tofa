#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (135.97)
#define EXPECTED_CC_M2 (135.965881)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerCanny(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int CannyCycleCount;

void canny_asm_test(unsigned char** src, unsigned char** dst, unsigned char* buffer,
		unsigned int threshold1, unsigned int threshold2, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int padding = 16;
	unsigned int maxWidth = 1920 + padding;
    buffer[0] = 1;
	testRunnerCanny.Init();
	testRunnerCanny.SetInput("input", src, width + padding, maxWidth, 9, SHAVE0);


	testRunnerCanny.SetInput("threshold1", threshold1, SHAVE0);
	testRunnerCanny.SetInput("threshold2", threshold2, SHAVE0);
	testRunnerCanny.SetInput("width", width, SHAVE0);

	testRunnerCanny.GuardInsert("output", SHAVE0, width, maxWidth, 1, dst);
	testRunnerCanny.Run(SHAVE0);
	if(width >= 320)
	{
		CannyCycleCount = testRunnerCanny.GetVariableValue("cycleCount");

		functionInfo.AddCyclePerPixelInfo((float)(CannyCycleCount - 12)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerCanny.GetOutput("output", SHAVE0, width, maxWidth, 1, dst);
	testRunnerCanny.GuardCheck("output", SHAVE0, width, maxWidth, 1, dst);
}
