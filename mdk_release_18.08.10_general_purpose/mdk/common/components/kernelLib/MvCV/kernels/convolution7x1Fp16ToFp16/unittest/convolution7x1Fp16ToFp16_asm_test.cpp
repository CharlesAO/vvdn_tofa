#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.90)
#define EXPECTED_CC_M2 (1.907552)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convolution7x1Fp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution7x1Fp16ToFp16CycleCount;

void convolution7x1Fp16ToFp16_asm_test(half** in, half* out, half* conv, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();	
	unsigned int maxWidth = 1920;
	convolution7x1Fp16ToFp16TestRunner.Init();
	convolution7x1Fp16ToFp16TestRunner.SetInput("input", in, width, maxWidth, 7, SHAVE0);
	convolution7x1Fp16ToFp16TestRunner.SetInput("conv", conv, 7, SHAVE0);
	convolution7x1Fp16ToFp16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convolution7x1Fp16ToFp16TestRunner.GuardInsert(string("output"), SHAVE0, width, out);
	convolution7x1Fp16ToFp16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution7x1Fp16ToFp16CycleCount = convolution7x1Fp16ToFp16TestRunner.GetVariableValue("cycleCount");
	
		functionInfo.AddCyclePerPixelInfo((float)(convolution7x1Fp16ToFp16CycleCount - 12)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution7x1Fp16ToFp16TestRunner.GetOutput(string("output"), SHAVE0, width, out);
	convolution7x1Fp16ToFp16TestRunner.GuardCheck(string("output"), SHAVE0, width, out);
}
