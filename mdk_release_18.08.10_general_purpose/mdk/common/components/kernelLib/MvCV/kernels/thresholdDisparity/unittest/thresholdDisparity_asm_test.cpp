#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.70)
#define PADDING 16 

TestRunner mvcvThresholdDisparityTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvThresholdDisparityCycleCount;

void mvcvThresholdDisparity_asm_test(unsigned char* input, unsigned char* output, unsigned int* countThresPixels, 
                                     unsigned int width, unsigned int threshold,
                	                 unsigned int setValue)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    mvcvThresholdDisparityTestRunner.Init();
    mvcvThresholdDisparityTestRunner.SetInput("input", input, width, SHAVE0);
    mvcvThresholdDisparityTestRunner.SetInput("output", output, width*2 + PADDING, SHAVE0);
    mvcvThresholdDisparityTestRunner.SetInput("countThresPixels", (unsigned char*)countThresPixels, PADDING*sizeof(unsigned int), SHAVE0);
    mvcvThresholdDisparityTestRunner.SetInput("threshold", (unsigned int)threshold, SHAVE0);
    mvcvThresholdDisparityTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    mvcvThresholdDisparityTestRunner.SetInput("setValue", (unsigned int)setValue, SHAVE0);
    mvcvThresholdDisparityTestRunner.Run(SHAVE0);
    mvcvThresholdDisparityCycleCount = (mvcvThresholdDisparityTestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvThresholdDisparityCycleCount)/ (float)(width*2));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    mvcvThresholdDisparityTestRunner.GetOutput(string("output"), SHAVE0, width*2 + PADDING, output);
    mvcvThresholdDisparityTestRunner.GetOutput(string("countThresPixels"), SHAVE0, PADDING*sizeof(unsigned int), (unsigned char*)countThresPixels);
}
