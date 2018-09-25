#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (7.97)
#define EXPECTED_CC_M2 (7.295759)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convolution7x7Fp16ToU8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution7x7Fp16ToU8CycleCount;

void Convolution7x7Fp16ToU8_asm_test(half** in, unsigned char** out, half conv[49], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convolution7x7Fp16ToU8TestRunner.Init();
	//add padding values at the end of each line
	convolution7x7Fp16ToU8TestRunner.SetInput("input", in, width + padding, maxWidth, 7, SHAVE0);
	convolution7x7Fp16ToU8TestRunner.SetInput("conv", (half*)&conv[0], 49, SHAVE0);
	convolution7x7Fp16ToU8TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution7x7Fp16ToU8TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution7x7Fp16ToU8TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution7x7Fp16ToU8CycleCount = convolution7x7Fp16ToU8TestRunner.GetVariableValue("cycleCount");
		//value 157 substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(convolution7x7Fp16ToU8CycleCount - 44)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution7x7Fp16ToU8TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution7x7Fp16ToU8TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
