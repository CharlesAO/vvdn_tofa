///
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.30)
#define EXPECTED_CC_M2 (1.552083)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner LookupTable10to8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int LookupTable10to8CycleCount;

void LUT10to8_asm_test(unsigned short** src, unsigned char** dest, unsigned char* lut, unsigned int width, unsigned int height)
{
    unsigned int maxWidth = 1920;
    
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    LookupTable10to8TestRunner.Init();
    LookupTable10to8TestRunner.SetInput("input", (unsigned char**)src, width * 2, maxWidth, 1, SHAVE0);
    LookupTable10to8TestRunner.SetInput("lut", lut, 4096, SHAVE0);
    LookupTable10to8TestRunner.SetInput("width", width, SHAVE0);
    LookupTable10to8TestRunner.SetInput("height", height, SHAVE0);
    LookupTable10to8TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, height, dest);
    LookupTable10to8TestRunner.Run(SHAVE0);
    if(width >= 1280)
	{
		LookupTable10to8CycleCount = LookupTable10to8TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(LookupTable10to8CycleCount - 11)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    LookupTable10to8TestRunner.GetOutput("output", SHAVE0, width, maxWidth, height, dest);
    LookupTable10to8TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, height, dest);
}
