#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.83)
#define PADDING 16 

TestRunner mvcvComputeADPyrOnePosWindowTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvComputeADPyrOnePosWindowCycleCount;

void mvcvComputeADPyrOnePosWindow_asm_test(unsigned char *input1, unsigned char *input2, unsigned char *output, unsigned int width)
{
    unsigned int disparities = 5;
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    mvcvComputeADPyrOnePosWindowTestRunner.Init();
    mvcvComputeADPyrOnePosWindowTestRunner.SetInput(   "input1", input1, width * disparities + PADDING, SHAVE0);
    mvcvComputeADPyrOnePosWindowTestRunner.SetInput(   "input2", input2, width * disparities + PADDING, SHAVE0);
    mvcvComputeADPyrOnePosWindowTestRunner.SetInput(   "output", output, width * disparities + PADDING, SHAVE0);
        mvcvComputeADPyrOnePosWindowTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

    mvcvComputeADPyrOnePosWindowTestRunner.GuardInsert(string("output"), SHAVE0, width * disparities + PADDING, output);
    mvcvComputeADPyrOnePosWindowTestRunner.Run(SHAVE0);
        
    if(width>=320)
    {
    mvcvComputeADPyrOnePosWindowCycleCount = (mvcvComputeADPyrOnePosWindowTestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvComputeADPyrOnePosWindowCycleCount)/ (float)(width * disparities));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    
    mvcvComputeADPyrOnePosWindowTestRunner.GetOutput(string("output"), SHAVE0, width * disparities + PADDING, output);
    mvcvComputeADPyrOnePosWindowTestRunner.GuardCheck(string("output"), SHAVE0, width * disparities + PADDING, output);
}
