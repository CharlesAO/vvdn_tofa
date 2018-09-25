#include "TestRunner.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define PADDING 32

#define EXPECTED_CC_M1 (17.42)
#define EXPECTED_CC_M2 (17.439453)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner Dilate7x7TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int Dilate7x7CycleCount;

void Dilate7x7_asm_test(unsigned char** src, unsigned char** dst, unsigned char** kernel, unsigned int width)
{
    unsigned int maxWidth = 1920 + PADDING;
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    Dilate7x7TestRunner.Init();
    Dilate7x7TestRunner.SetInput("input", src, width + PADDING, maxWidth, 7, SHAVE0);
    Dilate7x7TestRunner.SetInput("kernel", kernel, 7, 8, 7, SHAVE0);
    Dilate7x7TestRunner.SetInput("width", width, SHAVE0);
    
    Dilate7x7TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, dst);
    Dilate7x7TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		Dilate7x7CycleCount = Dilate7x7TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(Dilate7x7CycleCount - 16)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
        }
	Dilate7x7TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, dst);
	Dilate7x7TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, dst);
}






