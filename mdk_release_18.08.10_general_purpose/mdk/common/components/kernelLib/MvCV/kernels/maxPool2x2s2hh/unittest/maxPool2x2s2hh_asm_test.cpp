#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (1.66)
#define STRIDE 2
#define PADDING 8


TestRunner maxPool2x2s2hhTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int maxPool2x2s2hhCycleCount;

void maxPool2x2s2hh_asm_test(half **input, half **output, unsigned int outputWidth)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920 + PADDING;
    unsigned int inputWidth = outputWidth * STRIDE;
    maxPool2x2s2hhTestRunner.Init();
    maxPool2x2s2hhTestRunner.SetInput("input", input, inputWidth + 8, maxWidth, 2, SHAVE0);
    maxPool2x2s2hhTestRunner.SetInput("width", outputWidth, SHAVE0);

    maxPool2x2s2hhTestRunner.GuardInsert("output", SHAVE0, outputWidth, maxWidth, 1, output);
    maxPool2x2s2hhTestRunner.Run(SHAVE0);
    if(inputWidth >= 1280)
    {
        maxPool2x2s2hhCycleCount = maxPool2x2s2hhTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(maxPool2x2s2hhCycleCount - 11) / (float)outputWidth);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    maxPool2x2s2hhTestRunner.GetOutput("output", SHAVE0, outputWidth, maxWidth, 1, output);
    maxPool2x2s2hhTestRunner.GuardCheck("output", SHAVE0, outputWidth, maxWidth, 1, output);

}
