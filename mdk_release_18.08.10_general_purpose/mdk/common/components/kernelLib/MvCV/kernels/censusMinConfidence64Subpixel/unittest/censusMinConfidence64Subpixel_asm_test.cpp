#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (240)
#define DISPARITIES 64
#define PADDING 16 

TestRunner mvcvCensusMinConfidence64SubpixelTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMinConfidence64SubpixelCycleCount;

void mvcvCensusMinConfidence64Subpixel_asm_test(unsigned char *in, half *out, unsigned char *outm, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    mvcvCensusMinConfidence64SubpixelTestRunner.Init();
    mvcvCensusMinConfidence64SubpixelTestRunner.SetInput(    "input",   in, width * DISPARITIES + PADDING, SHAVE0);
    mvcvCensusMinConfidence64SubpixelTestRunner.SetInput(      "out",  out, width + PADDING, SHAVE0);
    mvcvCensusMinConfidence64SubpixelTestRunner.SetInput(     "outm", outm, width + PADDING, SHAVE0);
    mvcvCensusMinConfidence64SubpixelTestRunner.SetInput(    "width", (unsigned int)width, SHAVE0);

    mvcvCensusMinConfidence64SubpixelTestRunner.GuardInsert(string("out"), SHAVE0, width + PADDING, out);
    mvcvCensusMinConfidence64SubpixelTestRunner.GuardInsert(string("outm"), SHAVE0, width + PADDING, outm);
    mvcvCensusMinConfidence64SubpixelTestRunner.Run(SHAVE0);
    if(width >= 320)
    {
    mvcvCensusMinConfidence64SubpixelCycleCount = (mvcvCensusMinConfidence64SubpixelTestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMinConfidence64SubpixelCycleCount)/ (float)(width));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvCensusMinConfidence64SubpixelTestRunner.GetOutput(string("out"), SHAVE0, width + PADDING, out);
    mvcvCensusMinConfidence64SubpixelTestRunner.GetOutput(string("outm"), SHAVE0, width + PADDING, outm);
    mvcvCensusMinConfidence64SubpixelTestRunner.GuardCheck(string("out"), SHAVE0, width + PADDING, out);
    mvcvCensusMinConfidence64SubpixelTestRunner.GuardCheck(string("outm"), SHAVE0, width + PADDING, outm);
}
