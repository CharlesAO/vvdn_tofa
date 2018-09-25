#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"

#define EXPECTED_CC 41

TestRunner testRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void minTest3x3_fp16_asm_test(half* inBufferCandidates, half** inBuffer, unsigned int width, unsigned int minLocationsIn[],
		unsigned int minLocationsOut[], unsigned int minCountIn,  unsigned int* minCountOut)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    testRunner.Init();
    testRunner.SetInput("inBufferCandidates", inBufferCandidates, width, SHAVE0);
    testRunner.SetInput("inBuffer", inBuffer, 3 * width,  width, 3, SHAVE0);
    testRunner.SetInput("width", width, SHAVE0);
    testRunner.SetInput("minLocationsIn", (unsigned char*)minLocationsIn, 4*320, SHAVE0);
    testRunner.SetInput("minLocationsOut", (unsigned char*)minLocationsOut, 4*320, SHAVE0);
    testRunner.SetInput("minCountIn", minCountIn, SHAVE0);
    testRunner.SetInput("minCountOut", *minCountOut, SHAVE0);
    testRunner.GuardInsert("minCountOut", SHAVE0, minCountOut);
    testRunner.GuardInsert("minLocationsOut", SHAVE0, 4*width, (unsigned char*)minLocationsOut);
    testRunner.Run(SHAVE0);
    cycleCount = testRunner.GetVariableValue(std::string("cycleCount"));
    functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 15)/ (float)minCountIn);
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    testRunner.GetOutput("minCountOut", SHAVE0, minCountOut);
    testRunner.GuardCheck("minCountOut", SHAVE0, minCountOut);
    testRunner.GetOutput("minLocationsOut", SHAVE0, 4*width, (unsigned char*)minLocationsOut);
    testRunner.GuardCheck("minLocationsOut", SHAVE0, 4*width, (unsigned char*)minLocationsOut);
}

