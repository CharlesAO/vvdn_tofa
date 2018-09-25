#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M2 (2.605097)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#endif

TestRunner convSeparable7x7Fp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convSeparable7x7Fp16ToFp16CycleCount;

void convSeparable7x7Fp16ToFp16_asm_test(half** out, half** in, half conv[4], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convSeparable7x7Fp16ToFp16TestRunner.Init();
	//add padding values at the end of each line
	convSeparable7x7Fp16ToFp16TestRunner.SetInput("input", in, width + padding, maxWidth, 7, SHAVE0);
	convSeparable7x7Fp16ToFp16TestRunner.SetInput("conv", (half*)&conv[0], 4, SHAVE0);
	convSeparable7x7Fp16ToFp16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convSeparable7x7Fp16ToFp16TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convSeparable7x7Fp16ToFp16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convSeparable7x7Fp16ToFp16CycleCount = convSeparable7x7Fp16ToFp16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convSeparable7x7Fp16ToFp16CycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
	
	convSeparable7x7Fp16ToFp16TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convSeparable7x7Fp16ToFp16TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
