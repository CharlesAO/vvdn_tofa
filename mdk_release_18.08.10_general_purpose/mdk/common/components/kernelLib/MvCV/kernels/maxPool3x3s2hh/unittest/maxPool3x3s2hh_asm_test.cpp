#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (2.034)
#define STRIDE 2
#define PADDING 8


TestRunner maxPool3x3s2hhTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int maxPool3x3s2hhCycleCount;

void maxPool3x3s2hh_asm_test(half **input, half **output, unsigned int outputWidth)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920 + PADDING;
    unsigned int inputWidth = outputWidth * STRIDE;
    maxPool3x3s2hhTestRunner.Init();
    maxPool3x3s2hhTestRunner.SetInput("input", input, inputWidth + 8, maxWidth, 3, SHAVE0);
    maxPool3x3s2hhTestRunner.SetInput("width", outputWidth, SHAVE0);

    maxPool3x3s2hhTestRunner.GuardInsert("output", SHAVE0, outputWidth, maxWidth, 1, output);
    maxPool3x3s2hhTestRunner.Run(SHAVE0);
    if(inputWidth >= 1280)
    {
        maxPool3x3s2hhCycleCount = maxPool3x3s2hhTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(maxPool3x3s2hhCycleCount - 14) / (float)outputWidth);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    maxPool3x3s2hhTestRunner.GetOutput("output", SHAVE0, outputWidth, maxWidth, 1, output);
    maxPool3x3s2hhTestRunner.GuardCheck("output", SHAVE0, outputWidth, maxWidth, 1, output);

}
