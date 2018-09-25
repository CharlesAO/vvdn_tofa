#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (180.58)
#define EXPECTED_CC_M2 (184.029968)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner medianFilter15x15TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int medianFilter15x15CycleCount;

void medianFilter15x15_asm_test(unsigned int widthLine, unsigned char** outLine, unsigned char** inLine)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	medianFilter15x15TestRunner.Init();
	medianFilter15x15TestRunner.SetInput("input", inLine, widthLine + PADDING, maxWidth, 15, SHAVE0);
	medianFilter15x15TestRunner.SetInput("widthLine", widthLine, SHAVE0);

	medianFilter15x15TestRunner.GuardInsert("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter15x15TestRunner.Run(SHAVE0);
	if(widthLine >= 1280)
	{
		medianFilter15x15CycleCount = medianFilter15x15TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(medianFilter15x15CycleCount - 17) / (float)widthLine);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	medianFilter15x15TestRunner.GetOutput("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter15x15TestRunner.GuardCheck("output", SHAVE0, widthLine, maxWidth, 1, outLine);
}
