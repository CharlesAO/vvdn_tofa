#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (1.55)

#define PADDING 32

TestRunner AccumulateFp16MaxPool2s2TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int AccumulateFp16MaxPool2s2CycleCount;

void AccumulateFp16MaxPool2s2_asm_test(half** dst, half** input, u32 width)
{

    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920;
    AccumulateFp16MaxPool2s2TestRunner.Init();
    AccumulateFp16MaxPool2s2TestRunner.SetInput("input", input, width + PADDING, maxWidth + PADDING, 1, SHAVE0);
    AccumulateFp16MaxPool2s2TestRunner.SetInput("output", dst, width + PADDING, maxWidth + PADDING, 1, SHAVE0);
    AccumulateFp16MaxPool2s2TestRunner.SetInput("width", width, SHAVE0);

    AccumulateFp16MaxPool2s2TestRunner.GuardInsert("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
    AccumulateFp16MaxPool2s2TestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        AccumulateFp16MaxPool2s2CycleCount = AccumulateFp16MaxPool2s2TestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(AccumulateFp16MaxPool2s2CycleCount - 14)/ (((float)width)/2));
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    
    AccumulateFp16MaxPool2s2TestRunner.GetOutput("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
    AccumulateFp16MaxPool2s2TestRunner.GuardCheck("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
}
