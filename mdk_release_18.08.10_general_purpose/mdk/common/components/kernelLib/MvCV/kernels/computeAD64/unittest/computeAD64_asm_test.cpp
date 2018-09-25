#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.27)
#define PADDING 64 
#define DISPARITIES 64

TestRunner mvcvComputeAD64TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvComputeAD64CycleCount;

void mvcvComputeAD64_asm_test(unsigned char *in1, unsigned char *in2, unsigned char *out, unsigned int width, unsigned int flag)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    mvcvComputeAD64TestRunner.Init();
    mvcvComputeAD64TestRunner.SetInput("input1", in1, width * DISPARITIES + PADDING, SHAVE0);
    mvcvComputeAD64TestRunner.SetInput("input2", in2, width * DISPARITIES + PADDING, SHAVE0);
    mvcvComputeAD64TestRunner.SetInput("output", out, width * DISPARITIES + PADDING, SHAVE0);
    mvcvComputeAD64TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    mvcvComputeAD64TestRunner.SetInput("flag", (unsigned int)flag, SHAVE0);

    mvcvComputeAD64TestRunner.GuardInsert(string("output"), SHAVE0, width * DISPARITIES + PADDING, out);
    mvcvComputeAD64TestRunner.Run(SHAVE0);
    if(width >= 960)
    {
        mvcvComputeAD64CycleCount = (mvcvComputeAD64TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
        functionInfo.AddCyclePerPixelInfo((float)(mvcvComputeAD64CycleCount)/ (float)(width * DISPARITIES));
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvComputeAD64TestRunner.GetOutput(string("output"), SHAVE0, width * DISPARITIES + PADDING, out);
    mvcvComputeAD64TestRunner.GuardCheck(string("output"), SHAVE0, width * DISPARITIES + PADDING, out);
}
