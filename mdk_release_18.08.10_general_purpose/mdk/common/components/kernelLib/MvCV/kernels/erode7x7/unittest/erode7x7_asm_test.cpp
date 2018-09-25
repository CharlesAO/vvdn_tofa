#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (18.00)
#define EXPECTED_CC_M2 (18.175781)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner Erode7x7TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int Erode7x7CycleCount;

void Erode7x7_asm_test(unsigned char** src, unsigned char** dst, unsigned char** kernel, unsigned int widthLine)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
    Erode7x7TestRunner.Init();
    Erode7x7TestRunner.SetInput("input", src, widthLine + PADDING, maxWidth, 7, SHAVE0);
    Erode7x7TestRunner.SetInput("width", widthLine, SHAVE0);
    Erode7x7TestRunner.SetInput("kernel", kernel, 7, 8, 7, SHAVE0);

    Erode7x7TestRunner.GuardInsert("output", SHAVE0, widthLine, maxWidth, 1, dst);
    Erode7x7TestRunner.Run(SHAVE0);
	if(widthLine >= 1280)
	{
		Erode7x7CycleCount = Erode7x7TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(Erode7x7CycleCount - 17) / (float)widthLine);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
	Erode7x7TestRunner.GetOutput("output", SHAVE0, widthLine, maxWidth, 1, dst);
	Erode7x7TestRunner.GuardCheck("output", SHAVE0, widthLine, maxWidth, 1, dst);
}
