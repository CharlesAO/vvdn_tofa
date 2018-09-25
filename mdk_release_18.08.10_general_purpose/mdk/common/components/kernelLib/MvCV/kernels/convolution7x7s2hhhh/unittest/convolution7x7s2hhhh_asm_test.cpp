#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (9.22)

TestRunner convolution7x7s2hhhhTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convolution7x7s2hhhhCycleCount;

void Convolution7x7s2hhhh_asm_test(half** in, half** out, half conv[49], unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    // for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
    const unsigned int padding = 16;
    // maxWidth - the maximum image width for which was allocated space in the board application;
    // it is needed to permit proper memory initialization for line sizes lower than it
    unsigned int maxWidth = 1920 + padding;
    convolution7x7s2hhhhTestRunner.Init();
    //add padding values at the end of each line
    convolution7x7s2hhhhTestRunner.SetInput("input", in, width + padding, maxWidth, 7, SHAVE0);
    convolution7x7s2hhhhTestRunner.SetInput("conv", (half*)&conv[0], 49, SHAVE0);
    convolution7x7s2hhhhTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    convolution7x7s2hhhhTestRunner.GuardInsert(string("output"), SHAVE0, width / 2, maxWidth, 1, out);
    convolution7x7s2hhhhTestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        convolution7x7s2hhhhCycleCount = convolution7x7s2hhhhTestRunner.GetVariableValue("cycleCount");
        //value 157 substracted from boxFilterCycleCount is the value of measured cycles
        //for computations not included in boxfilter_asm function. It was obtained by
        //running the application without the function call
        functionInfo.AddCyclePerPixelInfo((float)(convolution7x7s2hhhhCycleCount - 16)/ ((float)width/2));
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    convolution7x7s2hhhhTestRunner.GetOutput(string("output"), SHAVE0, width / 2, maxWidth, 1, out);
    convolution7x7s2hhhhTestRunner.GuardCheck(string("output"), SHAVE0, width / 2, maxWidth, 1, out);
}
