#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (3.68)



TestRunner mvcvCensusMatchingPyrOnePosWindowTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMatchingPyrOnePosWindowCycleCount;

void mvcvCensusMatchingPyrOnePosWindow_asm_test(unsigned int *in1, unsigned int *in2, unsigned char *predicted, unsigned char *out, unsigned int width)
{
    unsigned int disparities = 5;
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    const unsigned int padding = 32 ;
    
    
    mvcvCensusMatchingPyrOnePosWindowTestRunner.Init();
    mvcvCensusMatchingPyrOnePosWindowTestRunner.SetInput("input1", in1, width + padding, SHAVE0);
    mvcvCensusMatchingPyrOnePosWindowTestRunner.SetInput("input2", in2, width + padding, SHAVE0);
    mvcvCensusMatchingPyrOnePosWindowTestRunner.SetInput("predicted", predicted, width + padding, SHAVE0);
    mvcvCensusMatchingPyrOnePosWindowTestRunner.SetInput("output", out, width * disparities + padding, SHAVE0);
    mvcvCensusMatchingPyrOnePosWindowTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

    mvcvCensusMatchingPyrOnePosWindowTestRunner.GuardInsert(string("output"), SHAVE0, width * disparities + padding, out);
    mvcvCensusMatchingPyrOnePosWindowTestRunner.Run(SHAVE0);
    if (width>=960)
    {
    mvcvCensusMatchingPyrOnePosWindowCycleCount = (mvcvCensusMatchingPyrOnePosWindowTestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMatchingPyrOnePosWindowCycleCount)/ (float)(width * disparities));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvCensusMatchingPyrOnePosWindowTestRunner.GetOutput(string("output"), SHAVE0, width * disparities + padding, out);
    mvcvCensusMatchingPyrOnePosWindowTestRunner.GuardCheck(string("output"), SHAVE0, width * disparities + padding, out);
}
