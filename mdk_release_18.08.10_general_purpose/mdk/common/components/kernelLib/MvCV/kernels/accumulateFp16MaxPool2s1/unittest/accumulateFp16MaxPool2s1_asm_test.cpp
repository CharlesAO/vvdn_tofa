#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (0.7)

#define PADDING 32

TestRunner AccumulateFp16MaxPool2s1TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int AccumulateFp16MaxPool2s1CycleCount;

void AccumulateFp16MaxPool2s1_asm_test(half** dst, half** input, u32 width)
{

    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920;
    AccumulateFp16MaxPool2s1TestRunner.Init();
    AccumulateFp16MaxPool2s1TestRunner.SetInput("input", input, width + PADDING, maxWidth + PADDING, 1, SHAVE0);
    AccumulateFp16MaxPool2s1TestRunner.SetInput("output", dst, width + PADDING, maxWidth + PADDING, 1, SHAVE0);
    AccumulateFp16MaxPool2s1TestRunner.SetInput("width", width, SHAVE0);

    AccumulateFp16MaxPool2s1TestRunner.GuardInsert("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
    AccumulateFp16MaxPool2s1TestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        AccumulateFp16MaxPool2s1CycleCount = AccumulateFp16MaxPool2s1TestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(AccumulateFp16MaxPool2s1CycleCount - 14)/ (float)width);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    
    AccumulateFp16MaxPool2s1TestRunner.GetOutput("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
    AccumulateFp16MaxPool2s1TestRunner.GuardCheck("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
}
