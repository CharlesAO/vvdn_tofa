#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (4.14)
#define EXPECTED_CC_M2 (2.723438)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner Gauss5x1_u32in_u16outTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int Gauss5x1_u32in_u16outCycleCount;

void Gauss5x1_u32in_u16out_asm_test(unsigned int** in, unsigned short** out, unsigned int width)
{

    FunctionInfo& functionInfo = FunctionInfo::Instance();

    unsigned int maxWidth = 1920;

    Gauss5x1_u32in_u16outTestRunner.Init();
    Gauss5x1_u32in_u16outTestRunner.SetInput("input", (unsigned char **)in, width * sizeof(unsigned int), maxWidth * sizeof(unsigned int), 5, SHAVE0);
    Gauss5x1_u32in_u16outTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

    
    Gauss5x1_u32in_u16outTestRunner.GuardInsert("output", SHAVE0, width * sizeof(unsigned short), maxWidth * sizeof(unsigned short), 1, (unsigned char **)out);
    Gauss5x1_u32in_u16outTestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        Gauss5x1_u32in_u16outCycleCount = Gauss5x1_u32in_u16outTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(Gauss5x1_u32in_u16outCycleCount - 16)/ (float)width);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    Gauss5x1_u32in_u16outTestRunner.GetOutput("output", SHAVE0, width * sizeof(unsigned short), maxWidth * sizeof(unsigned short), 1, (unsigned char **)out);
    Gauss5x1_u32in_u16outTestRunner.GuardCheck("output", SHAVE0, width * sizeof(unsigned short), maxWidth * sizeof(unsigned short), 1, (unsigned char **)out);
    
}
