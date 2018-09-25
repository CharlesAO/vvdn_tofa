#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (32.18)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner bilateral5x5TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int bilateral5x5CycleCount;

void bilateral5x5_asm_test(unsigned short *input, unsigned short *sigma, unsigned int width, unsigned short *output)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	bilateral5x5TestRunner.Init();
	bilateral5x5TestRunner.SetInput("input", input, width * 5, SHAVE0);
	bilateral5x5TestRunner.SetInput("sigma", sigma, width, SHAVE0);
	bilateral5x5TestRunner.SetInput("width", width, SHAVE0);

	bilateral5x5TestRunner.GuardInsert("output", SHAVE0, width, output);
	bilateral5x5TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		bilateral5x5CycleCount = bilateral5x5TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(bilateral5x5CycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	bilateral5x5TestRunner.GetOutput("output", SHAVE0, width, output);
	bilateral5x5TestRunner.GuardCheck("output", SHAVE0, width, output);

}
