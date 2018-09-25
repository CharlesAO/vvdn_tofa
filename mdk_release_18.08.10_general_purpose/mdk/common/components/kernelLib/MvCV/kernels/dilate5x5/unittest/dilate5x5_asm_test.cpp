#include "TestRunner.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (7.08)
#define EXPECTED_CC_M2 (6.754687)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner Dilate5x5TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int Dilate5x5CycleCount;

void Dilate5x5_asm_test(unsigned char** src, unsigned char** dst, unsigned char** kernel, unsigned int width)
{
	unsigned int maxWidth = 1920 + PADDING;
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    Dilate5x5TestRunner.Init();
    Dilate5x5TestRunner.SetInput("input", src, width + PADDING, maxWidth, 5, SHAVE0);
    Dilate5x5TestRunner.SetInput("output", dst, width + PADDING, maxWidth, 1, SHAVE0);
    Dilate5x5TestRunner.SetInput("kernel", kernel, 5, 8, 5, SHAVE0);
    Dilate5x5TestRunner.SetInput("width", width, SHAVE0);
    
    Dilate5x5TestRunner.GuardInsert("output", SHAVE0, width + PADDING, maxWidth, 1, dst);
    Dilate5x5TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		Dilate5x5CycleCount = Dilate5x5TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(Dilate5x5CycleCount - 16)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}		
    Dilate5x5TestRunner.GetOutput("output", SHAVE0, width + PADDING, maxWidth, 1, dst);
    Dilate5x5TestRunner.GuardCheck("output", SHAVE0, width + PADDING, maxWidth, 1, dst);
}
