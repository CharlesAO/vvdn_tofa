#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (5.99)
#define EXPECTED_CC_M2 (2.605097)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convSeparable7x7TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convSeparable7x7CycleCount;

void convSeparable7x7_asm_test(unsigned char** out, unsigned char** in, float conv[4], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convSeparable7x7TestRunner.Init();
	//add padding values at the end of each line
	convSeparable7x7TestRunner.SetInput("input", in, width + padding, maxWidth, 7, SHAVE0);
	convSeparable7x7TestRunner.SetInput("conv", (float*)&conv[0], 4, SHAVE0);
	convSeparable7x7TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convSeparable7x7TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convSeparable7x7TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convSeparable7x7CycleCount = convSeparable7x7TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convSeparable7x7CycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
	
	convSeparable7x7TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convSeparable7x7TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
