#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (1.291324)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner appTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void randNoiseFp16_asm_test(half *output, half *input, float noiseStrength, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	appTestRunner.Init();

	appTestRunner.SetInput("input", input, width, SHAVE0);
	appTestRunner.SetInput("noiseStrength", noiseStrength, SHAVE0);
	appTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	appTestRunner.GuardInsert(string("output"), SHAVE0, width, output);
	appTestRunner.Run(SHAVE0);
    if (width >= 1280)
    {
        cycleCount = appTestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(cycleCount)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }    

	appTestRunner.GetOutput(string("output"), SHAVE0, width, output);
	appTestRunner.GuardCheck(string("output"), SHAVE0, width, output);
}
