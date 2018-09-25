///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (153.231339)
#define EXPECTED_CC_M2 (169.850479)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif


TestRunner boxFilterTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int boxFilterCycleCount;

void boxfilter_asm_test(unsigned char** in, unsigned char** out, unsigned int kernelSize, unsigned int normalize, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for linesizes lower than it
	unsigned int padding = 16;
	unsigned int maxWidth = 1920 + padding;

	boxFilterTestRunner.Init();
    boxFilterTestRunner.SetInput("input", in, width + padding, maxWidth, kernelSize, SHAVE0);
    boxFilterTestRunner.SetInput("normalize", (unsigned int)normalize, SHAVE0);
    boxFilterTestRunner.SetInput("kernelSize", (unsigned int)kernelSize, SHAVE0);
    boxFilterTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	boxFilterTestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	boxFilterTestRunner.Run(SHAVE0);

	boxFilterCycleCount = boxFilterTestRunner.GetVariableValue("cycleCount");
	//value 157 substracted from boxFilterCycleCount is the value of measured cycles
	//for computations not included in boxfilter_asm function. It was obtained by
	//runnung the application without the function call
	functionInfo.AddCyclePerPixelInfo((float)(boxFilterCycleCount - 157)/ (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	boxFilterTestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	boxFilterTestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
