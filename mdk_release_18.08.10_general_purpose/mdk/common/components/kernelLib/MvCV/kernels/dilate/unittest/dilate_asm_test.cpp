#include "TestRunner.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define PADDING 8

#define EXPECTED_CC_M1 (720.33)
#define EXPECTED_CC_M2 (36.54427)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner DilateTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int DilateCycleCount;

void Dilate_asm_test(unsigned char** src, unsigned char** dst, unsigned char** kernel, unsigned int width, unsigned int height, unsigned int k)
{
	unsigned int maxWidth = 1920 + PADDING;
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    DilateTestRunner.Init();
    DilateTestRunner.SetInput("input", src, width + PADDING, maxWidth, k, SHAVE0);
    DilateTestRunner.SetInput("kernel", kernel, k, 8, k, SHAVE0);
    DilateTestRunner.SetInput("width", width, SHAVE0);
    DilateTestRunner.SetInput("height", height, SHAVE0);
    DilateTestRunner.SetInput("k", k, SHAVE0);

    DilateTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, dst);
    DilateTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		DilateCycleCount = DilateTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(DilateCycleCount - 16)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}		
    DilateTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, dst);
    DilateTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, dst);
}
