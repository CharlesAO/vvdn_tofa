#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M2 (26.12)

#define EXPECTED_CC EXPECTED_CC_M2


TestRunner censusTransform11x11u8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int censusTransform11x11u8CycleCount;

void censusTransform11x11u8_asm_test(unsigned char** in, unsigned char* out, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920 + PADDING;
    censusTransform11x11u8TestRunner.Init();
    censusTransform11x11u8TestRunner.SetInput("input", in, width + PADDING, maxWidth, 11, SHAVE0);
    censusTransform11x11u8TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
    censusTransform11x11u8TestRunner.GuardInsert(string("output"), SHAVE0, width*16, out);
    censusTransform11x11u8TestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
    censusTransform11x11u8CycleCount = (censusTransform11x11u8TestRunner.GetVariableValue(std::string("cycleCount"))) - 15;
    functionInfo.AddCyclePerPixelInfo((float)(censusTransform11x11u8CycleCount) / width);
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    censusTransform11x11u8TestRunner.GetOutput(string("output"), SHAVE0, width*16, out);
    censusTransform11x11u8TestRunner.GuardCheck(string("output"), SHAVE0, width*16, out);
}
