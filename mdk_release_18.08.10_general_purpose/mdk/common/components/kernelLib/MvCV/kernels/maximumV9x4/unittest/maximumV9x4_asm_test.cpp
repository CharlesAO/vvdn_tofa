///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.6)


TestRunner MaximumV9x4TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int MaximumV9x4CycleCount;

void MaximumV9x4_asm_test(half** in, half** out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// to calculate the last values on the line a padding of kernel_size - 1 is needed
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for linesizes lower than it
	unsigned int maxWidth = 1920;

	MaximumV9x4TestRunner.Init();
	MaximumV9x4TestRunner.SetInput("input", in, width, maxWidth, 12, SHAVE0);
	MaximumV9x4TestRunner.SetInput("output", out, width, maxWidth, 4, SHAVE0);
	MaximumV9x4TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	MaximumV9x4TestRunner.GuardInsert(string("output"), SHAVE0, width*sizeof(half), maxWidth*sizeof(half), 4, (unsigned char**)out);
	MaximumV9x4TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		MaximumV9x4CycleCount = MaximumV9x4TestRunner.GetVariableValue("cycleCount");
		//value 157 substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//runnung the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(MaximumV9x4CycleCount-28)/ (float)(width*4));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	MaximumV9x4TestRunner.GetOutput(string("output"), SHAVE0, width*sizeof(half), maxWidth*sizeof(half), 4, (unsigned char**)out);
	MaximumV9x4TestRunner.GuardCheck(string("output"), SHAVE0, width*sizeof(half), maxWidth*sizeof(half), 4, (unsigned char**)out);

}
