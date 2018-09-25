#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (106)

TestRunner mvcvCensusMinConfidence64TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMinConfidence64CycleCount;

void mvcvCensusMinConfidence64_asm_test(unsigned char *input, unsigned char *minimumPosition, unsigned char* minimumValue, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    const unsigned int disparities = 64 ;
    const unsigned int PADDING = 32 ;
    
    mvcvCensusMinConfidence64TestRunner.Init();
    mvcvCensusMinConfidence64TestRunner.SetInput("input", input, (width+PADDING) * disparities, SHAVE0);
    mvcvCensusMinConfidence64TestRunner.SetInput("minimumPosition", minimumPosition, width + PADDING, SHAVE0);
    mvcvCensusMinConfidence64TestRunner.SetInput("minimumValue", minimumValue, width + PADDING, SHAVE0);
    mvcvCensusMinConfidence64TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

    mvcvCensusMinConfidence64TestRunner.GuardInsert(string("minimumPosition"), SHAVE0, width + PADDING, minimumPosition);
    mvcvCensusMinConfidence64TestRunner.GuardInsert(string("minimumValue"), SHAVE0, width + PADDING, minimumValue);
    mvcvCensusMinConfidence64TestRunner.Run(SHAVE0);
    if(width >= 320)
    {
    mvcvCensusMinConfidence64CycleCount = (mvcvCensusMinConfidence64TestRunner.GetVariableValue(std::string("cycleCount")));
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMinConfidence64CycleCount)/ (float)(width));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvCensusMinConfidence64TestRunner.GetOutput(string("minimumPosition"), SHAVE0, width + PADDING, minimumPosition);
    mvcvCensusMinConfidence64TestRunner.GuardCheck(string("minimumPosition"), SHAVE0, width + PADDING, minimumPosition);
    mvcvCensusMinConfidence64TestRunner.GetOutput(string("minimumValue"), SHAVE0, width + PADDING, minimumValue);
    mvcvCensusMinConfidence64TestRunner.GuardCheck(string("minimumValue"), SHAVE0, width + PADDING, minimumValue);
}
