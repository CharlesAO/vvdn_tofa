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


TestRunner Convolution11x11s1xxhxTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int Convolution11x11s1xxhxCycleCount;

void Convolution11x11s1xxhx_asm_test(unsigned char** in, unsigned char** out, u8 conv[121], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	Convolution11x11s1xxhxTestRunner.Init();
	//add padding values at the end of each line
	Convolution11x11s1xxhxTestRunner.SetInput("input", in, width + padding, maxWidth, 11, SHAVE0);
	Convolution11x11s1xxhxTestRunner.SetInput("conv", (half*)&conv[0], 121, SHAVE0);
	Convolution11x11s1xxhxTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	Convolution11x11s1xxhxTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, out);
	Convolution11x11s1xxhxTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		Convolution11x11s1xxhxCycleCount = Convolution11x11s1xxhxTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(Convolution11x11s1xxhxCycleCount - 55)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	Convolution11x11s1xxhxTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, out);
	Convolution11x11s1xxhxTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, out);
}
