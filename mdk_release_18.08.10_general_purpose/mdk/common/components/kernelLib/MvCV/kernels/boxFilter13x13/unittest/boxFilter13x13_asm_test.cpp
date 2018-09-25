#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (22.33)
#define EXPECTED_CC_M2 (3.471354)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner boxFilter13x13TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int boxFilter13x13CycleCount;

void boxfilter13x13_asm_test(unsigned char** in, unsigned char** out, unsigned char normalize, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	boxFilter13x13TestRunner.Init();
	//add padding values at the end of each line
	boxFilter13x13TestRunner.SetInput("input", in, width + padding, maxWidth, 13, SHAVE0);
	boxFilter13x13TestRunner.SetInput("normalize", normalize, SHAVE0);
	boxFilter13x13TestRunner.SetInput("width", width, SHAVE0);

	boxFilter13x13TestRunner.GuardInsert(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
	boxFilter13x13TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	boxFilter13x13TestRunner.Run(SHAVE0);
	if (width >= 1280)
	{
	boxFilter13x13CycleCount = boxFilter13x13TestRunner.GetVariableValue("cycleCount");
	//value 65 substracted from boxFilterCycleCount is the value of measured cycles
	//for computations not included in boxfilter_asm function. It was obtained by
	//running the application without the function call
	functionInfo.AddCyclePerPixelInfo((float)(boxFilter13x13CycleCount - 65)/ (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	if(normalize != 1)
	{
		//for unnormalized variant the output values are 16bits, so the output size is width * 2
		boxFilter13x13TestRunner.GetOutput(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
		boxFilter13x13TestRunner.GuardCheck(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
	}
	else {
		boxFilter13x13TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
		boxFilter13x13TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
	}
}
