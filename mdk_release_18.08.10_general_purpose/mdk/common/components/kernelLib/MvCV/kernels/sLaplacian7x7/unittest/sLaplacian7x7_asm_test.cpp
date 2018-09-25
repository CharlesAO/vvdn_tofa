#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (10.79)
#define EXPECTED_CC_M2 (5.675)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner sLaplacian7x7TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int sLaplacian7x7CycleCount;

void sLaplacian7x7_asm_test(unsigned char** in, unsigned char** out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 8;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	sLaplacian7x7TestRunner.Init();
	//add padding values at the end of each line
	sLaplacian7x7TestRunner.SetInput("input", in, width + padding, maxWidth, 7, SHAVE0);
	
	sLaplacian7x7TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	sLaplacian7x7TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, out);
	sLaplacian7x7TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		sLaplacian7x7CycleCount = sLaplacian7x7TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(sLaplacian7x7CycleCount - 18)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	sLaplacian7x7TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, out);
	sLaplacian7x7TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, out);
}
