#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"

#define EXPECTED_CC 41

TestRunner testRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void maxTest3x3_fp16_asm_test(half* inBufferCandidates, half** inBuffer, unsigned int width, unsigned int maxLocationsIn[],
                              unsigned int maxLocationsOut[], unsigned int maxCountIn,  unsigned int* maxCountOut)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    testRunner.Init();
    testRunner.SetInput("inBufferCandidates", inBufferCandidates, width, SHAVE0);
    testRunner.SetInput("inBuffer", inBuffer, 3 * width, width, 3, SHAVE0);
    testRunner.SetInput("width", width, SHAVE0);
    testRunner.SetInput("maxLocationsIn", (unsigned char*)maxLocationsIn, 4*320, SHAVE0);
    testRunner.SetInput("maxLocationsOut", (unsigned char*)maxLocationsOut, 4*320, SHAVE0);
    testRunner.SetInput("maxCountIn", maxCountIn, SHAVE0);
    testRunner.SetInput("maxCountOut", *maxCountOut, SHAVE0);

    testRunner.GuardInsert("maxCountOut", SHAVE0, maxCountOut);
    testRunner.GuardInsert("maxLocationsOut", SHAVE0, 4*width, (unsigned char*)maxLocationsOut);
    testRunner.Run(SHAVE0);
    cycleCount = testRunner.GetVariableValue(std::string("cycleCount"));
    functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 15)/ (float)maxCountIn);
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    testRunner.GetOutput("maxCountOut", SHAVE0, maxCountOut);
    testRunner.GuardCheck("maxCountOut", SHAVE0, maxCountOut);
    testRunner.GetOutput("maxLocationsOut", SHAVE0, 4*width, (unsigned char*)maxLocationsOut);
    testRunner.GuardCheck("maxLocationsOut", SHAVE0, 4*width, (unsigned char*)maxLocationsOut);
}

