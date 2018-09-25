#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (8.7)
#define DISPARITIES 5
#define PADDING 16 

TestRunner mvcvCensusMinSubpixel3WindowTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMinSubpixel3WindowCycleCount;

void mvcvCensusMinSubpixel3Window_asm_test(unsigned char *in, half *out, unsigned char *outm, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    mvcvCensusMinSubpixel3WindowTestRunner.Init();
    mvcvCensusMinSubpixel3WindowTestRunner.SetInput(    "input",   in, width * DISPARITIES + PADDING, SHAVE0);
    mvcvCensusMinSubpixel3WindowTestRunner.SetInput(      "out",  out, width + PADDING, SHAVE0);
    mvcvCensusMinSubpixel3WindowTestRunner.SetInput(     "outm", outm, width + PADDING, SHAVE0);
    mvcvCensusMinSubpixel3WindowTestRunner.SetInput(    "width", (unsigned int)width, SHAVE0);

    mvcvCensusMinSubpixel3WindowTestRunner.GuardInsert(string("out"), SHAVE0, width + PADDING, out);
    mvcvCensusMinSubpixel3WindowTestRunner.GuardInsert(string("outm"), SHAVE0, width + PADDING, outm);
    mvcvCensusMinSubpixel3WindowTestRunner.Run(SHAVE0);
    if(width >= 320)
    {
    mvcvCensusMinSubpixel3WindowCycleCount = (mvcvCensusMinSubpixel3WindowTestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMinSubpixel3WindowCycleCount)/ (float)(width));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvCensusMinSubpixel3WindowTestRunner.GetOutput(string("out"), SHAVE0, width + PADDING, out);
    mvcvCensusMinSubpixel3WindowTestRunner.GetOutput(string("outm"), SHAVE0, width + PADDING, outm);
    mvcvCensusMinSubpixel3WindowTestRunner.GuardCheck(string("out"), SHAVE0, width + PADDING, out);
    mvcvCensusMinSubpixel3WindowTestRunner.GuardCheck(string("outm"), SHAVE0, width + PADDING, outm);
}
