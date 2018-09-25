#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.23)
#define EXPECTED_CC_M2 (0.2125)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner thresholdBinaryRangeTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int thresholdBinaryRangeCycleCount;

//void thresholdBinaryRange(u8** in, u8** out,  u8 lowerValue, u8 upperValue, u32 width)
void thresholdBinaryRange_asm_test(unsigned char **input, unsigned char **output, unsigned char lValue, unsigned char uValue, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	thresholdBinaryRangeTestRunner.Init();
	thresholdBinaryRangeTestRunner.SetInput("input", input, width, maxWidth, 1, SHAVE0);
	thresholdBinaryRangeTestRunner.SetInput("lowerValue", lValue, SHAVE0);
	thresholdBinaryRangeTestRunner.SetInput("upperValue", uValue, SHAVE0);
	thresholdBinaryRangeTestRunner.SetInput("width", width, SHAVE0);
	
	thresholdBinaryRangeTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	thresholdBinaryRangeTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		thresholdBinaryRangeCycleCount = thresholdBinaryRangeTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(thresholdBinaryRangeCycleCount - 11) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	thresholdBinaryRangeTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	thresholdBinaryRangeTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);
	
}
