#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (10.9)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner censusTransform7x7TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int censusTransform7x7CycleCount;

void censusTransform7x7_asm_test(unsigned char** in, unsigned long long* out, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920 + PADDING;

    censusTransform7x7TestRunner.Init();
    censusTransform7x7TestRunner.SetInput("input", in, width + PADDING, maxWidth, 7, SHAVE0);
    censusTransform7x7TestRunner.SetInput("width", width, SHAVE0);
    censusTransform7x7TestRunner.GuardInsert(string("output"), SHAVE0, width * sizeof(unsigned long long), (unsigned char*)out);
    censusTransform7x7TestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        censusTransform7x7CycleCount = censusTransform7x7TestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(censusTransform7x7CycleCount - 10)/ (float)width);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);

    }
    censusTransform7x7TestRunner.GetOutput(string("output"), SHAVE0, width * sizeof(unsigned long long), (unsigned char*)out);
    censusTransform7x7TestRunner.GuardCheck(string("output"), SHAVE0, width * sizeof(unsigned long long), (unsigned char*)out);
}
