#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (5.2)
#define DISPARITIES 3
#define PADDING 16 

TestRunner mvcvCensusMinSubpixel3TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMinSubpixel3CycleCount;

void mvcvCensusMinSubpixel3_asm_test(unsigned char *in, half *out, unsigned char *outm, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    mvcvCensusMinSubpixel3TestRunner.Init();
    mvcvCensusMinSubpixel3TestRunner.SetInput(    "input",   in, width * DISPARITIES + PADDING, SHAVE0);
    mvcvCensusMinSubpixel3TestRunner.SetInput(      "out",  out, width + PADDING, SHAVE0);
    mvcvCensusMinSubpixel3TestRunner.SetInput(     "outm", outm, width + PADDING, SHAVE0);
    mvcvCensusMinSubpixel3TestRunner.SetInput(    "width", (unsigned int)width, SHAVE0);

    mvcvCensusMinSubpixel3TestRunner.GuardInsert(string("out"), SHAVE0, width + PADDING, out);
    mvcvCensusMinSubpixel3TestRunner.GuardInsert(string("outm"), SHAVE0, width + PADDING, outm);
    mvcvCensusMinSubpixel3TestRunner.Run(SHAVE0);
    if(width >= 320)
    {
    mvcvCensusMinSubpixel3CycleCount = (mvcvCensusMinSubpixel3TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMinSubpixel3CycleCount)/ (float)(width));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvCensusMinSubpixel3TestRunner.GetOutput(string("out"), SHAVE0, width + PADDING, out);
    mvcvCensusMinSubpixel3TestRunner.GetOutput(string("outm"), SHAVE0, width + PADDING, outm);
    mvcvCensusMinSubpixel3TestRunner.GuardCheck(string("out"), SHAVE0, width + PADDING, out);
    mvcvCensusMinSubpixel3TestRunner.GuardCheck(string("outm"), SHAVE0, width + PADDING, outm);
}
