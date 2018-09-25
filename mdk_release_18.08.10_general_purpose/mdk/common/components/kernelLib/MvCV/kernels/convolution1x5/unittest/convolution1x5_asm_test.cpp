#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.44)
#define EXPECTED_CC_M2 (1.190755)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convolution1x5TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution1x5CycleCount;

void Convolution1x5_asm_test(unsigned char** in, unsigned char** out, half* conv, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convolution1x5TestRunner.Init();
	//add padding values at the end of each line
	convolution1x5TestRunner.SetInput("input", in, width + padding, maxWidth, 1, SHAVE0);
	convolution1x5TestRunner.SetInput("conv", conv, 5, SHAVE0);
	convolution1x5TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution1x5TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution1x5TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution1x5CycleCount = convolution1x5TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convolution1x5CycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution1x5TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution1x5TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
