#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.78)
#define EXPECTED_CC_M2 (1.777951)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convolution1x7Fp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution1x7Fp16ToFp16CycleCount;

void Convolution1x7Fp16ToFp16_asm_test(half* in, half* out, half conv[7], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 16;

	convolution1x7Fp16ToFp16TestRunner.Init();
	convolution1x7Fp16ToFp16TestRunner.SetInput("input", in, width + padding, SHAVE0);
	convolution1x7Fp16ToFp16TestRunner.SetInput("conv", (half*)&conv[0], 7, SHAVE0);
	convolution1x7Fp16ToFp16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution1x7Fp16ToFp16TestRunner.GuardInsert(string("output"), SHAVE0, width, out);
	convolution1x7Fp16ToFp16TestRunner.Run(SHAVE0);
	
	convolution1x7Fp16ToFp16CycleCount = convolution1x7Fp16ToFp16TestRunner.GetVariableValue("cycleCount");
	if(width >= 1280)
	{
		functionInfo.AddCyclePerPixelInfo((float)(convolution1x7Fp16ToFp16CycleCount - 3)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}

	convolution1x7Fp16ToFp16TestRunner.GetOutput(string("output"), SHAVE0, width, out);
	convolution1x7Fp16ToFp16TestRunner.GuardCheck(string("output"), SHAVE0, width, out);
}
