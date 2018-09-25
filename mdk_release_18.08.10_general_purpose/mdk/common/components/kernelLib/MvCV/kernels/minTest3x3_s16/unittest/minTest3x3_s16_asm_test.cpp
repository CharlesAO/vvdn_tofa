#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"

#define EXPECTED_CC (39.3)

TestRunner testRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void minTest3x3_s16_asm_test(short* inBufferCandidates, short** inBuffer, unsigned int minLocationsIn[],
                              unsigned int minLocationsOut[], unsigned int minCountIn,  unsigned int* minCountOut)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    unsigned int maxWidth = 320;
    testRunner.Init();
    testRunner.SetInput("inBufferCandidates", (unsigned char *)inBufferCandidates, maxWidth*sizeof(short), SHAVE0);
    testRunner.SetInput("inBuffer", (unsigned char **)inBuffer, maxWidth*sizeof(short), maxWidth*sizeof(short), 3, SHAVE0);
    testRunner.SetInput("minLocationsIn", (unsigned char*)minLocationsIn, 4*320, SHAVE0);
    testRunner.SetInput("minLocationsOut", (unsigned char*)minLocationsOut, 4*320, SHAVE0);
    testRunner.SetInput("minCountIn", minCountIn, SHAVE0);
    testRunner.SetInput("minCountOut", *minCountOut, SHAVE0);
    testRunner.GuardInsert("minCountOut", SHAVE0, minCountOut);
    testRunner.GuardInsert("minLocationsOut", SHAVE0, 4*320, (unsigned char*)minLocationsOut);
    testRunner.Run(SHAVE0);
 
    cycleCount = testRunner.GetVariableValue("cycleCount");
    functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 7)/ (float)minCountIn);
    functionInfo.setExpectedCycles((float)EXPECTED_CC);

    testRunner.GetOutput("minCountOut", SHAVE0, minCountOut);
    testRunner.GuardCheck("minCountOut", SHAVE0, minCountOut);
    testRunner.GetOutput("minLocationsOut", SHAVE0, 4*320, (unsigned char*)minLocationsOut);
    testRunner.GuardCheck("minLocationsOut", SHAVE0, 4*320, (unsigned char*)minLocationsOut);
}

