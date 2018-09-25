#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (63)

TestRunner mvcvCensusMinConfidence32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMinConfidence32CycleCount;

void mvcvCensusMinConfidence32_asm_test(unsigned char *input, unsigned char *minimumPosition, unsigned char* minimumValue, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    const unsigned int disparities = 32 ;
    const unsigned int PADDING = 32 ;
    
    mvcvCensusMinConfidence32TestRunner.Init();
    mvcvCensusMinConfidence32TestRunner.SetInput("input", input, (width+PADDING) * disparities, SHAVE0);
    mvcvCensusMinConfidence32TestRunner.SetInput("minimumPosition", minimumPosition, width + PADDING, SHAVE0);
    mvcvCensusMinConfidence32TestRunner.SetInput("minimumValue", minimumValue, width + PADDING, SHAVE0);
    mvcvCensusMinConfidence32TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

    mvcvCensusMinConfidence32TestRunner.GuardInsert(string("minimumPosition"), SHAVE0, width + PADDING, minimumPosition);
    mvcvCensusMinConfidence32TestRunner.GuardInsert(string("minimumValue"), SHAVE0, width + PADDING, minimumValue);
    mvcvCensusMinConfidence32TestRunner.Run(SHAVE0);
    if(width >= 320)
    {
    mvcvCensusMinConfidence32CycleCount = (mvcvCensusMinConfidence32TestRunner.GetVariableValue(std::string("cycleCount")));
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMinConfidence32CycleCount)/ (float)(width));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvCensusMinConfidence32TestRunner.GetOutput(string("minimumPosition"), SHAVE0, width + PADDING, minimumPosition);
    mvcvCensusMinConfidence32TestRunner.GuardCheck(string("minimumPosition"), SHAVE0, width + PADDING, minimumPosition);
    mvcvCensusMinConfidence32TestRunner.GetOutput(string("minimumValue"), SHAVE0, width + PADDING, minimumValue);
    mvcvCensusMinConfidence32TestRunner.GuardCheck(string("minimumValue"), SHAVE0, width + PADDING, minimumValue);
}
