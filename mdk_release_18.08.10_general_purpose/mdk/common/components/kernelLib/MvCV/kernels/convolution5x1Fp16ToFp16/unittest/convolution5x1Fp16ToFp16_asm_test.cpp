#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.16)
#define EXPECTED_CC_M2 (1.16)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convolution5x1Fp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution5x1Fp16ToFp16CycleCount;

void Convolution5x1Fp16ToFp16_asm_test(half** in, half** out, half conv[5], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	convolution5x1Fp16ToFp16TestRunner.Init();

	convolution5x1Fp16ToFp16TestRunner.SetInput("input", in, width, maxWidth, 5, SHAVE0);
	convolution5x1Fp16ToFp16TestRunner.SetInput("conv", (half*)&conv[0], 5, SHAVE0);
	convolution5x1Fp16ToFp16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution5x1Fp16ToFp16TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution5x1Fp16ToFp16TestRunner.Run(SHAVE0);
	
	convolution5x1Fp16ToFp16CycleCount = convolution5x1Fp16ToFp16TestRunner.GetVariableValue("cycleCount");
	if(width >= 1280)
	{
		functionInfo.AddCyclePerPixelInfo((float)(convolution5x1Fp16ToFp16CycleCount - 11)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}

	convolution5x1Fp16ToFp16TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convolution5x1Fp16ToFp16TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
