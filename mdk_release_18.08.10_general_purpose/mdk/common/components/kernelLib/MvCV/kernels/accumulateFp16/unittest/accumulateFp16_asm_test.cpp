#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (0.5)

#define PADDING 32

TestRunner AccumulateFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int AccumulateFp16CycleCount;

void AccumulateFp16_asm_test(half** dst, half** input, u32 width)
{

    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920;
    AccumulateFp16TestRunner.Init();
    AccumulateFp16TestRunner.SetInput("input", input, width + PADDING, maxWidth + PADDING, 1, SHAVE0);
    AccumulateFp16TestRunner.SetInput("output", dst, width + PADDING, maxWidth + PADDING, 1, SHAVE0);
    AccumulateFp16TestRunner.SetInput("width", width, SHAVE0);

    AccumulateFp16TestRunner.GuardInsert("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
    AccumulateFp16TestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        AccumulateFp16CycleCount = AccumulateFp16TestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(AccumulateFp16CycleCount - 14)/ (float)width);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    
    AccumulateFp16TestRunner.GetOutput("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
    AccumulateFp16TestRunner.GuardCheck("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
}
