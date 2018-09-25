#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"

#define EXPECTED_CC (39.3)

TestRunner testRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void maxTest3x3_s16_asm_test(short* inBufferCandidates, short** inBuffer, unsigned int maxLocationsIn[],
                              unsigned int maxLocationsOut[], unsigned int maxCountIn,  unsigned int* maxCountOut)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    unsigned int maxWidth = 320;

    testRunner.Init();
    testRunner.SetInput("inBufferCandidates", (unsigned char *)inBufferCandidates, maxWidth*sizeof(short), SHAVE0);
    testRunner.SetInput("inBuffer", (unsigned char **)inBuffer, maxWidth*sizeof(short), maxWidth*sizeof(short), 3, SHAVE0);
    testRunner.SetInput("maxLocationsIn", (unsigned char*)maxLocationsIn, 4*320, SHAVE0);
    testRunner.SetInput("maxLocationsOut", (unsigned char*)maxLocationsOut, 4*320, SHAVE0);
    testRunner.SetInput("maxCountIn", maxCountIn, SHAVE0);
    testRunner.SetInput("maxCountOut", *maxCountOut, SHAVE0);
    testRunner.GuardInsert("maxCountOut", SHAVE0, maxCountOut);
    testRunner.GuardInsert("maxLocationsOut", SHAVE0, 4*320, (unsigned char*)maxLocationsOut);
    testRunner.Run(SHAVE0);

    cycleCount = testRunner.GetVariableValue("cycleCount");
    functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 7)/ (float)maxCountIn);
    functionInfo.setExpectedCycles((float)EXPECTED_CC);

    testRunner.GetOutput("maxCountOut", SHAVE0, maxCountOut);
    testRunner.GuardCheck("maxCountOut", SHAVE0, maxCountOut);
    testRunner.GetOutput("maxLocationsOut", SHAVE0, 4*320, (unsigned char*)maxLocationsOut);
    testRunner.GuardCheck("maxLocationsOut", SHAVE0, 4*320, (unsigned char*)maxLocationsOut);
}

