#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"

TestRunner testRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void interpolatePixelBilinear_asm_test(half* line1, half* line2, float x, float y, half* result, unsigned int  width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    testRunner.Init();
    testRunner.SetInput("line1", line1, width, SHAVE0);
    testRunner.SetInput("line2", line2, width, SHAVE0);
    testRunner.SetInput("x", x, SHAVE0);
    testRunner.SetInput("y", y, SHAVE0);

    testRunner.GuardInsert("output", SHAVE0, 2, (unsigned char*)result);
    testRunner.Run(SHAVE0);
    cycleCount = testRunner.GetVariableValue(std::string("cycleCount"));

    functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 2));

    testRunner.GetOutput("output", SHAVE0, 2, (unsigned char*)result);
    testRunner.GuardCheck("output", SHAVE0, 2, (unsigned char*)result);
}

