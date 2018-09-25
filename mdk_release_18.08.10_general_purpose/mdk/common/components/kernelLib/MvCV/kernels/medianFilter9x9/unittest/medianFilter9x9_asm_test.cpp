#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (123.76)
#define EXPECTED_CC_M2 (130.512497)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner medianFilter9x9TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int medianFilter9x9CycleCount;

void medianFilter9x9_asm_test(unsigned int widthLine, unsigned char** outLine, unsigned char** inLine)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	medianFilter9x9TestRunner.Init();
	medianFilter9x9TestRunner.SetInput("input", inLine, widthLine + PADDING, maxWidth, 9, SHAVE0);
	medianFilter9x9TestRunner.SetInput("widthLine", widthLine, SHAVE0);

	medianFilter9x9TestRunner.GuardInsert("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter9x9TestRunner.Run(SHAVE0);
	if(widthLine >= 320)
	{
		medianFilter9x9CycleCount = medianFilter9x9TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(medianFilter9x9CycleCount - 17) / (float)widthLine);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	medianFilter9x9TestRunner.GetOutput("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter9x9TestRunner.GuardCheck("output", SHAVE0, widthLine, maxWidth, 1, outLine);
}
