#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (22.5)

TestRunner convolution9x9s2hhhhTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution9x9s2hhhhCycleCount;

void Convolution9x9s2hhhh_asm_test(half** in, half** out, half conv[81], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convolution9x9s2hhhhTestRunner.Init();
	//add padding values at the end of each line
	convolution9x9s2hhhhTestRunner.SetInput("input", in, width + padding, maxWidth, 9, SHAVE0);
	convolution9x9s2hhhhTestRunner.SetInput("output", out, width, maxWidth, 1, SHAVE0);
	convolution9x9s2hhhhTestRunner.SetInput("conv", (half*)&conv[0], 81, SHAVE0);
	convolution9x9s2hhhhTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution9x9s2hhhhTestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution9x9s2hhhhTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution9x9s2hhhhCycleCount = convolution9x9s2hhhhTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convolution9x9s2hhhhCycleCount - 65)/ (float)(width/2));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution9x9s2hhhhTestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution9x9s2hhhhTestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
