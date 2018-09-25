#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (4.39)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner convSeparable11x11TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convSeparable11x11CycleCount;

void convSeparable11x11_asm_test(unsigned char** out, unsigned char** in, float conv[6], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	convSeparable11x11TestRunner.Init();
	//add padding values at the end of each line
	convSeparable11x11TestRunner.SetInput("input", in, width + padding, maxWidth, 11, SHAVE0);
	convSeparable11x11TestRunner.SetInput("conv", (float*)&conv[0], 6, SHAVE0);
	convSeparable11x11TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convSeparable11x11TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	convSeparable11x11TestRunner.Run(SHAVE0);

	convSeparable11x11CycleCount = convSeparable11x11TestRunner.GetVariableValue("cycleCount");
	
	 if(width > 128)
    {
        functionInfo.AddCyclePerPixelInfo((float)(convSeparable11x11CycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
	
	
	convSeparable11x11TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	convSeparable11x11TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
