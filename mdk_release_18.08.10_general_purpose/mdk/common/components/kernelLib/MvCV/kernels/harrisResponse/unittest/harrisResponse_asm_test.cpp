#include "harrisResponse.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define WIDTH 8
#define HEIGHT 8

#define EXPECTED_CC_M1 (10.88)
#define EXPECTED_CC_M2 (13.75)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner HarrisResponseTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int HarrisResponseCycleCount;

float HarrisResponse_asm_test(unsigned char* data, unsigned int x, unsigned int y, unsigned int step_width, float k)
{
	float out = 0;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	HarrisResponseTestRunner.Init();
	HarrisResponseTestRunner.SetInput("input", data, WIDTH * HEIGHT, SHAVE0);
	HarrisResponseTestRunner.SetInput("x", x, SHAVE0);
	HarrisResponseTestRunner.SetInput("y", y, SHAVE0);
	HarrisResponseTestRunner.SetInput("step_width", step_width, SHAVE0);
	HarrisResponseTestRunner.SetInput("k", k, SHAVE0);

	HarrisResponseTestRunner.Run(SHAVE0);
	if(step_width >= 8)
	{
		HarrisResponseCycleCount = HarrisResponseTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(HarrisResponseCycleCount - 3) / (float)WIDTH);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	HarrisResponseTestRunner.GetOutput("output", SHAVE0, 1, &out);
	
	return out;
}
