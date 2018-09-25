#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.894531)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner pixelPacker10bTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int pixelPacker10bCycleCount;

void pixelPacker10b_asm_test(unsigned short *input, unsigned int *output32, unsigned char *output8, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	pixelPacker10bTestRunner.Init();
	pixelPacker10bTestRunner.SetInput("input", input, width, SHAVE0);
	pixelPacker10bTestRunner.SetInput("width", width, SHAVE0);
	pixelPacker10bTestRunner.GuardInsert("output32", SHAVE0, width / 4, (float*)output32);
	pixelPacker10bTestRunner.GuardInsert("output8", SHAVE0, width / 4, output8);
	pixelPacker10bTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		pixelPacker10bCycleCount = pixelPacker10bTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(pixelPacker10bCycleCount - 3) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	pixelPacker10bTestRunner.GetOutput("output32", SHAVE0, width / 4, (float*)output32);
	pixelPacker10bTestRunner.GuardCheck("output32", SHAVE0, width / 4, (float*)output32);
	
	pixelPacker10bTestRunner.GetOutput("output8", SHAVE0, width / 4, output8);
	pixelPacker10bTestRunner.GuardCheck("output8", SHAVE0, width / 4, output8);
}
