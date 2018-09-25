#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M2 (73)

#define EXPECTED_CC EXPECTED_CC_M2


TestRunner censusTransform11x11TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int censusTransform11x11CycleCount;

void censusTransform11x11_asm_test(unsigned short** in, unsigned char* out, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920 + PADDING;
    censusTransform11x11TestRunner.Init();
    censusTransform11x11TestRunner.SetInput("input", in, width + PADDING, maxWidth, 11, SHAVE0);
    censusTransform11x11TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    censusTransform11x11TestRunner.GuardInsert(string("output"), SHAVE0, width*16, out);
    censusTransform11x11TestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
    censusTransform11x11CycleCount = (censusTransform11x11TestRunner.GetVariableValue(std::string("cycleCount"))) - 15;
    functionInfo.AddCyclePerPixelInfo((float)(censusTransform11x11CycleCount)/ (float)(width));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    censusTransform11x11TestRunner.GetOutput(string("output"), SHAVE0, width*16, out);
    censusTransform11x11TestRunner.GuardCheck(string("output"), SHAVE0, width*16, out);
}
