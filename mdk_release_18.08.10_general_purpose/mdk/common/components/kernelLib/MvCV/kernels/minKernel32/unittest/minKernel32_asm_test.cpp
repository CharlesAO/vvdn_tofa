#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (37)
#define WIDTH 32

TestRunner mvcvMinKernel32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvMinKernel32CycleCount;

void mvcvMinKernel32_asm_test(unsigned char *input, unsigned char* min)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    mvcvMinKernel32TestRunner.Init();
    mvcvMinKernel32TestRunner.SetInput(   "input", input, WIDTH, SHAVE0);

    mvcvMinKernel32TestRunner.GuardInsert(string("min"), SHAVE0, 1, min);
    mvcvMinKernel32TestRunner.Run(SHAVE0);

    mvcvMinKernel32CycleCount = (mvcvMinKernel32TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvMinKernel32CycleCount));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);

    mvcvMinKernel32TestRunner.GetOutput(string("min"), SHAVE0, 1, min);
    mvcvMinKernel32TestRunner.GuardCheck(string("min"), SHAVE0, 1, min);
}
