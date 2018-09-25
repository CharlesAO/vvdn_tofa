#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (6.06)
#define EXPECTED_CC_M2 (8.192448)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convolution3x3s2xhhxTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution3x3s2xhhxCycleCount;

void Convolution3x3s2xhhx_asm_test(unsigned char** in, unsigned char** out, half conv[9], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	const unsigned int padding = 16;
	
	unsigned int maxWidth = 1920 + padding;

	convolution3x3s2xhhxTestRunner.Init();
	
	convolution3x3s2xhhxTestRunner.SetInput("input", in, width + padding, maxWidth, 3, SHAVE0);
	convolution3x3s2xhhxTestRunner.SetInput("conv", (half*)&conv[0], 9, SHAVE0);
	convolution3x3s2xhhxTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	convolution3x3s2xhhxTestRunner.GuardInsert(string("output"), SHAVE0, width/2, maxWidth, 1, out);
	convolution3x3s2xhhxTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convolution3x3s2xhhxCycleCount = convolution3x3s2xhhxTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convolution3x3s2xhhxCycleCount - 65)/ (float)width/2);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	convolution3x3s2xhhxTestRunner.GetOutput(string("output"), SHAVE0, width/2, maxWidth, 1, out);
	convolution3x3s2xhhxTestRunner.GuardCheck(string("output"), SHAVE0, width/2, maxWidth, 1, out);
}
