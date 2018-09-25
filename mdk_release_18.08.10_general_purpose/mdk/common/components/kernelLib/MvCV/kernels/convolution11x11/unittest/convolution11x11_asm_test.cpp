#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (16.72)
#define EXPECTED_CC_M2 (16.338543)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif


TestRunner convolution11x11TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution11x11CycleCount;

void Convolution11x11_asm_test(unsigned char** in, unsigned char** out, half conv[121], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convolution11x11TestRunner.Init();
	//add padding values at the end of each line
	convolution11x11TestRunner.SetInput("input", in, width + padding, maxWidth, 11, SHAVE0);
	convolution11x11TestRunner.SetInput("conv", (half*)&conv[0], 121, SHAVE0);
	convolution11x11TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution11x11TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, out);
	convolution11x11TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution11x11CycleCount = convolution11x11TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convolution11x11CycleCount - 55)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution11x11TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, out);
	convolution11x11TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, out);
}
