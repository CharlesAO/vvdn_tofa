#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (135.33)

TestRunner convolution11x11s8hhhhTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution11x11s8hhhhCycleCount;

void Convolution11x11s8hhhh_asm_test(half** in, half** out, half conv[121], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convolution11x11s8hhhhTestRunner.Init();
	//add padding values at the end of each line
	convolution11x11s8hhhhTestRunner.SetInput("input", in, width + padding, maxWidth, 11, SHAVE0);
	convolution11x11s8hhhhTestRunner.SetInput("conv", (half*)&conv[0], 121, SHAVE0);
	convolution11x11s8hhhhTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	convolution11x11s8hhhhTestRunner.GuardInsert("output", SHAVE0, width/8, maxWidth, 1, out);
	convolution11x11s8hhhhTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution11x11s8hhhhCycleCount = convolution11x11s8hhhhTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convolution11x11s8hhhhCycleCount - 55)/ (float)(width/8));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution11x11s8hhhhTestRunner.GetOutput("output", SHAVE0, width/8, maxWidth, 1, out);
	convolution11x11s8hhhhTestRunner.GuardCheck("output", SHAVE0, width/8, maxWidth, 1, out);
}
