#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.70)
#define PADDING 16 

TestRunner mvcvThresholdDisparityFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvThresholdDisparityFp16CycleCount;

void mvcvThresholdDisparityFp16_asm_test(unsigned char* input, half* output, unsigned int* countThresPixels, 
                                     unsigned int width, unsigned int threshold,
                	                 half setValue)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    mvcvThresholdDisparityFp16TestRunner.Init();
    mvcvThresholdDisparityFp16TestRunner.SetInput("input", input, width, SHAVE0);
    mvcvThresholdDisparityFp16TestRunner.SetInput("output", output, width*2 + PADDING, SHAVE0);
    mvcvThresholdDisparityFp16TestRunner.SetInput("countThresPixels", (unsigned char*)countThresPixels, PADDING*sizeof(unsigned int), SHAVE0);
    mvcvThresholdDisparityFp16TestRunner.SetInput("threshold", (unsigned int)threshold, SHAVE0);
    mvcvThresholdDisparityFp16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    mvcvThresholdDisparityFp16TestRunner.SetInput("setValue", (half*)&setValue, 1,  SHAVE0);

    mvcvThresholdDisparityFp16TestRunner.Run(SHAVE0);
    mvcvThresholdDisparityFp16CycleCount = (mvcvThresholdDisparityFp16TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvThresholdDisparityFp16CycleCount)/ (float)(width*2));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    mvcvThresholdDisparityFp16TestRunner.GetOutput(string("output"), SHAVE0, width*2 + PADDING, (half*)output);
    mvcvThresholdDisparityFp16TestRunner.GetOutput(string("countThresPixels"), SHAVE0, PADDING*sizeof(unsigned int), (unsigned char*)countThresPixels);
}
