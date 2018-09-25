#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (16.416016)


TestRunner convolution11x11s3hhhhTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution11x11s3hhhhCycleCount;

void Convolution11x11s3hhhh_asm_test(half** in, half** out, half conv[121], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convolution11x11s3hhhhTestRunner.Init();
	//add padding values at the end of each line
	convolution11x11s3hhhhTestRunner.SetInput("input", in, width + padding, maxWidth, 11, SHAVE0);
	convolution11x11s3hhhhTestRunner.SetInput("conv", (half*)&conv[0], 121, SHAVE0);
	convolution11x11s3hhhhTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	convolution11x11s3hhhhTestRunner.GuardInsert("output", SHAVE0, width/3, maxWidth, 1, out);
	convolution11x11s3hhhhTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution11x11s3hhhhCycleCount = convolution11x11s3hhhhTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convolution11x11s3hhhhCycleCount - 55)/ (float)width/3);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution11x11s3hhhhTestRunner.GetOutput("output", SHAVE0, width/3, maxWidth, 1, out);
	convolution11x11s3hhhhTestRunner.GuardCheck("output", SHAVE0, width/3, maxWidth, 1, out);
}
