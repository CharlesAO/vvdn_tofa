#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (1.918037)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner pixelUnpackerTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int pixelUnpackerCycleCount;

void pixelUnpacker_asm_test(unsigned int *input32, unsigned char *input8, unsigned short *output, unsigned int width, unsigned char shift)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	pixelUnpackerTestRunner.Init();
	pixelUnpackerTestRunner.SetInput("input32", (float*)input32, width/4, SHAVE0);
	pixelUnpackerTestRunner.SetInput("input8", input8, width/4, SHAVE0);
	pixelUnpackerTestRunner.SetInput("width", width, SHAVE0);
	pixelUnpackerTestRunner.SetInput("shift", shift, SHAVE0);

	pixelUnpackerTestRunner.GuardInsert("output", SHAVE0, width, output);

	pixelUnpackerTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		pixelUnpackerCycleCount = pixelUnpackerTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(pixelUnpackerCycleCount - 3) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	pixelUnpackerTestRunner.GetOutput("output", SHAVE0, width, output);
	pixelUnpackerTestRunner.GuardCheck("output", SHAVE0, width, output);

}
