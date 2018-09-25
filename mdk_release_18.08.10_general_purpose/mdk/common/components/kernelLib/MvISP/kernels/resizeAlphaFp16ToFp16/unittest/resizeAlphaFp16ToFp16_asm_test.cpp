#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (2.192448)


TestRunner ResizeAlphaFp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int ResizeAlphaFp16ToFp16CycleCount;

void ResizeAlphaFp16ToFp16_asm_test(half** alpha, half* output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 3200 + padding;
	ResizeAlphaFp16ToFp16TestRunner.Init();
	//add padding values at the end of each line
	ResizeAlphaFp16ToFp16TestRunner.SetInput("input", alpha, width*2 + padding, maxWidth, 2, SHAVE0);
	ResizeAlphaFp16ToFp16TestRunner.SetInput("output", output, width + padding, SHAVE0);
	ResizeAlphaFp16ToFp16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	ResizeAlphaFp16ToFp16TestRunner.GuardInsert(string("output"), SHAVE0, width + padding, output);
	ResizeAlphaFp16ToFp16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		ResizeAlphaFp16ToFp16CycleCount = ResizeAlphaFp16ToFp16TestRunner.GetVariableValue("cycleCount");
		//the value substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(ResizeAlphaFp16ToFp16CycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	ResizeAlphaFp16ToFp16TestRunner.GetOutput(string("output"), SHAVE0, width + padding, output);
	ResizeAlphaFp16ToFp16TestRunner.GuardCheck(string("output"), SHAVE0, width + padding, output);
	}
