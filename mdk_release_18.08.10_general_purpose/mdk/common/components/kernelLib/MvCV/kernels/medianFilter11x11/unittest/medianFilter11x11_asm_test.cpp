#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (137.90)
#define EXPECTED_CC_M2 (149.72139)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner medianFilter11x11TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int medianFilter11x11CycleCount;

void medianFilter11x11_asm_test(unsigned int widthLine, unsigned char** outLine, unsigned char** inLine)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
	medianFilter11x11TestRunner.Init();
	medianFilter11x11TestRunner.SetInput("input", inLine, widthLine + PADDING, maxWidth, 11, SHAVE0);
	medianFilter11x11TestRunner.SetInput("widthLine", widthLine, SHAVE0);

	medianFilter11x11TestRunner.GuardInsert("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter11x11TestRunner.Run(SHAVE0);
	if(widthLine >= 320)
	{
		medianFilter11x11CycleCount = medianFilter11x11TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(medianFilter11x11CycleCount - 17) / (float)widthLine);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	medianFilter11x11TestRunner.GetOutput("output", SHAVE0, widthLine, maxWidth, 1, outLine);
	medianFilter11x11TestRunner.GuardCheck("output", SHAVE0, widthLine, maxWidth, 1, outLine);
}
