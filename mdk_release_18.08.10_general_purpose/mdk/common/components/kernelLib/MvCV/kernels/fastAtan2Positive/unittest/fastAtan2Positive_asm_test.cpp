#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"

TestRunner testRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void fastAtan2Positive_asm_test(half y, half x, half* result)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    testRunner.Init();
    testRunner.SetInput("x", &x,1, SHAVE0);
    testRunner.SetInput("y", &y,1, SHAVE0);
    testRunner.GuardInsert("result", SHAVE0, 2, (unsigned char*)result);
    testRunner.Run(SHAVE0);
    cycleCount = testRunner.GetVariableValue(std::string("cycleCount"));
    functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 2));

    testRunner.GetOutput("result", SHAVE0, 2, (unsigned char*)result);
    testRunner.GuardCheck("result", SHAVE0, 2, (unsigned char*)result);
}

