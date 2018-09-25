#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (70.33)
#define EXPECTED_CC_M2 (74.354675)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner medianFilter3x3TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int medianFilter3x3CycleCount;

void medianFilter3x3_asm_test(unsigned int widthLine, unsigned char** outLine, unsigned char** inLine)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	medianFilter3x3TestRunner.Init();
	medianFilter3x3TestRunner.SetInput("input", inLine, widthLine + PADDING, maxWidth, 3, SHAVE0);
	medianFilter3x3TestRunner.SetInput("widthLine", widthLine, SHAVE0);

	medianFilter3x3TestRunner.GuardInsert("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter3x3TestRunner.Run(SHAVE0);
	if(widthLine >= 320)
	{
		medianFilter3x3CycleCount = medianFilter3x3TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(medianFilter3x3CycleCount - 17) / (float)widthLine);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	medianFilter3x3TestRunner.GetOutput("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter3x3TestRunner.GuardCheck("output", SHAVE0, widthLine, maxWidth, 1, outLine);
}
