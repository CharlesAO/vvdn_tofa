#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"

TestRunner testRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void interpolatePixelBilinearS16_asm_test(short* line1, short* line2, float x, float y, float* result, unsigned int  width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    testRunner.Init();
    testRunner.SetInput("line1", (unsigned char*)line1, width*2, SHAVE0);
    testRunner.SetInput("line2", (unsigned char*)line2, width*2, SHAVE0);
    testRunner.SetInput("x", x, SHAVE0);
    testRunner.SetInput("y", y, SHAVE0);

    testRunner.GuardInsert("output", SHAVE0, 4, (unsigned char*)result);
    testRunner.Run(SHAVE0);
    cycleCount = testRunner.GetVariableValue(std::string("cycleCount"));

    functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 2));

    testRunner.GetOutput("output", SHAVE0, 4, (unsigned char*)result);
    testRunner.GuardCheck("output", SHAVE0, 4, (unsigned char*)result);
}

