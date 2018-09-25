#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (86.71)
#define EXPECTED_CC_M2 (92.829895)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner medianFilter5x5TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int medianFilter5x5CycleCount;

void medianFilter5x5_asm_test(unsigned int widthLine, unsigned char** outLine, unsigned char** inLine)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	medianFilter5x5TestRunner.Init();
	medianFilter5x5TestRunner.SetInput("input", inLine, widthLine + PADDING, maxWidth, 5, SHAVE0);
	medianFilter5x5TestRunner.SetInput("widthLine", widthLine, SHAVE0);

	medianFilter5x5TestRunner.GuardInsert("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter5x5TestRunner.Run(SHAVE0);
	if(widthLine >= 320)
	{
		medianFilter5x5CycleCount = medianFilter5x5TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(medianFilter5x5CycleCount - 17) / (float)widthLine);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	medianFilter5x5TestRunner.GetOutput("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter5x5TestRunner.GuardCheck("output", SHAVE0, widthLine, maxWidth, 1, outLine);
}
