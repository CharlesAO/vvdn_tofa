///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.6)


TestRunner MaximumV2TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int MaximumV2CycleCount;

void MaximumV2_asm_test(half** in, half** out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// to calculate the last values on the line a padding of kernel_size - 1 is needed
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for linesizes lower than it
	unsigned int maxWidth = 1920;

	MaximumV2TestRunner.Init();
	MaximumV2TestRunner.SetInput("input", in, width, maxWidth, 2, SHAVE0);
	MaximumV2TestRunner.SetInput("output", out, width, maxWidth, 1, SHAVE0);
	MaximumV2TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	MaximumV2TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		MaximumV2CycleCount = MaximumV2TestRunner.GetVariableValue("cycleCount");
		//value 157 substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//runnung the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(MaximumV2CycleCount-28)/ (float)(width));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	MaximumV2TestRunner.GetOutput(string("output"), SHAVE0, width*sizeof(half), maxWidth*sizeof(half), 1, (unsigned char**)out);

}
