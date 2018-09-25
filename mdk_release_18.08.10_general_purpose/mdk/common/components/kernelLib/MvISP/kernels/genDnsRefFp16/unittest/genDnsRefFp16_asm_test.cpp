#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include "genDnsRefFp16_asm_test.h"
#include "genDnsRefFp16.h"

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (3.535681)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner appTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void genDnsRefFp16_asm_test(half *inY, unsigned int yc, int x0,  
    YDnsRefFp16Param* param, unsigned char* outRef, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	appTestRunner.Init();

	appTestRunner.SetInput("input", inY, width, SHAVE0);
	appTestRunner.SetInput("yc", (unsigned int)yc, SHAVE0);
    appTestRunner.SetInput("x0", (unsigned int)x0, SHAVE0);
    appTestRunner.SetInput("lutGamma", param->lutGamma, 256, SHAVE0);
    appTestRunner.SetInput("lutDist", param->lutDist, 256, SHAVE0);
    appTestRunner.SetInput("shift", (unsigned int)param->shift, SHAVE0);
    appTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	appTestRunner.GuardInsert(string("output"), SHAVE0, width, outRef);
	appTestRunner.Run(SHAVE0);
    if (width >= 1280)
    {
        cycleCount = appTestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(cycleCount)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }    

	appTestRunner.GetOutput(string("output"), SHAVE0, width, outRef);
	appTestRunner.GuardCheck(string("output"), SHAVE0, width, outRef);
}
