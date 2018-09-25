#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC (13.14)

TestRunner censusTransformAverageRefMask7x7TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int censusTransformAverageRefMask7x7CycleCount;

void censusTransformAverageRefMask7x7_asm_test(unsigned char** in, unsigned int* out, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920 + PADDING;

    censusTransformAverageRefMask7x7TestRunner.Init();
    censusTransformAverageRefMask7x7TestRunner.SetInput("input", in, width + PADDING, maxWidth, 7, SHAVE0);
    censusTransformAverageRefMask7x7TestRunner.SetInput("width", width, SHAVE0);
    censusTransformAverageRefMask7x7TestRunner.GuardInsert(string("output"), SHAVE0, width * sizeof(unsigned int), (unsigned char*)out);
    censusTransformAverageRefMask7x7TestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        censusTransformAverageRefMask7x7CycleCount = censusTransformAverageRefMask7x7TestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(censusTransformAverageRefMask7x7CycleCount - 10)/ (float)width);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);

    }
    censusTransformAverageRefMask7x7TestRunner.GetOutput(string("output"), SHAVE0, width * sizeof(unsigned int), (unsigned char*)out);
    censusTransformAverageRefMask7x7TestRunner.GuardCheck(string("output"), SHAVE0, width * sizeof(unsigned int), (unsigned char*)out);
}
