#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (11.72)
#define EXPECTED_CC_M2 (11.331381)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convolution9x9Fp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution9x9Fp16ToFp16CycleCount;

void Convolution9x9Fp16ToFp16_asm_test(half** in, half** out, half conv[81], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convolution9x9Fp16ToFp16TestRunner.Init();
	//add padding values at the end of each line
	convolution9x9Fp16ToFp16TestRunner.SetInput("input", in, width + padding, maxWidth, 9, SHAVE0);
	convolution9x9Fp16ToFp16TestRunner.SetInput("conv", (half*)&conv[0], 81, SHAVE0);
	convolution9x9Fp16ToFp16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution9x9Fp16ToFp16TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution9x9Fp16ToFp16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution9x9Fp16ToFp16CycleCount = convolution9x9Fp16ToFp16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convolution9x9Fp16ToFp16CycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution9x9Fp16ToFp16TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution9x9Fp16ToFp16TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}