#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (20.06)
#define EXPECTED_CC_M2 (18.192448)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner Convolution3x3s3hhhhTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int Convolution3x3s3hhhhCycleCount;

void Convolution3x3s3hhhh_asm_test(half** in, half** out, half conv[9], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();

	const unsigned int padding = 16;


	unsigned int maxWidth = 1920 + padding;
	Convolution3x3s3hhhhTestRunner.Init();

	Convolution3x3s3hhhhTestRunner.SetInput("input", in, width + padding, maxWidth, 3, SHAVE0);
	Convolution3x3s3hhhhTestRunner.SetInput("conv", (half*)&conv[0], 9, SHAVE0);
	Convolution3x3s3hhhhTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	Convolution3x3s3hhhhTestRunner.GuardInsert(string("output"), SHAVE0, width/3, maxWidth, 1, out);
	Convolution3x3s3hhhhTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		Convolution3x3s3hhhhCycleCount = Convolution3x3s3hhhhTestRunner.GetVariableValue("cycleCount");

		functionInfo.AddCyclePerPixelInfo((float)(Convolution3x3s3hhhhCycleCount - 65)/ (float)width/3);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	Convolution3x3s3hhhhTestRunner.GetOutput(string("output"), SHAVE0, width/3, maxWidth, 1, out);
	Convolution3x3s3hhhhTestRunner.GuardCheck(string("output"), SHAVE0, width/3, maxWidth, 1, out);
	}

