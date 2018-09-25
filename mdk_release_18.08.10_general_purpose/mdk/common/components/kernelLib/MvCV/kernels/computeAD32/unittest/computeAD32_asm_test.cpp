#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.42)
#define PADDING 32 
#define DISPARITIES 32

TestRunner mvcvComputeAD32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvComputeAD32CycleCount;

void mvcvComputeAD32_asm_test(unsigned char *in1, unsigned char *in2, unsigned char *out, unsigned int width, unsigned int flag)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    mvcvComputeAD32TestRunner.Init();
    mvcvComputeAD32TestRunner.SetInput("input1", in1, width * DISPARITIES + PADDING, SHAVE0);
    mvcvComputeAD32TestRunner.SetInput("input2", in2, width * DISPARITIES + PADDING, SHAVE0);
    mvcvComputeAD32TestRunner.SetInput("output", out, width * DISPARITIES + PADDING, SHAVE0);
    mvcvComputeAD32TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    mvcvComputeAD32TestRunner.SetInput("flag", (unsigned int)flag, SHAVE0);

    mvcvComputeAD32TestRunner.GuardInsert(string("output"), SHAVE0, width * DISPARITIES + PADDING, out);
    mvcvComputeAD32TestRunner.Run(SHAVE0);
    if(width >= 960)
    {
        mvcvComputeAD32CycleCount = (mvcvComputeAD32TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
        functionInfo.AddCyclePerPixelInfo((float)(mvcvComputeAD32CycleCount)/ (float)(width * DISPARITIES));
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvComputeAD32TestRunner.GetOutput(string("output"), SHAVE0, width * DISPARITIES + PADDING, out);
    mvcvComputeAD32TestRunner.GuardCheck(string("output"), SHAVE0, width * DISPARITIES + PADDING, out);
}
