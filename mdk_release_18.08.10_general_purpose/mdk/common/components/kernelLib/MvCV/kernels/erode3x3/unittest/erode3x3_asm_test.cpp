#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (2.52)
#define EXPECTED_CC_M2 (3.027344)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner Erode3x3TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int Erode3x3CycleCount;

void Erode3x3_asm_test(unsigned char** src, unsigned char** dst, unsigned char** kernel, unsigned int widthLine)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING;
    Erode3x3TestRunner.Init();
    Erode3x3TestRunner.SetInput("input", src, widthLine + PADDING, maxWidth, 3, SHAVE0);
    Erode3x3TestRunner.SetInput("width", widthLine, SHAVE0);
    Erode3x3TestRunner.SetInput("kernel", kernel, 3, 4, 3, SHAVE0);

    Erode3x3TestRunner.GuardInsert("output", SHAVE0, widthLine, maxWidth, 1, dst);
    Erode3x3TestRunner.Run(SHAVE0);
	if(widthLine >= 1280)
	{
		Erode3x3CycleCount = Erode3x3TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(Erode3x3CycleCount - 17) / (float)widthLine);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
   Erode3x3TestRunner.GetOutput("output", SHAVE0, widthLine, maxWidth, 1, dst);
   Erode3x3TestRunner.GuardCheck("output", SHAVE0, widthLine, maxWidth, 1, dst);
}
