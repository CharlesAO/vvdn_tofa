#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (4.52)
#define EXPECTED_CC_M2 (1.443612)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner boxFilter5x5TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int boxFilter5x5CycleCount;

void boxfilter5x5_asm_test(unsigned char** in, unsigned char** out, unsigned int normalize, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	boxFilter5x5TestRunner.Init();
	//add padding values at the end of each line
	boxFilter5x5TestRunner.SetInput("input", in, width + padding, maxWidth, 5, SHAVE0);
	boxFilter5x5TestRunner.SetInput("normalize", (unsigned int)normalize, SHAVE0);
	boxFilter5x5TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	boxFilter5x5TestRunner.GuardInsert(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
	boxFilter5x5TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	boxFilter5x5TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
	boxFilter5x5CycleCount = boxFilter5x5TestRunner.GetVariableValue("cycleCount");
	//value 65 substracted from boxFilterCycleCount is the value of measured cycles
	//for computations not included in boxfilter_asm function. It was obtained by
	//running the application without the function call
	functionInfo.AddCyclePerPixelInfo((float)(boxFilter5x5CycleCount - 65)/ (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	if(normalize != 1)
	{
		//for unnormalized variant the output values are 16bits, so the output size is width * 2
		boxFilter5x5TestRunner.GetOutput(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
		boxFilter5x5TestRunner.GuardCheck(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
	}
	else {
		boxFilter5x5TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
		boxFilter5x5TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
	}
}
