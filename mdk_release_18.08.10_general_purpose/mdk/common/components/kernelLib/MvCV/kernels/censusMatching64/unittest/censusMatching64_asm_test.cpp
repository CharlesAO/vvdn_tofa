#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (0.7)



TestRunner mvcvCensusMatching64TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMatching64CycleCount;

void mvcvCensusMatching64_asm_test(unsigned int *in1, unsigned int *in2, unsigned char *out, unsigned int width, unsigned int flag)
{
    unsigned int disparities = 64;
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    const unsigned int padding = 64 ;
    
    
    mvcvCensusMatching64TestRunner.Init();
    mvcvCensusMatching64TestRunner.SetInput("input1", in1, width + padding, SHAVE0);
    mvcvCensusMatching64TestRunner.SetInput("input2", in2, width + padding, SHAVE0);
    mvcvCensusMatching64TestRunner.SetInput("output", out, width * disparities + padding, SHAVE0);
    mvcvCensusMatching64TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    mvcvCensusMatching64TestRunner.SetInput("flag", (unsigned int)flag, SHAVE0);

    mvcvCensusMatching64TestRunner.GuardInsert(string("output"), SHAVE0, width * disparities + padding, out);
    mvcvCensusMatching64TestRunner.Run(SHAVE0);
    if(width >= 960)
    {
    mvcvCensusMatching64CycleCount = (mvcvCensusMatching64TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMatching64CycleCount)/ (float)(width * disparities));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvCensusMatching64TestRunner.GetOutput(string("output"), SHAVE0, width * disparities + padding, out);
    mvcvCensusMatching64TestRunner.GuardCheck(string("output"), SHAVE0, width * disparities + padding, out);
}
