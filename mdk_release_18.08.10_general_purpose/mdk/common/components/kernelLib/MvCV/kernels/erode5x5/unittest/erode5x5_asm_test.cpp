#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (7.51)
#define EXPECTED_CC_M2 (7.618099)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner Erode5x5TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int Erode5x5CycleCount;

void Erode5x5_asm_test(unsigned char** src, unsigned char** dst, unsigned char** kernel, unsigned int widthLine)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
    Erode5x5TestRunner.Init();
    Erode5x5TestRunner.SetInput("input", src, widthLine + PADDING, maxWidth, 5, SHAVE0);
    Erode5x5TestRunner.SetInput("width", widthLine, SHAVE0);
    Erode5x5TestRunner.SetInput("kernel", kernel, 5, 8, 5, SHAVE0);

    Erode5x5TestRunner.GuardInsert("output", SHAVE0, widthLine, maxWidth, 1, dst);
    Erode5x5TestRunner.Run(SHAVE0);
	if(widthLine >= 1280)
	{
		Erode5x5CycleCount = Erode5x5TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(Erode5x5CycleCount - 17) / (float)widthLine);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
	Erode5x5TestRunner.GetOutput("output", SHAVE0, widthLine, maxWidth, 1, dst);
	Erode5x5TestRunner.GuardCheck("output", SHAVE0, widthLine, maxWidth, 1, dst);
}
