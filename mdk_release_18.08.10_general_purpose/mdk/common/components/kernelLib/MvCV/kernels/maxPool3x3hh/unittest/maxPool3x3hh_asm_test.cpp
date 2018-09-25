#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (2.017)
#define PADDING 8


TestRunner maxPool3x3hhTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int maxPool3x3hhCycleCount;

void maxPool3x3hh_asm_test(half **input, half **output, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920 + PADDING;
    maxPool3x3hhTestRunner.Init();
    maxPool3x3hhTestRunner.SetInput("input", input, width + 8, maxWidth, 3, SHAVE0);
    maxPool3x3hhTestRunner.SetInput("width", width, SHAVE0);

    maxPool3x3hhTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
    maxPool3x3hhTestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        maxPool3x3hhCycleCount = maxPool3x3hhTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(maxPool3x3hhCycleCount - 14) / (float)width);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    maxPool3x3hhTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
    maxPool3x3hhTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
