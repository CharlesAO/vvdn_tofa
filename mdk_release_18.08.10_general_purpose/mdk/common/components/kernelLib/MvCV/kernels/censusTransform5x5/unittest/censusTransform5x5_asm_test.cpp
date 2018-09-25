#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (11.23)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner censusTransform5x5TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int censusTransform5x5CycleCount;

void censusTransform5x5_asm_test(unsigned char** in, unsigned int* out, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920 + PADDING;

    censusTransform5x5TestRunner.Init();
    censusTransform5x5TestRunner.SetInput("input", in, width + PADDING, maxWidth, 5, SHAVE0);
    censusTransform5x5TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    censusTransform5x5TestRunner.GuardInsert(string("output"), SHAVE0, width * 4, (unsigned char*)out);
    censusTransform5x5TestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        censusTransform5x5CycleCount = censusTransform5x5TestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(censusTransform5x5CycleCount - 10)/ (float)width);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    censusTransform5x5TestRunner.GetOutput(string("output"), SHAVE0, width * 4, (unsigned char*)out);
    censusTransform5x5TestRunner.GuardCheck(string("output"), SHAVE0, width * 4, (unsigned char*)out);
}
