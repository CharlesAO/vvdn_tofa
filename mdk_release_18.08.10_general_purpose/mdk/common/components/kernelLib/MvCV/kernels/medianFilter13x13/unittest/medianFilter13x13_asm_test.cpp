#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (154.59)
#define EXPECTED_CC_M2 (169.581085)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner medianFilter13x13TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int medianFilter13x13CycleCount;

void medianFilter13x13_asm_test(unsigned int widthLine, unsigned char** outLine, unsigned char** inLine)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	medianFilter13x13TestRunner.Init();
	medianFilter13x13TestRunner.SetInput("input", inLine, widthLine + PADDING, maxWidth, 13, SHAVE0);
	medianFilter13x13TestRunner.SetInput("widthLine", widthLine, SHAVE0);

	medianFilter13x13TestRunner.GuardInsert("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter13x13TestRunner.Run(SHAVE0);
	if(widthLine >= 320)
	{
		medianFilter13x13CycleCount = medianFilter13x13TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(medianFilter13x13CycleCount - 17) / (float)widthLine);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	medianFilter13x13TestRunner.GetOutput("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter13x13TestRunner.GuardCheck("output", SHAVE0, widthLine, maxWidth, 1, outLine);
}
