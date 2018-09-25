///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.6)


TestRunner MaximumV3TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int MaximumV3CycleCount;

void MaximumV3_asm_test(half** in, half** out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// to calculate the last values on the line a padding of kernel_size - 1 is needed
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for linesizes lower than it
	unsigned int maxWidth = 1920;

	MaximumV3TestRunner.Init();
	MaximumV3TestRunner.SetInput("input", in, width, maxWidth, 3, SHAVE0);
	MaximumV3TestRunner.SetInput("output", out, width, maxWidth, 1, SHAVE0);
	MaximumV3TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	MaximumV3TestRunner.GuardInsert(string("output"), SHAVE0, width*sizeof(half), maxWidth*sizeof(half), 1, (unsigned char**)out);
	MaximumV3TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		MaximumV3CycleCount = MaximumV3TestRunner.GetVariableValue("cycleCount");
		//value 157 substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//runnung the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(MaximumV3CycleCount-28)/ (float)(width));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	MaximumV3TestRunner.GetOutput(string("output"), SHAVE0, width*sizeof(half), maxWidth*sizeof(half), 1, (unsigned char**)out);
	MaximumV3TestRunner.GuardCheck(string("output"), SHAVE0, width*sizeof(half), maxWidth*sizeof(half), 1, (unsigned char**)out);

}
