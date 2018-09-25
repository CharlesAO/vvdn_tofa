#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (2.81)
#define EXPECTED_CC_M2 (2.30599)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convolution1x9TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution1x9CycleCount;

void Convolution1x9_asm_test(unsigned char** in, unsigned char** out, half* conv, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convolution1x9TestRunner.Init();
	//add padding values at the end of each line
	convolution1x9TestRunner.SetInput("input", in, width + padding, maxWidth, 1, SHAVE0);
	convolution1x9TestRunner.SetInput("conv", conv, 9, SHAVE0);
	convolution1x9TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution1x9TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution1x9TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution1x9CycleCount = convolution1x9TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convolution1x9CycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution1x9TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution1x9TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
