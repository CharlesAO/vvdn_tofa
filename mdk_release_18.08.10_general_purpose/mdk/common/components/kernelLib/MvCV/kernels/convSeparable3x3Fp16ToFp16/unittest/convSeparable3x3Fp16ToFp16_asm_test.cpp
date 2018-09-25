#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC_M2 (1.034028)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#endif

TestRunner appTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void convSeparable3x3Fp16ToFp16_asm_test(half* out, half** in, half conv[2], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	appTestRunner.Init();
	//add padding values at the end of each line
	appTestRunner.SetInput("input", in, width + padding, maxWidth, 3, SHAVE0);
	appTestRunner.SetInput("conv", (half*)&conv[0], 2, SHAVE0);
	appTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	appTestRunner.GuardInsert(string("output"), SHAVE0, width, out);
	appTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cycleCount = appTestRunner.GetVariableValue("cycleCount");
		//value 157 substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 7)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
	appTestRunner.GetOutput(string("output"), SHAVE0, width, out);
	appTestRunner.GuardCheck(string("output"), SHAVE0, width, out);
}
