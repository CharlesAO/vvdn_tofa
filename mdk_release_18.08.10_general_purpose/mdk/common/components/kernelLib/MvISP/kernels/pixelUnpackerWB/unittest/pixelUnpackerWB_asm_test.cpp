#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (1.667324)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner pixelUnpackerWBTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int pixelUnpackerWBCycleCount;

void pixelUnpackerWB_asm_test(unsigned int *input32, unsigned char *input8, unsigned short *output, unsigned int width, unsigned char shift, unsigned short *awbCoef, unsigned int line_no)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	pixelUnpackerWBTestRunner.Init();
	pixelUnpackerWBTestRunner.SetInput("input32", (float*)input32, width/4, SHAVE0);
	pixelUnpackerWBTestRunner.SetInput("input8", input8, width/4, SHAVE0);
	pixelUnpackerWBTestRunner.SetInput("width", width, SHAVE0);
	pixelUnpackerWBTestRunner.SetInput("shift", shift, SHAVE0);
	pixelUnpackerWBTestRunner.SetInput("awbCoef", (unsigned char*)awbCoef, 4 * 2, SHAVE0);
	pixelUnpackerWBTestRunner.SetInput("line_no", line_no, SHAVE0);

	pixelUnpackerWBTestRunner.GuardInsert("output", SHAVE0, width, output);
	pixelUnpackerWBTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		pixelUnpackerWBCycleCount = pixelUnpackerWBTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(pixelUnpackerWBCycleCount - 3) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	pixelUnpackerWBTestRunner.GetOutput("output", SHAVE0, width, output);
	pixelUnpackerWBTestRunner.GuardCheck("output", SHAVE0, width, output);

}
