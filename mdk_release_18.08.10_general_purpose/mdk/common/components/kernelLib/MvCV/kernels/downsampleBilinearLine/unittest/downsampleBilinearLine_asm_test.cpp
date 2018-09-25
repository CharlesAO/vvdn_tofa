#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (1.6)

TestRunner mvcvDownsampleBilinearLineTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvDownsampleBilinearLineCycleCount;

void mvcvDownsampleBilinearLine_asm_test(unsigned short *inLine, unsigned int *tempLine,
                                         unsigned short *outLine, unsigned int inWidth,
                                         unsigned int select)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    mvcvDownsampleBilinearLineTestRunner.Init();
    mvcvDownsampleBilinearLineTestRunner.SetInput("inLine", inLine, inWidth, SHAVE0);
    mvcvDownsampleBilinearLineTestRunner.SetInput("tempLine", tempLine, inWidth, SHAVE0);
    mvcvDownsampleBilinearLineTestRunner.SetInput("outLine", outLine, inWidth, SHAVE0);
    mvcvDownsampleBilinearLineTestRunner.SetInput("inWidth", (unsigned int)inWidth, SHAVE0);
    mvcvDownsampleBilinearLineTestRunner.SetInput("select_line", (unsigned int)select, SHAVE0);

    mvcvDownsampleBilinearLineTestRunner.GuardInsert(string("tempLine"), SHAVE0, inWidth*4, (unsigned char*)tempLine);
    mvcvDownsampleBilinearLineTestRunner.GuardInsert(string("outLine"), SHAVE0, inWidth, outLine);
    mvcvDownsampleBilinearLineTestRunner.Run(SHAVE0);
    if(inWidth >= 960)
    {
    mvcvDownsampleBilinearLineCycleCount = (mvcvDownsampleBilinearLineTestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvDownsampleBilinearLineCycleCount)/ (float)(inWidth/2));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvDownsampleBilinearLineTestRunner.GetOutput(string("tempLine"), SHAVE0, inWidth*4, (unsigned char*)tempLine);
    mvcvDownsampleBilinearLineTestRunner.GuardCheck(string("tempLine"), SHAVE0, inWidth*4, (unsigned char*)tempLine);
    mvcvDownsampleBilinearLineTestRunner.GetOutput(string("outLine"), SHAVE0, inWidth, outLine);
    mvcvDownsampleBilinearLineTestRunner.GuardCheck(string("outLine"), SHAVE0, inWidth, outLine);
}
