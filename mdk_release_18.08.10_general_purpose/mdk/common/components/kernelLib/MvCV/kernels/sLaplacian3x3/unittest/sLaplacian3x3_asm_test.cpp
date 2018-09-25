#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.14)
#define EXPECTED_CC_M2 (0.661811)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner sLaplacian3x3TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int sLaplacian3x3CycleCount;

void sLaplacian3x3_asm_test(unsigned char** in, unsigned char** out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 8;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	sLaplacian3x3TestRunner.Init();
	//add padding values at the end of each line
	sLaplacian3x3TestRunner.SetInput("input", in, width + padding, maxWidth, 3, SHAVE0);
	
	sLaplacian3x3TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	sLaplacian3x3TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, out);
	sLaplacian3x3TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		sLaplacian3x3CycleCount = sLaplacian3x3TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(sLaplacian3x3CycleCount - 12)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	sLaplacian3x3TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, out);
	sLaplacian3x3TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, out);
}
