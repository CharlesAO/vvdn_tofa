#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.83)
#define PADDING 16 

TestRunner mvcvComputeADPyrOnePosTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvComputeADPyrOnePosCycleCount;

void mvcvComputeADPyrOnePos_asm_test(unsigned char *input1, unsigned char *input2, unsigned char *output, unsigned int width)
{
	unsigned int disparities = 3;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvComputeADPyrOnePosTestRunner.Init();
	mvcvComputeADPyrOnePosTestRunner.SetInput(   "input1", input1, width * disparities + PADDING, SHAVE0);
	mvcvComputeADPyrOnePosTestRunner.SetInput(   "input2", input2, width * disparities + PADDING, SHAVE0);
	mvcvComputeADPyrOnePosTestRunner.SetInput(   "output", output, width * disparities + PADDING, SHAVE0);
    mvcvComputeADPyrOnePosTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	mvcvComputeADPyrOnePosTestRunner.GuardInsert(string("output"), SHAVE0, width * disparities + PADDING, output);
	mvcvComputeADPyrOnePosTestRunner.Run(SHAVE0);

	mvcvComputeADPyrOnePosCycleCount = (mvcvComputeADPyrOnePosTestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvComputeADPyrOnePosCycleCount)/ (float)(width * disparities));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	mvcvComputeADPyrOnePosTestRunner.GetOutput(string("output"), SHAVE0, width * disparities + PADDING, output);
	mvcvComputeADPyrOnePosTestRunner.GuardCheck(string("output"), SHAVE0, width * disparities + PADDING, output);
}
