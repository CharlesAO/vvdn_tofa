///
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (3.28)
#define EXPECTED_CC_M2 (3.788982)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner LookupTable10to16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int LookupTable10to16CycleCount;

void LUT10to16_asm_test(unsigned short** src, unsigned short** dest, unsigned short* lut, unsigned int width, unsigned int height)
{
    unsigned int maxWidth = 1920;
    
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    LookupTable10to16TestRunner.Init();
    LookupTable10to16TestRunner.SetInput("input", (unsigned char**)src, width * 2, maxWidth, 1, SHAVE0);
    LookupTable10to16TestRunner.SetInput("lut", (unsigned char*)lut, 4096 * 2, SHAVE0);
    LookupTable10to16TestRunner.SetInput("width", width, SHAVE0);
    LookupTable10to16TestRunner.SetInput("height", height, SHAVE0);
    LookupTable10to16TestRunner.GuardInsert("output", SHAVE0, width * 2, maxWidth, height, (unsigned char**)dest);
    LookupTable10to16TestRunner.Run(SHAVE0);
    if(width >= 1280)
	{
		LookupTable10to16CycleCount = LookupTable10to16TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(LookupTable10to16CycleCount - 11)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    LookupTable10to16TestRunner.GetOutput("output", SHAVE0, width * 2, maxWidth, height, (unsigned char**)dest);
    LookupTable10to16TestRunner.GuardCheck("output", SHAVE0, width * 2, maxWidth, height, (unsigned char**)dest);
}
