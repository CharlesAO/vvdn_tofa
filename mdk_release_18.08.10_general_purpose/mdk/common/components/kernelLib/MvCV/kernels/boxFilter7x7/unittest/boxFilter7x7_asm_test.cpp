#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (7.47)
#define EXPECTED_CC_M2 (1.9528)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner boxFilter7x7TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int boxFilter7x7CycleCount;

void boxfilter7x7_asm_test(unsigned char** in, unsigned char** out, unsigned int normalize, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	boxFilter7x7TestRunner.Init();
	//add padding values at the end of each line
	boxFilter7x7TestRunner.SetInput("input", in, width + padding, maxWidth, 7, SHAVE0);
	boxFilter7x7TestRunner.SetInput("normalize", (unsigned int)normalize, SHAVE0);
	boxFilter7x7TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

        if(normalize != 1)
        {
	boxFilter7x7TestRunner.GuardInsert(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
	}
        else
        { 
        boxFilter7x7TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	}
        boxFilter7x7TestRunner.Run(SHAVE0);

	if (width >= 1280)
	{
		boxFilter7x7CycleCount = boxFilter7x7TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(boxFilter7x7CycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}

	if(normalize != 1)
	{
		//for unnormalized variant the output values are 16bits, so the output size is width * 2
		boxFilter7x7TestRunner.GetOutput(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
		boxFilter7x7TestRunner.GuardCheck(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
	}
	else {
		boxFilter7x7TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
		boxFilter7x7TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
	}
}
