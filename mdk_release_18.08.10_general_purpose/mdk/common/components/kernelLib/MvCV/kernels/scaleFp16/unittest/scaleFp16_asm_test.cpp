///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.3)


TestRunner scaleFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int scaleFp16CycleCount = 0;

void scaleFp16_asm_test(half** in, half** out, half scale, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// to calculate the last values on the line a padding of kernel_size - 1 is needed
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for linesizes lower than it
	unsigned int maxWidth = 1920;

	scaleFp16TestRunner.Init();
	scaleFp16TestRunner.SetInput("input", in, width, maxWidth, 1, SHAVE0);
	scaleFp16TestRunner.SetInput("output", out, width, maxWidth, 1, SHAVE0);
    scaleFp16TestRunner.SetInput("scale", scale.getPackedValue(), SHAVE0);
	scaleFp16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	scaleFp16TestRunner.GuardInsert("output", SHAVE0,  width*sizeof(half), maxWidth*sizeof(half), 1, (unsigned char**)out);

	scaleFp16TestRunner.Run(SHAVE0);
	if(width >= 1000)
	{
	    scaleFp16CycleCount = scaleFp16TestRunner.GetVariableValue("cycleCount");
	    functionInfo.AddCyclePerPixelInfo((float)(scaleFp16CycleCount)/ (float)(width));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	scaleFp16TestRunner.GetOutput(string("output"), SHAVE0, width*sizeof(half), maxWidth*sizeof(half), 1, (unsigned char**)out);
	scaleFp16TestRunner.GuardCheck("output", SHAVE0,  width*sizeof(half), maxWidth*sizeof(half), 1, (unsigned char**)out);
}


