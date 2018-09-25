#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (3.06)
#define EXPECTED_CC_M2 (2.192448)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convolution3x3TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution3x3CycleCount;

void Convolution3x3_asm_test(unsigned char** in, unsigned char** out, half conv[9], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convolution3x3TestRunner.Init();
	//add padding values at the end of each line
	convolution3x3TestRunner.SetInput("input", in, width + padding, maxWidth, 3, SHAVE0);
	convolution3x3TestRunner.SetInput("conv", (half*)&conv[0], 9, SHAVE0);
	convolution3x3TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution3x3TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution3x3TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution3x3CycleCount = convolution3x3TestRunner.GetVariableValue("cycleCount");
		//the value substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(convolution3x3CycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution3x3TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution3x3TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
