#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (2.06)
#define EXPECTED_CC_M2 (2.069662)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convolution7x1TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution7x1CycleCount;

void Convolution7x1_asm_test(unsigned char** in, unsigned char** out, half* conv, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920;

	convolution7x1TestRunner.Init();

	convolution7x1TestRunner.SetInput("input", in, width, maxWidth, 7, SHAVE0);
	convolution7x1TestRunner.SetInput("conv", conv, 7, SHAVE0);
	convolution7x1TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution7x1TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution7x1TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution7x1CycleCount = convolution7x1TestRunner.GetVariableValue("cycleCount");
		//value 157 substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(convolution7x1CycleCount - 44)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution7x1TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution7x1TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
