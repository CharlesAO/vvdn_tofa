#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (3.5)

#define PADDING 32

TestRunner AccumulateFp16MaxPool3s2withReLUTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int AccumulateFp16MaxPool3s2withReLUCycleCount;

void AccumulateFp16MaxPool3s2withReLU_asm_test(half** dst, half** input, u32 width)
{

    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920;
    AccumulateFp16MaxPool3s2withReLUTestRunner.Init();
    AccumulateFp16MaxPool3s2withReLUTestRunner.SetInput("input", input, width + PADDING, maxWidth + PADDING, 1, SHAVE0);
    AccumulateFp16MaxPool3s2withReLUTestRunner.SetInput("output", dst, width + PADDING, maxWidth + PADDING, 1, SHAVE0);
    AccumulateFp16MaxPool3s2withReLUTestRunner.SetInput("width", width, SHAVE0);

    AccumulateFp16MaxPool3s2withReLUTestRunner.GuardInsert("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
    AccumulateFp16MaxPool3s2withReLUTestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        AccumulateFp16MaxPool3s2withReLUCycleCount = AccumulateFp16MaxPool3s2withReLUTestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(AccumulateFp16MaxPool3s2withReLUCycleCount - 14)/ (((float)width)/2));
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    
    AccumulateFp16MaxPool3s2withReLUTestRunner.GetOutput("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
    AccumulateFp16MaxPool3s2withReLUTestRunner.GuardCheck("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
}
