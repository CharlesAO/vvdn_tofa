#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (0.7)



TestRunner mvcvCensusMatching32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMatching32CycleCount;

void mvcvCensusMatching32_asm_test(unsigned int *in1, unsigned int *in2, unsigned char *out, unsigned int width, unsigned int flag)
{
    unsigned int disparities = 32;
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    const unsigned int padding = 32 ;
    
    
    mvcvCensusMatching32TestRunner.Init();
    mvcvCensusMatching32TestRunner.SetInput("input1", in1, width + padding, SHAVE0);
    mvcvCensusMatching32TestRunner.SetInput("input2", in2, width + padding, SHAVE0);
    mvcvCensusMatching32TestRunner.SetInput("output", out, width * disparities + padding, SHAVE0);
    mvcvCensusMatching32TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    mvcvCensusMatching32TestRunner.SetInput("flag", (unsigned int)flag, SHAVE0);

    mvcvCensusMatching32TestRunner.GuardInsert(string("output"), SHAVE0, width * disparities + padding, out);
    mvcvCensusMatching32TestRunner.Run(SHAVE0);
    if (width>=960)
    {
    mvcvCensusMatching32CycleCount = (mvcvCensusMatching32TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMatching32CycleCount)/ (float)(width * disparities));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvCensusMatching32TestRunner.GetOutput(string("output"), SHAVE0, width * disparities + padding, out);
    mvcvCensusMatching32TestRunner.GuardCheck(string("output"), SHAVE0, width * disparities + padding, out);
}
