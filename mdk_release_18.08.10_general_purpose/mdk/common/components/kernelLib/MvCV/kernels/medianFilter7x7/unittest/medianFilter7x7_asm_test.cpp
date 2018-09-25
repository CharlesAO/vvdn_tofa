#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (105.75)
#define EXPECTED_CC_M2 (110.462425)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner medianFilter7x7TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int medianFilter7x7CycleCount;

void medianFilter7x7_asm_test(unsigned int widthLine, unsigned char** outLine, unsigned char** inLine)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	medianFilter7x7TestRunner.Init();
	medianFilter7x7TestRunner.SetInput("input", inLine, widthLine + PADDING, maxWidth, 7, SHAVE0);
	medianFilter7x7TestRunner.SetInput("widthLine", widthLine, SHAVE0);

	medianFilter7x7TestRunner.GuardInsert("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter7x7TestRunner.Run(SHAVE0);
	if(widthLine >= 1280)
	{
		medianFilter7x7CycleCount = medianFilter7x7TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(medianFilter7x7CycleCount - 17) / (float)widthLine);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	medianFilter7x7TestRunner.GetOutput("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter7x7TestRunner.GuardCheck("output", SHAVE0, widthLine, maxWidth, 1, outLine);
}
