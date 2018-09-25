#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (0.5)

#define PADDING 32

TestRunner AddV2Fp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int AddV2Fp16CycleCount;

void AddV2Fp16_asm_test(half** dst, half** input, u32 width)
{

    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920;
    AddV2Fp16TestRunner.Init();
    AddV2Fp16TestRunner.SetInput("input", input, width + PADDING, maxWidth + PADDING, 2, SHAVE0);
    AddV2Fp16TestRunner.SetInput("output", dst, width + PADDING, maxWidth + PADDING, 1, SHAVE0);
    AddV2Fp16TestRunner.SetInput("width", width, SHAVE0);

    AddV2Fp16TestRunner.GuardInsert(string("output"),SHAVE0 , width + PADDING, maxWidth + PADDING, 1,dst );
    AddV2Fp16TestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        AddV2Fp16CycleCount = AddV2Fp16TestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(AddV2Fp16CycleCount - 14)/ (float)width);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }

    AddV2Fp16TestRunner.GetOutput("output", SHAVE0, width + PADDING, maxWidth + PADDING, 1, dst);
        AddV2Fp16TestRunner.GuardCheck(string("output"),SHAVE0 , width + PADDING, maxWidth + PADDING, 1,dst );
}
