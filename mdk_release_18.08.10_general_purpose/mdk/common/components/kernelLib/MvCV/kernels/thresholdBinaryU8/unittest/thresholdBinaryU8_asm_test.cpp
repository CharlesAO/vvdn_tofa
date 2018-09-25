#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.22)
#define EXPECTED_CC_M2 (0.213542)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner thresholdBinaryTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int thresholdBinaryU8CycleCount;

void thresholdBinaryU8_asm_test(unsigned char** in, unsigned char** out, unsigned char threshold, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	unsigned int maxWidth = 1920;

	thresholdBinaryTestRunner.Init();
	thresholdBinaryTestRunner.SetInput("input", in, width, maxWidth, 1, SHAVE0);
    thresholdBinaryTestRunner.SetInput("thresh", (unsigned int)threshold, SHAVE0);
	thresholdBinaryTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	thresholdBinaryTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, out);
	thresholdBinaryTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		thresholdBinaryU8CycleCount = thresholdBinaryTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(thresholdBinaryU8CycleCount - 10)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	thresholdBinaryTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, out);
	thresholdBinaryTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, out);
}
