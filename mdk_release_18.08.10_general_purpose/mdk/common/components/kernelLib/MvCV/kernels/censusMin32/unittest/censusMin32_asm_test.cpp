#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (42)

TestRunner mvcvCensusMin32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMin32CycleCount;

void mvcvCensusMin32_asm_test(unsigned char *in1, unsigned char *out, unsigned char *outm, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    const unsigned int disparities = 32 ;
    const unsigned int PADDING = 32 ;
    
    
    mvcvCensusMin32TestRunner.Init();
    mvcvCensusMin32TestRunner.SetInput("input1", in1, width * disparities, SHAVE0);
    mvcvCensusMin32TestRunner.SetInput("output", out, width + PADDING, SHAVE0);
    mvcvCensusMin32TestRunner.SetInput("outputm", outm, width + PADDING, SHAVE0);
    mvcvCensusMin32TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

    mvcvCensusMin32TestRunner.GuardInsert(string("output"), SHAVE0, width + PADDING, out);
    mvcvCensusMin32TestRunner.GuardInsert(string("outputm"), SHAVE0, width + PADDING, outm);
    mvcvCensusMin32TestRunner.Run(SHAVE0);
    if(width >= 960)
    {
    mvcvCensusMin32CycleCount = (mvcvCensusMin32TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMin32CycleCount)/ (float)(width));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvCensusMin32TestRunner.GetOutput(string("output"), SHAVE0, width + PADDING, out);
    mvcvCensusMin32TestRunner.GuardCheck(string("output"), SHAVE0, width + PADDING, out);
    mvcvCensusMin32TestRunner.GetOutput(string("outputm"), SHAVE0, width + PADDING, outm);
    mvcvCensusMin32TestRunner.GuardCheck(string("outputm"), SHAVE0, width + PADDING, outm);
}
