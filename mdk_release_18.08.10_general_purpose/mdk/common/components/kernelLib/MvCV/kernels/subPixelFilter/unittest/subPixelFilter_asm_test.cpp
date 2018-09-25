#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (4.83)


TestRunner mvcvSubPixelFilterTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvSubPixelFilterCycleCount;

void mvcvSubPixelFilter_asm_test(unsigned char* inputDisparityMap, unsigned char* inputCostVolume, 
                                 unsigned short* outputDisparityMap, unsigned int width, unsigned char maxDisp, 
                                 unsigned char numFractionalBit, unsigned char * lutToUse)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    const unsigned int padding = 16 ;
    
    
    mvcvSubPixelFilterTestRunner.Init();
    mvcvSubPixelFilterTestRunner.SetInput("inputDisparityMap", inputDisparityMap, width + padding, SHAVE0);
    mvcvSubPixelFilterTestRunner.SetInput("lutToUse", lutToUse, 4096, SHAVE0);
    mvcvSubPixelFilterTestRunner.SetInput("inputCostVolume", inputCostVolume, (width + padding) * maxDisp, SHAVE0);
    mvcvSubPixelFilterTestRunner.SetInput("outputDisparityMap", outputDisparityMap, width + padding, SHAVE0);

    mvcvSubPixelFilterTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    mvcvSubPixelFilterTestRunner.SetInput("maxDisp", (unsigned char)maxDisp, SHAVE0);
    mvcvSubPixelFilterTestRunner.SetInput("numFractionalBit", (unsigned char)numFractionalBit, SHAVE0);

    mvcvSubPixelFilterTestRunner.GuardInsert(string("outputDisparityMap"), SHAVE0, width + padding, outputDisparityMap);
    mvcvSubPixelFilterTestRunner.Run(SHAVE0);
    if(width >= 960)
    {
    mvcvSubPixelFilterCycleCount = (mvcvSubPixelFilterTestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
        functionInfo.AddCyclePerPixelInfo((float)(mvcvSubPixelFilterCycleCount)/ (float)(width));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvSubPixelFilterTestRunner.GetOutput(string("outputDisparityMap"), SHAVE0, width + padding, outputDisparityMap);
    mvcvSubPixelFilterTestRunner.GuardCheck(string("outputDisparityMap"), SHAVE0, width + padding, outputDisparityMap);
}
