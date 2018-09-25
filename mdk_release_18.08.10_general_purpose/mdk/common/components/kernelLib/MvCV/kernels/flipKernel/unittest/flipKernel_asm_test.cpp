#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (32)


TestRunner mvcvFlipKernelTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvFlipKernelCycleCount;

void mvcvFlipKernel_asm_test(unsigned char *in1, unsigned char *out, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    const unsigned int PADDING = 32 ;
    
    
    mvcvFlipKernelTestRunner.Init();
    mvcvFlipKernelTestRunner.SetInput("input1", in1, width, SHAVE0);
    mvcvFlipKernelTestRunner.SetInput("output", out, width + PADDING, SHAVE0);
    mvcvFlipKernelTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

    mvcvFlipKernelTestRunner.GuardInsert(string("output"), SHAVE0, width + PADDING, out);
    mvcvFlipKernelTestRunner.Run(SHAVE0);
    if(width >= 960)
    {
    mvcvFlipKernelCycleCount = (mvcvFlipKernelTestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvFlipKernelCycleCount)/ (float)(width));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvFlipKernelTestRunner.GetOutput(string("output"), SHAVE0, width + PADDING, out);
    mvcvFlipKernelTestRunner.GuardCheck(string("output"), SHAVE0, width + PADDING, out);
}
