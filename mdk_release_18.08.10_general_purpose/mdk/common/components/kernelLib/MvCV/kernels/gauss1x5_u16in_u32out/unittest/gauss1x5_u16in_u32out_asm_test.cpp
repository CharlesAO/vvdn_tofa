#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (2.723438)

TestRunner gauss1x5_u16in_u32outTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int gauss1x5_u16in_u32outCycleCount;

void gauss1x5_u16in_u32out_asm_test(unsigned short** in, unsigned int** out, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int padding = 16;
	unsigned int maxWidth = 1920 + padding;

	gauss1x5_u16in_u32outTestRunner.Init();
	gauss1x5_u16in_u32outTestRunner.SetInput("input", in, width + padding, maxWidth, 1, SHAVE0);
	gauss1x5_u16in_u32outTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	
	gauss1x5_u16in_u32outTestRunner.GuardInsert("output", SHAVE0, width*4, maxWidth*4, 1, (unsigned char**)out);
	gauss1x5_u16in_u32outTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		gauss1x5_u16in_u32outCycleCount = gauss1x5_u16in_u32outTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(gauss1x5_u16in_u32outCycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	gauss1x5_u16in_u32outTestRunner.GetOutput("output", SHAVE0, width*4, maxWidth*4, 1, (unsigned char**)out);
	gauss1x5_u16in_u32outTestRunner.GuardCheck("output", SHAVE0, width*4, maxWidth*4, 1, (unsigned char**)out);
	
}
